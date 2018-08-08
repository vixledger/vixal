// Copyright 2014 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "transactions/OfferExchange.h"
#include "database/Database.h"
#include "ledger/LedgerDelta.h"
#include "ledger/LedgerManager.h"
#include "ledger/TrustFrame.h"
#include "util/uint128_t.h"
#include "util/Logging.h"

namespace vixal {

// returns the amount of wheat that would be traded
// while buying as much sheep as possible

int64_t
canSellAtMostBasedOnSheep(Asset const &sheep, TrustFrame::pointer sheepLine,
                          Price const &wheatPrice, LedgerManager &ledgerManager) {
    if (sheep.type() == ASSET_TYPE_NATIVE) {
        return INT64_MAX;
    }

    // compute value based on what the account can receive
    auto sellerMaxSheep =
            sheepLine ? sheepLine->getMaxAmountReceive(ledgerManager) : 0;

    auto wheatAmount = int64_t{};
    if (!bigDivide(wheatAmount, sellerMaxSheep, wheatPrice.d, wheatPrice.n,
                   ROUND_DOWN)) {
        wheatAmount = INT64_MAX;
    }

    return wheatAmount;
}

int64_t
canSellAtMost(AccountFrame::pointer account, Asset const &asset,
              TrustFrame::pointer trustLine, LedgerManager &ledgerManager) {
    if (asset.type() == ASSET_TYPE_NATIVE) {
        // can only send above the minimum balance
        return std::max(
                {account->getAvailableBalance(ledgerManager), int64_t(0)});
    }

    if (trustLine && trustLine->isAuthorized()) {
        return std::max(
                {trustLine->getAvailableBalance(ledgerManager), int64_t(0)});
    }

    return 0;
}

int64_t
canBuyAtMost(AccountFrame::pointer account, Asset const &asset,
             TrustFrame::pointer trustLine, LedgerManager &ledgerManager) {
    if (asset.type() == ASSET_TYPE_NATIVE) {
        return std::max(
                {account->getMaxAmountReceive(ledgerManager), int64_t(0)});
    } else {
        return trustLine
               ? std::max({trustLine->getMaxAmountReceive(ledgerManager),
                           int64_t(0)})
               : 0;
    }
}

ExchangeResult
exchangeV2(int64_t wheatReceived, Price price, int64_t maxWheatReceive,
           int64_t maxSheepSend) {
    auto result = ExchangeResult{};
    result.reduced = wheatReceived > maxWheatReceive;
    wheatReceived = std::min(wheatReceived, maxWheatReceive);

    // this guy can get X wheat to you. How many sheep does that get him?
    if (!bigDivide(result.numSheepSend, wheatReceived, price.n, price.d,
                   ROUND_DOWN)) {
        result.numSheepSend = INT64_MAX;
    }

    result.reduced = result.reduced || (result.numSheepSend > maxSheepSend);
    result.numSheepSend = std::min(result.numSheepSend, maxSheepSend);
    // bias towards seller (this cannot overflow at this point)
    result.numWheatReceived =
            bigDivide(result.numSheepSend, price.d, price.n, ROUND_DOWN);

    return result;
}

ExchangeResult
exchangeV3(int64_t wheatReceived, Price price, int64_t maxWheatReceive,
           int64_t maxSheepSend) {
    auto result = ExchangeResult{};
    result.reduced = wheatReceived > maxWheatReceive;
    result.numWheatReceived = std::min(wheatReceived, maxWheatReceive);

    // this guy can get X wheat to you. How many sheep does that get him?
    // bias towards seller
    if (!bigDivide(result.numSheepSend, result.numWheatReceived, price.n,
                   price.d, ROUND_UP)) {
        result.reduced = true;
        result.numSheepSend = INT64_MAX;
    }

    result.reduced = result.reduced || (result.numSheepSend > maxSheepSend);
    result.numSheepSend = std::min(result.numSheepSend, maxSheepSend);

    auto newWheatReceived = int64_t{};
    if (!bigDivide(newWheatReceived, result.numSheepSend, price.d, price.n,
                   ROUND_DOWN)) {
        newWheatReceived = INT64_MAX;
    }
    result.numWheatReceived =
            std::min(result.numWheatReceived, newWheatReceived);

    return result;
}

// Check that the relative error between the price and the effective price does
// not exceed 1%. If canFavorWheat == true then this function does an asymmetric
// check such that error favoring the seller of wheat can be unbounded, while
// the relative error between the price and the effective price does not exceed
// 1% if it is favoring the seller of sheep. The functionality of canFavorWheat
// is required for PathPayment.
bool
checkPriceErrorBound(Price price, int64_t wheatReceive, int64_t sheepSend,
                     bool canFavorWheat) {
    // Let K = 100 / threshold, where threshold is the maximum relative error in
    // percent (so in this case, threshold = 1%). Then we can rearrange the
    // formula for relative error as follows:
    //     abs(price - effPrice) <= price / K
    //     price.d * abs(price - effPrice) <= price.n / K
    //     abs(price.n - price.d * effPrice) <= price.n / K
    //     abs(price.n * effPrice.d - price.d * effPrice.n)
    //         <= price.n * effPrice.d / K
    //     abs(K * price.n * effPrice.d - K * price.d * effPrice.n)
    //         <= price.n * effPrice.d

    // These never overflow since price.n and price.d are int32_t
    int64_t errN = (int64_t) 100 * (int64_t) price.n;
    int64_t errD = (int64_t) 100 * (int64_t) price.d;

    uint128_t lhs = bigMultiply(errN, wheatReceive);
    uint128_t rhs = bigMultiply(errD, sheepSend);

    if (canFavorWheat && rhs > lhs) {
        return true;
    }

    uint128_t absDiff = (lhs > rhs) ? (lhs - rhs) : (rhs - lhs);
    uint128_t cap = bigMultiply(price.n, wheatReceive);
    return (absDiff <= cap);
}

static uint128_t
calculateOfferValue(int32_t priceN, int32_t priceD, int64_t maxSend,
                    int64_t maxReceive) {
    uint128_t sendValue = bigMultiply(maxSend, priceN);
    uint128_t receiveValue = bigMultiply(maxReceive, priceD);
    return std::min({sendValue, receiveValue});
}

// exchangeV10 is a system for crossing offers that provides guarantees
// regarding the direction and magnitude of rounding errors:
// - When considering two crossing offers subject to a variety of limits,
//   exchangeV10 has a consistent approach to determining which offer is larger.
//   The smaller offer is always removed from the book.
// - When two offers cross, the rounding error will favor the offer that remains
//   in the book.
// - The rounding error will not favor either party by more than 1% (except in
//   the case of path payment, where the rounding error can favor the offer in
//   the book by an arbitrary amount). If the rounding error would exceed 1%,
//   no trade occurs and the smaller offer is removed.
//
// As mentioned above, exchangeV10 retains the same guarantees when it is used
// to perform path payment except that the rounding error can favor the offer in
// the book by an arbitrary amount. This behavior is required in order to
// guarantee that, if the amount offered in the book exceeds maxWheatReceive
// after every offer is adjusted, then exchangeV10 will satisfy the constraint
// imposed by path payment of wheatReceive == maxWheatReceive. We note that this
// behavior is acceptable because path payment uses maxSend to determine whether
// the operation should succeed given the final effective price. If excessive
// rounding occurs, then path payment will fail because maxSend was exceeded.
//
// The quantities wheatValue and sheepValue play a central role in this function
// so it is worth discussing their significance. If we were working in arbitrary
// precision arithmetic, we would find that the value of the wheat offer in
// terms of sheep is
//     wheatOfferInTermsOfSheep = min(maxWheatSend * price, maxSheepReceive)
// and the size of the sheep offer in terms of sheep is
//     sheepOfferInTermsOfSheep = min(maxWheatReceive * price, maxSheepSend)
// Then we can see that the wheat offer is larger than the sheep offer if
//     wheatOfferInTermsOfSheep > sheepOfferInTermsOfSheep
// We are not, however, working in arbitrary precision arithmetic so we proceed
// by multiplying by price.d which yields
//     wheatOfferInTermsOfSheep * price.d
//         = min(maxWheatSend * price.n, maxSheepReceive * price.d)
//         = wheatValue
// and
//     sheepOfferInTermsOfSheep * price.d
//         = min(maxWheatReceive * price.n, maxSheepSend * price.d)
//         = sheepValue
// where both wheatValue and sheepValue are now integers. Clearly it is
// equivalent to say that the wheat offer is larger than the sheep offer if
//     wheatValue > sheepValue
// From this analysis, wheatValue can be thought of as the rescaled size of the
// wheat offer in terms of sheep after considering all limits. Analogously,
// sheepValue can be thought of as the rescaled size of the sheep offer in terms
// of sheep after considering all limits.
//
// If isPathPayment == false --------------------------------------------------
// We first consider the case (wheatStays && price.n > price.d). Then
//     wheatReceive = floor(sheepValue / price.n)
//                  <= sheepValue / price.n
//                  <= (maxWheatReceive * price.n) / price.n
//                  = maxWheatReceive
//     wheatReceive = floor(sheepValue / price.n)
//                  <= sheepValue / price.n
//                  < wheatValue / price.n
//                  <= maxWheatSend
// so wheatReceive cannot exceed its limits. Similarly,
//     sheepSend = ceil(wheatReceive * price.n / price.d)
//               = ceil(floor(sheepValue / price.n) * price.n / price.d)
//               <= ceil((sheepValue / price.n) * price.n / price.d)
//               = ceil(sheepValue / price.d)
//               <= ceil((maxSheepSend * price.d) / price.d)
//               = maxSheepSend
//     sheepSend = ceil(wheatReceive * price.n / price.d)
//               = ceil(floor(sheepValue / price.n) * price.n / price.d)
//               <= ceil((sheepValue / price.n) * price.n / price.d)
//               = ceil(sheepValue / price.d)
//               <= ceil(wheatValue / price.d)
//               <= ceil((maxSheepReceive * price.d) / price.d)
//               = maxSheepReceive
// so sheepSend cannot exceed its limits. Because the limits for both
// wheatReceive and sheepSend are int64_t, neither bigDivide can fail. Now the
// effective price would be
//     sheepSend / wheatReceive
//         = ceil(wheatReceive * price.n / price.d) / wheatReceive
//         >= (wheatReceive * price.n / price.d) / wheatReceive
//         = price.n / price.d
// so in this case the seller of wheat is favored.
//
// We next consider the case (wheatStays && price.n <= price.d). Then
//     sheepSend = floor(sheepValue / price.d)
//               <= sheepValue / price.d
//               <= maxSheepSend
//     sheepSend = floor(sheepValue / price.d)
//               <= sheepValue / price.d
//               < wheatValue / price.d
//               <= maxSheepReceive
// so sheepSend cannot exceed its limits. Similarly,
//     wheatReceive = floor(sheepSend * price.d / price.n)
//                  <= sheepSend * price.d / price.n
//                  <= floor(sheepValue / price.d) * price.d / price.n
//                  <= (sheepValue / price.d) * price.d / price.n
//                  = sheepValue / price.n
//                  <= maxWheatReceive
//     wheatReceive = floor(sheepSend * price.d / price.n)
//                  <= sheepSend * price.d / price.n
//                  <= floor(sheepValue / price.d) * price.d / price.n
//                  <= (sheepValue / price.d) * price.d / price.n
//                  = sheepValue / price.n
//                  < wheatValue / price.n
//                  <= maxWheatSend
// so wheatReceive cannot exceed its limits. Because the limits for both
// wheatReceive and sheepSend are int64_t, neither bigDivide can fail. Now the
// effective price would be
//     sheepSend / wheatReceive
//         = sheepSend / floor(sheepSend * price.d / price.n)
//         >= sheepSend / (sheepSend * price.d / price.n)
//         = price.n / price.d
// so in this case the seller of wheat is favored.
//
// We now shift attention to the case (!wheatStays && price.n > price.d). Then
//     wheatReceive = floor(wheatValue / price.n)
//                  <= wheatValue / price.n
//                  <= maxWheatSend
//     wheatReceive = floor(wheatValue / price.n)
//                  <= wheatValue / price.n
//                  <= sheepValue / price.n
//                  = maxWheatSend
// so wheatReceive cannot exceed its limits. Similarly,
//     sheepSend = floor(wheatReceive * price.n / price.d)
//               <= wheatReceive * price.n / price.d
//               = floor(wheatValue / price.n) * price.n / price.d
//               <= (wheatValue / price.n) * price.n / price.d
//               = wheatValue / price.d
//               <= maxSheepReceive
//     sheepSend = floor(wheatReceive * price.n / price.d)
//               <= wheatReceive * price.n / price.d
//               = floor(wheatValue / price.n) * price.n / price.d
//               <= (wheatValue / price.n) * price.n / price.d
//               = wheatValue / price.d
//               < sheepValue / price.d
//               <= maxSheepSend
// so sheepSend cannot exceed its limits. Because the limits for both
// wheatReceive and sheepSend are int64_t, neither bigDivide can fail. Now the
// effective price would be
//     sheepSend / wheatReceive
//         = floor(wheatReceive * price.n / price.d) / wheatReceive
//         <= (wheatReceive * price.n / price.d) / wheatReceive
//         = price.n / price.d
// so in this case the seller of sheep is favored.
//
// Finally, we come to the case (!wheatStays && price.n <= price.d). Then
//     sheepSend = floor(wheatValue / price.d)
//               <= wheatValue / price.d
//               <= maxSheepReceive
//     sheepSend = floor(wheatValue / price.d)
//               <= wheatValue / price.d
//               <= sheepValue / price.d
//               = maxSheepSend
// so sheepSend cannot exceed its limits. Similarly,
//     wheatReceive = ceil(sheepSend * price.d / price.n)
//                  = ceil(floor(wheatValue / price.d) * price.d / price.n)
//                  <= ceil((wheatValue / price.d) * price.d / price.n)
//                  = ceil(wheatValue / price.n)
//                  <= maxWheatSend
//     wheatReceive = ceil(sheepSend * price.d / price.n)
//                  = ceil(floor(wheatValue / price.d) * price.d / price.n)
//                  <= ceil((wheatValue / price.d) * price.d / price.n)
//                  = ceil(wheatValue / price.n)
//                  <= ceil(sheepValue / price.n)
//                  <= maxWheatReceive
// so wheatReceive cannot exceed its limits. Because the limits for both
// wheatReceive and sheepSend are int64_t, neither bigDivide can fail. Now the
// effective price would be
//     sheepSend / wheatReceive
//         = sheepSend / ceil(sheepSend * price.d / price.n)
//         <= sheepSend / (sheepSend * price.d / price.n)
//         = price.n / price.d
// so in this case the seller of sheep is favored.
//
// If isPathPayment == true ---------------------------------------------------
// We first consider the case wheatStays. In this case, we guarantee that the
// effective price favors wheat
//     sheepSend / wheatReceive >= price.n / price.d
// Path payment can only succeed if, after the last offer is crossed,
// wheatReceive == maxWheatReceive. Because wheatStays, we know that this is the
// last offer that will be crossed. Then if we are to satisfy both constraints,
// it is necessary that
//     sheepSend / maxWheatReceive >= price.n / price.d
// which is equivalent to
//     sheepSend * price.d >= maxWheatReceive * price.n
// But sheepSend <= maxSheepSend, so this can only be satisfied if
//     maxSheepSend * price.d >= maxWheatReceive * price.n             (*)
// If this constraint is not satisfied, then the operation must fail.
//
// We will now show that the case (wheatStays && price.n > price.d) along with
// the constraint (*) guarantees that wheatReceive == maxWheatReceive. In this
// case we have
//     sheepValue = maxWheatReceive * price.n
// so
//     wheatReceive = floor(sheepValue / price.n)
//                  = maxWheatReceive
// and
//     sheepSend = ceil(wheatReceive * price.n / price.d)
//                = ceil(maxWheatReceive * price.n / price.d)
// Clearly the operation succeeds.
//
// We will next show that if wheatReceive == maxWheatReceive in the case
// (wheatStays && price.n <= price.d) then
//     sheepSend = ceil(maxWheatReceive * price.n / price.d)
// so the outcome is unchanged from (wheatStays && price.n > price.d). Note that
// the constraint (*) is satisfied since it is a necessary condition to have
// wheatReceive == maxWheatReceive. Then
//     sheepValue = maxWheatReceive * price.n
// so
//     sheepSend = floor(sheepValue / price.d)
//               = floor(maxWheatReceive * price.n / price.d)
// which is equivalent to
//     maxWheatReceive * price.n / price.d - 1 < sheepSend
//         <= maxWheatReceive * price.n / price.d
// Furthermore, we have
//     maxWheatReceive = floor(sheepSend * price.d / price.n)
// which is equivalent to
//     maxWheatReceive <= sheepSend * price.d / price.n < maxWheatReceive + 1
//     maxWheatReceive * price.n / price.d <= sheepSend
//         < (maxWheatReceive + 1) * price.n / price.d
// Combining the above inequalities, we find
//     maxWheatReceive * price.n / price.d <= sheepSend
//         <= maxWheatReceive * price.n / price.d
// so clearly we have
//     sheepSend = maxWheatReceive * price.n / price.d
// But sheepSend is an integer, so
//     sheepSend = ceil(sheepSend)
//               = ceil(maxWheatReceive * price.n / price.d)
// which completes this argument.
//
// Now we come to the reason that when wheatStays we handle all cases the same
// regardless of the price. The previous argument showed that the two cases are
// identical when wheatReceive == maxWheatReceive. But it is possible in the
// case (wheatStays && price.n <= price.d) that wheatReceive < maxWheatReceive.
// Consider the case
//     price = 2/3
//     maxWheatSend = 150
//     maxWheatReceive = 101
//     maxSheepSend = INT64_MAX
//     maxSheepReceive = INT64_MAX
//     isPathPayment = true
// so
//     wheatValue = min(2 * 150, 3 * INT64_MAX) = 300
//     sheepValue = min(3 * INT64_MAX, 2 * 101) = 202
// which implies (wheatStays && price.n <= price.d). Then
//     sheepSend = floor(sheepValue / price.d)
//               = floor(202 / 3) = 67
//     wheatReceive = floor(sheepSend * price.d / price.n)
//                  = floor(67 * 3 / 2) = 100
// and clearly wheatReceive == 100 != 101 == maxWheatReceive.
//
// At this point we have determined what must occur if wheatStays but have not
// addressed the case !wheatStays. If wheatReceive == maxWheatReceive, then the
// operation succeeds. If wheatReceive < maxWheatReceive, then the operation
// will cross additional offers since !wheatStays.
ExchangeResultV10
exchangeV10(Price price, int64_t maxWheatSend, int64_t maxWheatReceive,
            int64_t maxSheepSend, int64_t maxSheepReceive, bool isPathPayment) {
    auto beforeThresholds = exchangeV10WithoutPriceErrorThresholds(
            price, maxWheatSend, maxWheatReceive, maxSheepSend, maxSheepReceive,
            isPathPayment);
    return applyPriceErrorThresholds(
            price, beforeThresholds.numWheatReceived, beforeThresholds.numSheepSend,
            beforeThresholds.wheatStays, isPathPayment);
}

// See comment before exchangeV10.
ExchangeResultV10
exchangeV10WithoutPriceErrorThresholds(Price price, int64_t maxWheatSend,
                                       int64_t maxWheatReceive,
                                       int64_t maxSheepSend,
                                       int64_t maxSheepReceive,
                                       bool isPathPayment) {
    uint128_t wheatValue =
            calculateOfferValue(price.n, price.d, maxWheatSend, maxSheepReceive);
    uint128_t sheepValue =
            calculateOfferValue(price.d, price.n, maxSheepSend, maxWheatReceive);
    bool wheatStays = (wheatValue > sheepValue);

    int64_t wheatReceive;
    int64_t sheepSend;
    if (wheatStays) {
        if (price.n > price.d || isPathPayment) // Wheat is more valuable
        {
            wheatReceive = bigDivide(sheepValue, price.n, ROUND_DOWN);
            sheepSend = bigDivide(wheatReceive, price.n, price.d, ROUND_UP);
        } else // Sheep is more valuable
        {
            sheepSend = bigDivide(sheepValue, price.d, ROUND_DOWN);
            wheatReceive = bigDivide(sheepSend, price.d, price.n, ROUND_DOWN);
        }
    } else {
        if (price.n > price.d) // Wheat is more valuable
        {
            wheatReceive = bigDivide(wheatValue, price.n, ROUND_DOWN);
            sheepSend = bigDivide(wheatReceive, price.n, price.d, ROUND_DOWN);
        } else // Sheep is more valuable
        {
            sheepSend = bigDivide(wheatValue, price.d, ROUND_DOWN);
            wheatReceive = bigDivide(sheepSend, price.d, price.n, ROUND_UP);
        }
    }

    // Neither of these should ever throw.
    if (wheatReceive < 0 ||
        wheatReceive > std::min({maxWheatReceive, maxWheatSend})) {
        throw std::runtime_error("wheatReceive out of bounds");
    }
    if (sheepSend < 0 || sheepSend > std::min({maxSheepReceive, maxSheepSend})) {
        throw std::runtime_error("sheepSend out of bounds");
    }

    ExchangeResultV10 res;
    res.numWheatReceived = wheatReceive;
    res.numSheepSend = sheepSend;
    res.wheatStays = wheatStays;
    return res;
}

// See comment before exchangeV10.
ExchangeResultV10
applyPriceErrorThresholds(Price price, int64_t wheatReceive, int64_t sheepSend,
                          bool wheatStays, bool isPathPayment) {
    if (wheatReceive > 0 && sheepSend > 0) {
        uint128_t wheatReceiveValue = bigMultiply(wheatReceive, price.n);
        uint128_t sheepSendValue = bigMultiply(sheepSend, price.d);

        // ExchangeV10 guarantees that if wheat stays then the wheat seller
        // must be favored. Similarly, if sheep stays then the sheep seller
        // must be favored.
        if (wheatStays && sheepSendValue < wheatReceiveValue) {
            throw std::runtime_error("favored sheep when wheat stays");
        }
        if (!wheatStays && sheepSendValue > wheatReceiveValue) {
            throw std::runtime_error("favored wheat when sheep stays");
        }

        if (!isPathPayment) {
            // Both sellers must get a price no more than 1% worse than the
            // price crossed. Otherwise, no trade occurs.
            if (!checkPriceErrorBound(price, wheatReceive, sheepSend, false)) {
                sheepSend = 0;
                wheatReceive = 0;
            }
        } else {
            // When the wheat seller is favored, they can be arbitrarily favored
            // since path payment has a sendMax parameter to determine whether
            // a price was acceptable. When the sheep seller is favored, we
            // still want the wheat seller to get a price no more than 1% worse
            // than the price crossed. The sheep seller can only be favored if
            // !wheatStays, and in this case the entire offer will be taken. But
            // the offer was adjusted immediately before exchangeV10, so we
            // know that it satisfies the threshold in this case.
            if (!checkPriceErrorBound(price, wheatReceive, sheepSend, true)) {
                throw std::runtime_error("exceeded price error bound");
            }
        }
    } else {
        wheatReceive = 0;
        sheepSend = 0;
    }

    ExchangeResultV10 res;
    res.numWheatReceived = wheatReceive;
    res.numSheepSend = sheepSend;
    res.wheatStays = wheatStays;
    return res;
}

LoadBestOfferContext::LoadBestOfferContext(Database &db, Asset const &selling,
                                           Asset const &buying)
        : mSelling(selling), mBuying(buying), mDb(db), mBatchIterator(mBatch.end()) {
    loadBatchIfNecessary();
}

void
LoadBestOfferContext::loadBatchIfNecessary() {
    if (mBatchIterator == mBatch.end()) {
        mBatch.clear();
        OfferFrame::loadBestOffers(5, 0, mSelling, mBuying, mBatch, mDb);
        mBatchIterator = mBatch.begin();
    }
}

OfferFrame::pointer
LoadBestOfferContext::loadBestOffer() {
    return (mBatchIterator != mBatch.end()) ? *mBatchIterator : nullptr;
}

void
LoadBestOfferContext::eraseAndUpdate() {
    ++mBatchIterator;
    loadBatchIfNecessary();
}


OfferExchange::OfferExchange(LedgerDelta &delta, LedgerManager &ledgerManager)
        : mDelta(delta), mLedgerManager(ledgerManager) {
}

OfferExchange::CrossOfferResult
OfferExchange::crossOffer(OfferFrame &sellingWheatOffer,
                          int64_t maxWheatReceived, int64_t &numWheatReceived,
                          int64_t maxSheepSend, int64_t &numSheepSend) {
    assert(maxWheatReceived > 0);
    assert(maxSheepSend > 0);

    // we're about to make changes to the offer
    mDelta.recordEntry(sellingWheatOffer);

    Asset &sheep = sellingWheatOffer.getOffer().buying;
    Asset &wheat = sellingWheatOffer.getOffer().selling;
    AccountID &accountBID = sellingWheatOffer.getOffer().sellerID;

    Database &db = mLedgerManager.getDatabase();

    AccountFrame::pointer accountB;
    accountB = AccountFrame::loadAccount(mDelta, accountBID, db);
    if (!accountB) {
        throw std::runtime_error(
                "invalid database state: offer must have matching account");
    }

    TrustFrame::pointer wheatLineAccountB;
    if (wheat.type() != ASSET_TYPE_NATIVE) {
        wheatLineAccountB =
                TrustFrame::loadTrustLine(accountBID, wheat, db, &mDelta);
    }

    TrustFrame::pointer sheepLineAccountB;
    if (sheep.type() != ASSET_TYPE_NATIVE) {
        sheepLineAccountB =
                TrustFrame::loadTrustLine(accountBID, sheep, db, &mDelta);
    }

    numWheatReceived = std::min(
            {canSellAtMostBasedOnSheep(sheep, sheepLineAccountB,
                                       sellingWheatOffer.getOffer().price,
                                       mLedgerManager),
             canSellAtMost(accountB, wheat, wheatLineAccountB, mLedgerManager),
             sellingWheatOffer.getOffer().amount});
    assert(numWheatReceived >= 0);

    sellingWheatOffer.getOffer().amount = numWheatReceived;
    auto exchangeResult =
            mLedgerManager.getCurrentLedgerVersion() < 3
            ? exchangeV2(numWheatReceived, sellingWheatOffer.getOffer().price,
                         maxWheatReceived, maxSheepSend)
            : exchangeV3(numWheatReceived, sellingWheatOffer.getOffer().price,
                         maxWheatReceived, maxSheepSend);

    numWheatReceived = exchangeResult.numWheatReceived;
    numSheepSend = exchangeResult.numSheepSend;

    bool offerTaken = false;

    switch (exchangeResult.type()) {
        case ExchangeResultType::REDUCED_TO_ZERO:
            return eOfferCantConvert;
        case ExchangeResultType::BOGUS:
            // force delete the offer as it represents a bogus offer
            numWheatReceived = 0;
            numSheepSend = 0;
            offerTaken = true;
            break;
        default:
            break;
    }

    offerTaken =
            offerTaken || sellingWheatOffer.getOffer().amount <= numWheatReceived;
    if (offerTaken) { // entire offer is taken
        sellingWheatOffer.storeDelete(mDelta, db);

        accountB->addNumEntries(-1, mLedgerManager);
        accountB->storeChange(mDelta, db);
    } else {
        sellingWheatOffer.getOffer().amount -= numWheatReceived;
        sellingWheatOffer.storeChange(mDelta, db);
    }

    // Adjust balances
    if (numSheepSend != 0) {
        if (sheep.type() == ASSET_TYPE_NATIVE) {
            if (!accountB->addBalance(numSheepSend, mLedgerManager)) {
                return eOfferCantConvert;
            }
            accountB->storeChange(mDelta, db);
        } else {
            if (!sheepLineAccountB->addBalance(numSheepSend, mLedgerManager)) {
                return eOfferCantConvert;
            }
            sheepLineAccountB->storeChange(mDelta, db);
        }
    }

    if (numWheatReceived != 0) {
        if (wheat.type() == ASSET_TYPE_NATIVE) {
            if (!accountB->addBalance(-numWheatReceived, mLedgerManager)) {
                return eOfferCantConvert;
            }
            accountB->storeChange(mDelta, db);
        } else {
            if (!wheatLineAccountB->addBalance(-numWheatReceived, mLedgerManager)) {
                return eOfferCantConvert;
            }
            wheatLineAccountB->storeChange(mDelta, db);
        }
    }

    mOfferTrail.push_back(
            ClaimOfferAtom(accountB->getID(), sellingWheatOffer.getOfferID(), wheat,
                           numWheatReceived, sheep, numSheepSend));

    return offerTaken ? eOfferTaken : eOfferPartial;
}

void
adjustOffer(OfferFrame &offer, LedgerManager &lm, AccountFrame::pointer account,
            Asset const &wheat, TrustFrame::pointer wheatLine,
            Asset const &sheep, TrustFrame::pointer sheepLine) {

    OfferEntry &oe = offer.getOffer();
    int64_t maxWheatSend =
            std::min({oe.amount, canSellAtMost(account, wheat, wheatLine, lm)});
    int64_t maxSheepReceive = canBuyAtMost(account, sheep, sheepLine, lm);
    oe.amount = adjustOffer(oe.price, maxWheatSend, maxSheepReceive);
}

// The central property of adjustOffer is that it has no effect when applied to
// an offer which has already been adjusted. In what follows, we will prove this
// is true by considering each case individually. As a preliminary step, we
// first show that we must only consider cases where sheep stays in exchangeV10.
// To see this, note that
//     sheepValue = min(maxSheepSend * price.d, maxWheatReceive * price.n)
//                = min(INT64_MAX * price.d, INT64_MAX * price.n)
//                = INT64_MAX * min(price.d, price.n)
// and
//     wheatValue = min(maxWheatSend * price.n, maxSheepReceive * price.d)
//                <= min(INT64_MAX * price.n, INT64_MAX * price.d)
//                = INT64_MAX * min(price.n, price.d)
//                = sheepValue
// so sheep stays. This is expected, since adjustOffer is modeling the case
// where a buyer with no limits crosses an offer on the book.
//
// We will first consider the case where price.n > price.d and
//     maxWheatSend * price.n < maxSheepReceive * price.d
// Combining the above inequality with the definition of wheatValue, we see
//     wheatValue = maxWheatSend * price.n
// so in the adjusted offer we have
//     maxWheatSend' = wheatReceive
//                   = floor(wheatValue / price.n)
//                   = maxWheatSend
// so we conclude that the offer is not modified by adjustOffer. Clearly if the
// offer is not modified by adjustOffer, then the adjusted offer is also not
// modified by adjustOffer.
//
// We next consider the case where price.n > price.d and
//     maxWheatSend * price.n >= maxSheepReceive * price.d
// Combining the above inequality with the definition of wheatValue, we see
//     wheatValue = maxSheepReceive * price.d
// so in the adjusted offer we have
//     maxWheatSend' = wheatReceive
//                   = floor(wheatValue / price.n)
//                   = floor(maxSheepReceive * price.d / price.n)
// Now we suppose that adjustOffer was applied to the adjusted offer. We first
// observe that
//     maxWheatSend' * price.n
//         = floor(maxSheepReceive * price.d / price.n) * price.n
//         <= (maxSheepReceive * price.d / price.n) * price.n
//         = maxSheepReceive * price.d
// which combined with the defition of wheatValue yields
//     wheatValue' = maxWheatSend' * price.n
// From this we find that
//     wheatReceive' = floor(wheatValue' / price.n)
//                   = maxWheatSend'
// so we conclude that the adjusted offer is not modified by adjustOffer.
//
// We now consider the case where price.n <= price.d and
//     maxWheatSend * price.n < maxSheepReceive * price.d
// Combining the above inequality with the definition of wheatValue, we see
//     wheatValue = maxWheatSend * price.n
// from which it follows that
//     sheepSend = floor(wheatValue / price.d)
//               = floor(maxWheatSend * price.n / price.d)
// so in the adjusted offer we have
//     maxWheatSend'
//         = wheatReceive
//         = ceil(sheepSend * price.d / price.n)
//         = ceil(floor(maxWheatSend * price.n / price.d) * price.d / price.n)
// Now we suppose that adjustOffer was applied to the adjusted offer. We first
// observe that
//     maxWheatSend' * price.n
//         = ceil(floor(maxWheatSend * price.n / price.d) * price.d / price.n)
//               * price.n
//         <= ceil((maxWheatSend * price.n / price.d) * price.d / price.n)
//               * price.n
//         = maxWheatSend * price.n
//         < maxSheepReceive * price.d
// which combined with the definition of wheatValue yields
//     wheatValue' = maxWheatSend' * price.n
// From this we find that
//     sheepSend'
//         = floor(wheatValue / price.d)
//         = floor(maxWheatSend' * price.n / price.d)
//         = floor(ceil(sheepSend * price.d / price.n) * price.n / price.d)
//         >= floor((sheepSend * price.d / price.n) * price.n / price.d)
//         = sheepSend
//     sheepSend'
//         = floor(wheatValue / price.d)
//         = floor(maxWheatSend' * price.n / price.d)
//         = floor(ceil(sheepSend * price.d / price.n) * price.n / price.d)
//         = floor(ceil(floor(maxWheatSend * price.n / price.d)
//               * price.d / price.n) * price.n / price.d)
//         <= floor(ceil((maxWheatSend * price.n / price.d) * price.d / price.n)
//               * price.n / price.d)
//         = floor(maxWheatSend * price.n / price.d)
//         = sheepSend
// so sheepSend' = sheepSend after combining both inequalities. Then
//     wheatReceive' = ceil(sheepSend' * price.d / price.n)
//                   = ceil(sheepSend * price.d / price.n)
//                   = maxWheatSend'
// so we conclude that the adjusted offer is not modified by adjustOffer.
//
// Finally, we turn our attention to the case price.n <= price.d and
//     maxWheatSend * price.n >= maxSheepReceive * price.d
// Combining the above inequality with the definition of wheatValue, we see
//     wheatValue = maxSheepReceive * price.d
// from which it follows that
//     sheepSend = floor(wheatValue / price.d)
//               = floor((maxSheepReceive * price.d) / price.d)
//               = maxSheepReceive
// so in the adjusted offer we have
//     maxWheatSend' = wheatReceive
//                   = ceil(sheepSend * price.d / price.n)
//                   = ceil(maxSheepReceive * price.d / price.n)
// Now we suppose that adjustOffer was applied to the adjusted offer. We first
// observe that
//     maxWheatSend' * price.n
//         = ceil(maxSheepReceive * price.d / price.n) * price.n
//         >= (maxSheepReceive * price.d / price.n) * price.n
//         = maxSheepReceive * price.d
// which combined with the definition of wheatValue yields
//     wheatValue' = maxSheepReceive * price.d
// From this we find that
//     sheepSend' = floor(wheatValue / price.d)
//                = floor((maxSheepReceive * price.d) / price.d)
//                = maxSheepReceive
//                = sheepSend
// Then
//     wheatReceive' = ceil(sheepSend' * price.d / price.n)
//                   = ceil(sheepSend * price.d / price.n)
//                   = wheatReceive
// so we conclude that the adjusted offer is not modified by adjustOffer.
int64_t
adjustOffer(Price const &price, int64_t maxWheatSend, int64_t maxSheepReceive) {
    auto res = exchangeV10(price, maxWheatSend, INT64_MAX, INT64_MAX,
                           +maxSheepReceive, false);
    return res.numWheatReceived;
}

OfferExchange::CrossOfferResult
OfferExchange::crossOfferV10(OfferFrame &sellingWheatOffer,
                             int64_t maxWheatReceived,
                             int64_t &numWheatReceived, int64_t maxSheepSend,
                             int64_t &numSheepSend, bool &wheatStays,
                             bool isPathPayment) {
    assert(maxWheatReceived > 0);
    assert(maxSheepSend > 0);

    // we're about to make changes to the offer
    mDelta.recordEntry(sellingWheatOffer);

    Asset &sheep = sellingWheatOffer.getOffer().buying;
    Asset &wheat = sellingWheatOffer.getOffer().selling;
    AccountID &accountBID = sellingWheatOffer.getOffer().sellerID;

    Database &db = mLedgerManager.getDatabase();
    AccountFrame::pointer accountB;
    accountB = AccountFrame::loadAccount(mDelta, accountBID, db);
    if (!accountB) {

        throw std::runtime_error(
                "invalid database state: offer must have matching account");
    }

    TrustFrame::pointer wheatLineAccountB;
    if (wheat.type() != ASSET_TYPE_NATIVE) {
        wheatLineAccountB =
                TrustFrame::loadTrustLine(accountBID, wheat, db, &mDelta);
    }

    TrustFrame::pointer sheepLineAccountB;
    if (sheep.type() != ASSET_TYPE_NATIVE) {
        sheepLineAccountB =
                TrustFrame::loadTrustLine(accountBID, sheep, db, &mDelta);
    }

    // Remove liabilities associated with the offer being crossed.
    if (mLedgerManager.getCurrentLedgerVersion() >= 10) {
        sellingWheatOffer.releaseLiabilities(accountB, sheepLineAccountB,
                                             wheatLineAccountB, mDelta, db,
                                             mLedgerManager);
    }
    // Note: As of the asset-backed offers proposal, this call to adjustOffer
    // should only effect offers that remained after the version upgrade.
    adjustOffer(sellingWheatOffer, mLedgerManager, accountB, wheat,
                wheatLineAccountB, sheep, sheepLineAccountB);

    int64_t maxWheatSend =
            canSellAtMost(accountB, wheat, wheatLineAccountB, mLedgerManager);
    maxWheatSend =
            std::min({sellingWheatOffer.getOffer().amount, maxWheatSend});
    int64_t maxSheepReceive =
            canBuyAtMost(accountB, sheep, sheepLineAccountB, mLedgerManager);
    auto exchangeResult = exchangeV10(
            sellingWheatOffer.getOffer().price, maxWheatSend, maxWheatReceived,
            maxSheepSend, maxSheepReceive, isPathPayment);

    numWheatReceived = exchangeResult.numWheatReceived;
    numSheepSend = exchangeResult.numSheepSend;
    wheatStays = exchangeResult.wheatStays;

    // Adjust balances
    if (numSheepSend != 0) {
        if (sheep.type() == ASSET_TYPE_NATIVE) {
            if (!accountB->addBalance(numSheepSend, mLedgerManager)) {
                throw std::runtime_error("overflowed sheep balance");
            }
            accountB->storeChange(mDelta, db);
        } else {
            if (!sheepLineAccountB->addBalance(numSheepSend, mLedgerManager)) {
                throw std::runtime_error("overflowed sheep balance");
            }
            sheepLineAccountB->storeChange(mDelta, db);
        }
    }

    if (numWheatReceived != 0) {
        if (wheat.type() == ASSET_TYPE_NATIVE) {
            if (!accountB->addBalance(-numWheatReceived, mLedgerManager)) {
                throw std::runtime_error("overflowed wheat balance");
            }
            accountB->storeChange(mDelta, db);
        } else {
            if (!wheatLineAccountB->addBalance(-numWheatReceived, mLedgerManager)) {
                throw std::runtime_error("overflowed wheat balance");
            }
            wheatLineAccountB->storeChange(mDelta, db);
        }
    }
    if (wheatStays) {
        sellingWheatOffer.getOffer().amount -= numWheatReceived;
        adjustOffer(sellingWheatOffer, mLedgerManager, accountB, wheat,
                    wheatLineAccountB, sheep, sheepLineAccountB);
    } else {
        sellingWheatOffer.getOffer().amount = 0;
    }

    if (sellingWheatOffer.getOffer().amount == 0) {
        sellingWheatOffer.storeDelete(mDelta, db);

        accountB->addNumEntries(-1, mLedgerManager);
        accountB->storeChange(mDelta, db);
    } else {
        if (mLedgerManager.getCurrentLedgerVersion() >= 10) {
            sellingWheatOffer.acquireLiabilities(accountB, sheepLineAccountB,
                                                 wheatLineAccountB, mDelta, db,
                                                 mLedgerManager);
        }
        sellingWheatOffer.storeChange(mDelta, db);
    }

    mOfferTrail.push_back(
            ClaimOfferAtom(accountB->getID(), sellingWheatOffer.getOfferID(), wheat,
                           numWheatReceived, sheep, numSheepSend));

    return (sellingWheatOffer.getOffer().amount == 0) ? eOfferTaken
                                                      : eOfferPartial;
}

OfferExchange::ConvertResult
OfferExchange::convertWithOffers(
        Asset const &sheep, int64_t maxSheepSend, int64_t &sheepSend,
        Asset const &wheat, int64_t maxWheatReceive, int64_t &wheatReceived,
        bool isPathPayment,
        std::function<OfferFilterResult(OfferFrame const &)> filter) {
    sheepSend = 0;
    wheatReceived = 0;

    Database &db = mLedgerManager.getDatabase();

    bool needMore = (maxWheatReceive > 0 && maxSheepSend > 0);
    LoadBestOfferContext context(db, wheat, sheep);
    OfferFrame::pointer wheatOffer;
    while (needMore && (wheatOffer = context.loadBestOffer())) {
        if (filter && filter(*wheatOffer) == eStop) {
            return eFilterStop;
        }

        int64_t numWheatReceived;
        int64_t numSheepSend;

        CrossOfferResult cor;
        if (mLedgerManager.getCurrentLedgerVersion() >= 10) {
            bool wheatStays;
            cor = crossOfferV10(*wheatOffer, maxWheatReceive, numWheatReceived,
                                maxSheepSend, numSheepSend, wheatStays,
                                isPathPayment);
            needMore = !wheatStays;
        } else {
            cor = crossOffer(*wheatOffer, maxWheatReceive, numWheatReceived,
                             maxSheepSend, numSheepSend);
            needMore = true;
        }

        assert(numSheepSend >= 0);
        assert(numSheepSend <= maxSheepSend);
        assert(numWheatReceived >= 0);
        assert(numWheatReceived <= maxWheatReceive);

        switch (cor) {
            case eOfferTaken:
                context.eraseAndUpdate();
                break;
            case eOfferPartial:
                break;
            case eOfferCantConvert:
                return ePartial;
        }

        sheepSend += numSheepSend;
        maxSheepSend -= numSheepSend;

        wheatReceived += numWheatReceived;
        wheatReceived += numWheatReceived;
        maxWheatReceive -= numWheatReceived;

        needMore = needMore && (maxWheatReceive > 0 && maxSheepSend > 0);
        if (!needMore) {
            return eOK;
        } else if (cor == eOfferPartial) {
            return ePartial;
        }
    }
    if ((mLedgerManager.getCurrentLedgerVersion() < 10) || !needMore) {
        return eOK;
    } else {
        return ePartial;
    }
}
}


