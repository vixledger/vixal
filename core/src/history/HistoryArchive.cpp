// Copyright 2014 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

// ASIO is somewhat particular about when it gets included -- it wants to be the
// first to include <windows.h> -- so we try to include it before everything
// else.
#include "util/asio.h"
#include "history/HistoryArchive.h"
#include "CoreVersion.h"
#include "bucket/Bucket.h"
#include "bucket/BucketList.h"
#include "history/HistoryManager.h"
#include "util/format.h"
#include "application/Application.h"
#include "util/Fs.h"
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#include <set>

namespace vixal {

unsigned const HistoryArchiveState::HISTORY_ARCHIVE_STATE_VERSION = 1;

template<typename... Tokens>
std::string
formatString(std::string const &templateString, Tokens const &... tokens) {
    try {
        return fmt::format(templateString, tokens...);
    } catch (fmt::FormatError const &ex) {
        CLOG(ERROR, "History") << "failed to format string \"" << templateString
                               << "\":" << ex.what();
        throw std::runtime_error("failed to format command string");
    }
}

bool
HistoryArchiveState::futuresAllReady() const {
    for (auto const &level : currentBuckets) {
        if (level.next.isMerging()) {
            if (!level.next.mergeComplete()) {
                return false;
            }
        }
    }
    return true;
}

bool
HistoryArchiveState::futuresAllResolved() const {
    for (auto const &level : currentBuckets) {
        if (level.next.isMerging()) {
            return false;
        }
    }
    return true;
}

void
HistoryArchiveState::resolveAllFutures() {
    for (auto &level : currentBuckets) {
        if (level.next.isMerging()) {
            level.next.resolve();
        }
    }
}

void
HistoryArchiveState::resolveAnyReadyFutures() {
    for (auto &level : currentBuckets) {
        if (level.next.isMerging() && level.next.mergeComplete()) {
            level.next.resolve();
        }
    }
}

void
HistoryArchiveState::save(std::string const &outFile) const {
    assert(futuresAllResolved());
    std::ofstream out(outFile);
    cereal::JSONOutputArchive ar(out);
    serialize(ar);
}

std::string
HistoryArchiveState::toString() const {
    std::ostringstream out;
    {
        cereal::JSONOutputArchive ar(out);
        serialize(ar);
    }
    return out.str();
}

void
HistoryArchiveState::load(std::string const &inFile) {
    std::ifstream in(inFile);
    cereal::JSONInputArchive ar(in);
    serialize(ar);
    if (version != HISTORY_ARCHIVE_STATE_VERSION) {
        CLOG(ERROR, "History") << "unexpected history archive state version: " << version;
        throw std::runtime_error("unexpected history archive state version");
    }
    assert(futuresAllResolved());
}

void
HistoryArchiveState::fromString(std::string const &str) {
    std::istringstream in(str);
    cereal::JSONInputArchive ar(in);
    serialize(ar);
    assert(futuresAllResolved());
}

std::string
HistoryArchiveState::baseName() {
    return std::string("vixal-history.json");
}

std::string
HistoryArchiveState::wellKnownRemoteDir() {
    // The RFC 5785 dir
    return std::string(".well-known");
}

std::string
HistoryArchiveState::wellKnownRemoteName() {
    return wellKnownRemoteDir() + "/" + baseName();
}

std::string
HistoryArchiveState::remoteDir(uint32_t snapshotNumber) {
    return fs::remoteDir("history", fs::hexStr(snapshotNumber));
}

std::string
HistoryArchiveState::remoteName(uint32_t snapshotNumber) {
    return fs::remoteName("history", fs::hexStr(snapshotNumber), "json");
}

std::string
HistoryArchiveState::localName(Application &app, std::string const &archiveName) {
    return app.getHistoryManager().localFilename(archiveName + "-" + baseName());
}

Hash
HistoryArchiveState::getBucketListHash() {
    // NB: This hash algorithm has to match "what the BucketList does" to
    // calculate its BucketList hash exactly. It's not a particularly complex
    // algorithm -- just hash all the hashes of all the bucket levels, in order,
    // with each level-hash being the hash of its curr bucket then its snap
    // bucket -- but we duplicate the logic here because it honestly seems like
    // it'd be less readable to try to abstract the code between the two
    // relatively-different representations. Everything will explode if there is
    // any difference in these algorithms anyways, so..

    auto totalHash = SHA256::create();
    for (auto const &level : currentBuckets) {
        auto levelHash = SHA256::create();
        levelHash->add(hexToBin(level.curr));
        levelHash->add(hexToBin(level.snap));
        totalHash->add(levelHash->finish());
    }
    return totalHash->finish();
}

std::vector<std::string>
HistoryArchiveState::differingBuckets(HistoryArchiveState const &other) const {
    assert(futuresAllResolved());
    std::set<std::string> inhibit;
    uint256 zero;
    inhibit.insert(binToHex(zero));
    for (auto b : other.currentBuckets) {
        inhibit.insert(b.curr);
        if (b.next.isLive()) {
            b.next.resolve();
        }
        if (b.next.hasOutputHash()) {
            inhibit.insert(b.next.getOutputHash());
        }
        inhibit.insert(b.snap);
    }
    std::vector<std::string> ret;
    for (uint32_t i = BucketList::kNumLevels; i != 0; --i) {
        auto s = currentBuckets[i - 1].snap;
        auto n = s;
        if (currentBuckets[i - 1].next.hasOutputHash()) {
            n = currentBuckets[i - 1].next.getOutputHash();
        }
        auto c = currentBuckets[i - 1].curr;
        auto bs = {s, n, c};
        for (auto const &j : bs) {
            if (inhibit.find(j) == inhibit.end()) {
                ret.push_back(j);
                inhibit.insert(j);
            }
        }
    }
    return ret;
}

std::vector<std::string>
HistoryArchiveState::allBuckets() const {
    std::set<std::string> buckets;
    for (auto const &level : currentBuckets) {
        buckets.insert(level.curr);
        buckets.insert(level.snap);
        auto nh = level.next.getHashes();
        buckets.insert(nh.begin(), nh.end());
    }
    return std::vector<std::string>(buckets.begin(), buckets.end());
}

HistoryArchiveState::HistoryArchiveState() : server(VIXAL_CORE_VERSION) {
    uint256 u;
    std::string s = binToHex(u);
    HistoryStateBucket b;
    b.curr = s;
    b.snap = s;
    while (currentBuckets.size() < BucketList::kNumLevels) {
        currentBuckets.push_back(b);
    }
}

HistoryArchiveState::HistoryArchiveState(uint32_t ledgerSeq,
                                         BucketList const &buckets)
        : server(VIXAL_CORE_VERSION), currentLedger(ledgerSeq) {
    for (uint32_t i = 0; i < BucketList::kNumLevels; ++i) {
        HistoryStateBucket b;
        auto &level = buckets.getLevel(i);
        b.curr = binToHex(level.getCurr()->getHash());
        b.next = level.getNext();
        b.snap = binToHex(level.getSnap()->getHash());
        currentBuckets.push_back(b);
    }
}

HistoryArchive::HistoryArchive(HistoryArchiveConfiguration const &config)
        : mConfig(config) {
}

HistoryArchive::~HistoryArchive() {
}

bool
HistoryArchive::hasGetCmd() const {
    return !mConfig.mGetCmd.empty();
}

bool
HistoryArchive::hasPutCmd() const {
    return !mConfig.mPutCmd.empty();
}

bool
HistoryArchive::hasMkdirCmd() const {
    return !mConfig.mMkdirCmd.empty();
}

std::string const &
HistoryArchive::getName() const {
    return mConfig.mName;
}

std::string
HistoryArchive::getFileCmd(std::string const &remote, std::string const &local) const {
    if (mConfig.mGetCmd.empty()) {
        return "";
    }
    return formatString(mConfig.mGetCmd, remote, local);
}

std::string
HistoryArchive::putFileCmd(std::string const &local, std::string const &remote) const {
    if (mConfig.mPutCmd.empty()) {
        return "";
    }
    return formatString(mConfig.mPutCmd, remote, local);
}

std::string
HistoryArchive::mkdirCmd(std::string const &remoteDir) const {
    if (mConfig.mMkdirCmd.empty()) {
        return "";
    }
    return formatString(mConfig.mMkdirCmd, remoteDir);
}

void
HistoryArchive::markSuccess() {
    mSuccess++;
}

void
HistoryArchive::markFailure() {
    mFailure++;
}

Json::Value
HistoryArchive::getJsonInfo() const {
    Json::Value result;
    result["success"] = mSuccess;
    result["failure"] = mFailure;
    return result;
}
}
