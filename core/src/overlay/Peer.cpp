// Copyright 2014 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "overlay/Peer.h"

#include "overlay/BanManager.h"
#include "crypto/Hex.h"
#include "crypto/Random.h"
#include "crypto/SHA.h"
#include "herder/Herder.h"
#include "application/Application.h"
#include "overlay/LoadManager.h"
#include "overlay/OverlayManager.h"
#include "overlay/PeerAuth.h"
#include "overlay/PeerRecord.h"
#include "util/Logging.h"

#include "xdrpp/marshal.h"

#include <util/random.hpp>

/// LATER: need to add some way of docking peers that are misbehaving by sending you bad data

namespace vixal {

using namespace std;
using namespace soci;

using xdr::operator<;

using random_t = random_static;

medida::Meter &
Peer::getByteReadMeter(Application &app) {
    return app.getMetrics().newMeter({"overlay", "byte", "read"}, "byte");
}

medida::Meter &
Peer::getByteWriteMeter(Application &app) {
    return app.getMetrics().newMeter({"overlay", "byte", "write"}, "byte");
}

Peer::Peer(Application &app, PeerRole role)
        : mApp(app),
          mRole(role),
          mState(role == WE_CALLED_REMOTE ? CONNECTING : CONNECTED),
          mRemoteOverlayVersion(0),
          mRemoteListeningPort(0),
          mIdleTimer(app.getClock()),
          mLastRead(app.getClock().now()),
          mLastWrite(app.getClock().now()),
          mMessageRead(app.getMetrics().newMeter({"overlay", "message", "read"}, "message")),
          mMessageWrite(app.getMetrics().newMeter({"overlay", "message", "write"}, "message")),
          mByteRead(getByteReadMeter(app)),
          mByteWrite(getByteWriteMeter(app)),
          mErrorRead(app.getMetrics().newMeter({"overlay", "error", "read"}, "error")),
          mErrorWrite(app.getMetrics().newMeter({"overlay", "error", "write"}, "error")),
          mTimeoutIdle(app.getMetrics().newMeter({"overlay", "timeout", "idle"}, "timeout")),
          mRecvErrorTimer(app.getMetrics().newTimer({"overlay", "recv", "error"})),
          mRecvHelloTimer(app.getMetrics().newTimer({"overlay", "recv", "hello"})),
          mRecvAuthTimer(app.getMetrics().newTimer({"overlay", "recv", "auth"})),
          mRecvDontHaveTimer(app.getMetrics().newTimer({"overlay", "recv", "dont-have"})),
          mRecvGetPeersTimer(app.getMetrics().newTimer({"overlay", "recv", "get-peers"})),
          mRecvPeersTimer(app.getMetrics().newTimer({"overlay", "recv", "peers"})),
          mRecvGetTxSetTimer(app.getMetrics().newTimer({"overlay", "recv", "get-txset"})),
          mRecvTxSetTimer(app.getMetrics().newTimer({"overlay", "recv", "txset"})),
          mRecvTransactionTimer(app.getMetrics().newTimer({"overlay", "recv", "transaction"})),
          mRecvGetSCPQuorumSetTimer(app.getMetrics().newTimer({"overlay", "recv", "get-scp-qset"})),
          mRecvSCPQuorumSetTimer(app.getMetrics().newTimer({"overlay", "recv", "scp-qset"})),
          mRecvSCPMessageTimer(app.getMetrics().newTimer({"overlay", "recv", "scp-message"})),
          mRecvGetSCPStateTimer(app.getMetrics().newTimer({"overlay", "recv", "get-scp-state"})),
          mRecvSCPPrepareTimer(app.getMetrics().newTimer({"overlay", "recv", "scp-prepare"})),
          mRecvSCPConfirmTimer(app.getMetrics().newTimer({"overlay", "recv", "scp-confirm"})),
          mRecvSCPNominateTimer(app.getMetrics().newTimer({"overlay", "recv", "scp-nominate"})),
          mRecvSCPExternalizeTimer(app.getMetrics().newTimer({"overlay", "recv", "scp-externalize"})),
          mSendErrorMeter(app.getMetrics().newMeter({"overlay", "send", "error"}, "message")),
          mSendHelloMeter(app.getMetrics().newMeter({"overlay", "send", "hello"}, "message")),
          mSendAuthMeter(app.getMetrics().newMeter({"overlay", "send", "auth"}, "message")),
          mSendDontHaveMeter(app.getMetrics().newMeter({"overlay", "send", "dont-have"}, "message")),
          mSendGetPeersMeter(app.getMetrics().newMeter({"overlay", "send", "get-peers"}, "message")),
          mSendPeersMeter(app.getMetrics().newMeter({"overlay", "send", "peers"}, "message")),
          mSendGetTxSetMeter(app.getMetrics().newMeter({"overlay", "send", "get-txset"}, "message")),
          mSendTransactionMeter(app.getMetrics().newMeter({"overlay", "send", "transaction"}, "message")),
          mSendTxSetMeter(app.getMetrics().newMeter({"overlay", "send", "txset"}, "message")),
          mSendGetSCPQuorumSetMeter(app.getMetrics().newMeter({"overlay", "send", "get-scp-qset"}, "message")),
          mSendSCPQuorumSetMeter(app.getMetrics().newMeter({"overlay", "send", "scp-qset"}, "message")),
          mSendSCPMessageSetMeter(app.getMetrics().newMeter({"overlay", "send", "scp-message"}, "message")),
          mSendGetSCPStateMeter(app.getMetrics().newMeter({"overlay", "send", "get-scp-state"}, "message")),
          mDropInConnectHandlerMeter(app.getMetrics().newMeter({"overlay", "drop", "connect-handler"}, "drop")),
          mDropInRecvMessageDecodeMeter(app.getMetrics().newMeter({"overlay", "drop", "recv-message-decode"}, "drop")),
          mDropInRecvMessageSeqMeter(app.getMetrics().newMeter({"overlay", "drop", "recv-message-seq"}, "drop")),
          mDropInRecvMessageMacMeter(app.getMetrics().newMeter({"overlay", "drop", "recv-message-mac"}, "drop")),
          mDropInRecvMessageUnauthMeter(app.getMetrics().newMeter({"overlay", "drop", "recv-message-unauth"}, "drop")),
          mDropInRecvHelloUnexpectedMeter(
                  app.getMetrics().newMeter({"overlay", "drop", "recv-hello-unexpected"}, "drop")),
          mDropInRecvHelloVersionMeter(app.getMetrics().newMeter({"overlay", "drop", "recv-hello-version"}, "drop")),
          mDropInRecvHelloSelfMeter(app.getMetrics().newMeter({"overlay", "drop", "recv-hello-self"}, "drop")),
          mDropInRecvHelloPeerIDMeter(app.getMetrics().newMeter({"overlay", "drop", "recv-hello-peerid"}, "drop")),
          mDropInRecvHelloCertMeter(app.getMetrics().newMeter({"overlay", "drop", "recv-hello-cert"}, "drop")),
          mDropInRecvHelloBanMeter(app.getMetrics().newMeter({"overlay", "drop", "recv-hello-ban"}, "drop")),
          mDropInRecvHelloNetMeter(app.getMetrics().newMeter({"overlay", "drop", "recv-hello-net"}, "drop")),
          mDropInRecvHelloPortMeter(app.getMetrics().newMeter({"overlay", "drop", "recv-hello-port"}, "drop")),
          mDropInRecvAuthUnexpectedMeter(
                  app.getMetrics().newMeter({"overlay", "drop", "recv-auth-unexpected"}, "drop")),
          mDropInRecvAuthRejectMeter(app.getMetrics().newMeter({"overlay", "drop", "recv-auth-reject"}, "drop")),
          mDropInRecvAuthInvalidPeerMeter(
                  app.getMetrics().newMeter({"overlay", "drop", "recv-auth-invalid-peer"}, "drop")),
          mDropInRecvErrorMeter(app.getMetrics().newMeter({"overlay", "drop", "recv-error"}, "drop")) {
    auto bytes = randomBytes(mSendNonce.size());
    std::copy(bytes.begin(), bytes.end(), mSendNonce.begin());
}

void
Peer::sendHello() {
    CLOG(DEBUG, "Overlay") << "Peer::sendHello to " << toString();
    VixalMessage msg;
    msg.type(HELLO);
    Hello &elo = msg.hello();
    elo.ledgerVersion = mApp.getConfig().LEDGER_PROTOCOL_VERSION;
    elo.overlayMinVersion = mApp.getConfig().OVERLAY_PROTOCOL_MIN_VERSION;
    elo.overlayVersion = mApp.getConfig().OVERLAY_PROTOCOL_VERSION;
    elo.versionStr = mApp.getConfig().VERSION_STR;
    elo.networkID = mApp.getNetworkID();
    elo.listeningPort = mApp.getConfig().PEER_PORT;
    elo.peerID = mApp.getConfig().NODE_SEED.getPublicKey();
    elo.cert = this->getAuthCert();
    elo.nonce = mSendNonce;
    sendMessage(msg);
}

AuthCert
Peer::getAuthCert() {
    return mApp.getOverlayManager().getPeerAuth().getAuthCert();
}

size_t
Peer::getIOTimeoutSeconds() const {
    if (isAuthenticated()) {
        // Normally willing to wait 30s to hear anything from an authenticated peer.
        return mApp.getConfig().PEER_TIMEOUT;
    } else {
        // We give peers much less timing leeway while performing handshake.
        return mApp.getConfig().PEER_AUTHENTICATION_TIMEOUT;
    }
}

void
Peer::receivedBytes(size_t byteCount, bool gotFullMessage) {
    LoadManager::PeerContext loadCtx(mApp, mPeerID);
    mLastRead = mApp.getClock().now();
    if (gotFullMessage)
        mMessageRead.mark();
    mByteRead.mark(byteCount);
}

void
Peer::startIdleTimer() {
    if (shouldAbort()) {
        return;
    }

    auto self = shared_from_this();
    mIdleTimer.expires_after(std::chrono::seconds(getIOTimeoutSeconds()));
    mIdleTimer.async_wait([self](asio::error_code const &error) {
        self->idleTimerExpired(error);
    });
}

void
Peer::idleTimerExpired(asio::error_code const &error) {
    if (!error) {
        auto now = mApp.getClock().now();
        auto timeout = std::chrono::seconds(getIOTimeoutSeconds());
        if (((now - mLastRead) >= timeout) && ((now - mLastWrite) >= timeout)) {
            CLOG(WARNING, "Overlay") << "idle timeout";
            mTimeoutIdle.mark();
            drop();
        } else {
            startIdleTimer();
        }
    }
}

void
Peer::sendAuth() {
    VixalMessage msg;
    msg.type(AUTH);
    sendMessage(msg);
}

std::string
Peer::toString() {
    std::stringstream s;
    s << getIP() << ":" << mRemoteListeningPort;
    return s.str();
}

void
Peer::drop(ErrorCode err, std::string const &msg) {
    VixalMessage m;
    m.type(ERROR_MSG);
    m.error().code = err;
    m.error().msg = msg;
    sendMessage(m);
    // note: this used to be a post which caused delays in stopping
    // to process read messages.
    // this has no effect wrt the sending queue.
    drop();
}

void
Peer::connectHandler(asio::error_code const &error) {
    if (error) {
        CLOG(WARNING, "Overlay") << " connectHandler error: "
                                 << error.message();
        mDropInConnectHandlerMeter.mark();
        drop();
    } else {
        CLOG(DEBUG, "Overlay") << "connected " << toString();
        connected();
        mState = CONNECTED;
        sendHello();
    }
}

void
Peer::sendDontHave(MessageType type, uint256 const &itemID) {
    VixalMessage msg;
    msg.type(DONT_HAVE);
    msg.dontHave().reqHash = itemID;
    msg.dontHave().type = type;

    sendMessage(msg);
}

void
Peer::sendSCPQuorumSet(SCPQuorumSetPtr qSet) {
    VixalMessage msg;
    msg.type(SCP_QUORUMSET);
    msg.qSet() = *qSet;

    sendMessage(msg);
}

void
Peer::sendGetTxSet(uint256 const &setID) {
    VixalMessage newMsg;
    newMsg.type(GET_TX_SET);
    newMsg.txSetHash() = setID;

    sendMessage(newMsg);
}

void
Peer::sendGetQuorumSet(uint256 const &setID) {
    if (Logging::logTrace("Overlay"))
        CLOG(TRACE, "Overlay") << "Get quorum set: " << hexAbbrev(setID);

    VixalMessage newMsg;
    newMsg.type(GET_SCP_QUORUMSET);
    newMsg.qSetHash() = setID;

    sendMessage(newMsg);
}

void
Peer::sendGetPeers() {
    CLOG(TRACE, "Overlay") << "Get peers";

    VixalMessage newMsg;
    newMsg.type(GET_PEERS);

    sendMessage(newMsg);
}

void
Peer::sendGetScpState(uint32 ledgerSeq) {
    CLOG(TRACE, "Overlay") << "Get SCP State for " << ledgerSeq;

    VixalMessage newMsg;
    newMsg.type(GET_SCP_STATE);
    newMsg.getSCPLedgerSeq() = ledgerSeq;

    sendMessage(newMsg);
}

void
Peer::sendPeers() {
    // send top 50 peers we know about
    vector<PeerRecord> peerList;
    PeerRecord::loadPeerRecords(
            mApp.getDatabase(), 50, mApp.getClock().now(),
            [&](PeerRecord const &pr) {
                if (!pr.isPrivateAddress() && !pr.isSelfAddressAndPort(getIP(), mRemoteListeningPort)) {
                    peerList.emplace_back(pr);
                }
                return peerList.size() < 50;
            });
    VixalMessage newMsg;
    newMsg.type(PEERS);
    newMsg.peers().reserve(peerList.size());
    for (auto const &pr : peerList) {
        if (pr.isPrivateAddress() ||
            pr.isSelfAddressAndPort(getIP(), mRemoteListeningPort)) {
            continue;
        }
        PeerAddress pa;
        pr.toXdr(pa);
        newMsg.peers().push_back(pa);
    }
    sendMessage(newMsg);
}

static std::string
msgSummary(VixalMessage const &msg) {
    switch (msg.type()) {
        case ERROR_MSG:
            return "ERROR";
        case HELLO:
            return "HELLO";
        case AUTH:
            return "AUTH";
        case DONT_HAVE:
            return "DONTHAVE";
        case GET_PEERS:
            return "GETPEERS";
        case PEERS:
            return "PEERS";

        case GET_TX_SET:
            return "GETTXSET";
        case TX_SET:
            return "TXSET";

        case TRANSACTION:
            return "TRANSACTION";

        case GET_SCP_QUORUMSET:
            return "GET_SCP_QSET";
        case SCP_QUORUMSET:
            return "SCP_QSET";
        case SCP_MESSAGE:
            switch (msg.envelope().statement.pledges.type()) {
                case SCP_ST_PREPARE:
                    return "SCP::PREPARE";
                case SCP_ST_CONFIRM:
                    return "SCP::CONFIRM";
                case SCP_ST_EXTERNALIZE:
                    return "SCP::EXTERNALIZE";
                case SCP_ST_NOMINATE:
                    return "SCP::NOMINATE";
            }
        case GET_SCP_STATE:
            return "GET_SCP_STATE";
    }
    return "UNKNOWN";
}

void
Peer::sendMessage(VixalMessage const &msg) {
    if (Logging::logTrace("Overlay"))
        CLOG(TRACE, "Overlay")
                << "("
                << mApp.getConfig().toShortString(
                        mApp.getConfig().NODE_SEED.getPublicKey())
                << ") send: " << msgSummary(msg)
                << " to : " << mApp.getConfig().toShortString(mPeerID);

    switch (msg.type()) {
        case ERROR_MSG:
            mSendErrorMeter.mark();
            break;
        case HELLO:
            mSendHelloMeter.mark();
            break;
        case AUTH:
            mSendAuthMeter.mark();
            break;
        case DONT_HAVE:
            mSendDontHaveMeter.mark();
            break;
        case GET_PEERS:
            mSendGetPeersMeter.mark();
            break;
        case PEERS:
            mSendPeersMeter.mark();
            break;
        case GET_TX_SET:
            mSendGetTxSetMeter.mark();
            break;
        case TX_SET:
            mSendTxSetMeter.mark();
            break;
        case TRANSACTION:
            mSendTransactionMeter.mark();
            break;
        case GET_SCP_QUORUMSET:
            mSendGetSCPQuorumSetMeter.mark();
            break;
        case SCP_QUORUMSET:
            mSendSCPQuorumSetMeter.mark();
            break;
        case SCP_MESSAGE:
            mSendSCPMessageSetMeter.mark();
            break;
        case GET_SCP_STATE:
            mSendGetSCPStateMeter.mark();
            break;
    };

    AuthenticatedMessage amsg;
    amsg.v0().message = msg;
    if (msg.type() != HELLO && msg.type() != ERROR_MSG) {
        amsg.v0().sequence = mSendMacSeq;
        amsg.v0().mac = hmacSha256(mSendMacKey, xdr::xdr_to_opaque(mSendMacSeq, msg));
        ++mSendMacSeq;
    }
    xdr::msg_ptr xdrBytes(xdr::xdr_to_msg(amsg));
    this->sendMessage(std::move(xdrBytes));
}

void
Peer::recvMessage(xdr::msg_ptr const &msg) {
    if (shouldAbort()) {
        return;
    }

    LoadManager::PeerContext loadCtx(mApp, mPeerID);

    CLOG(TRACE, "Overlay") << "received xdr::msg_ptr";
    try {
        AuthenticatedMessage am;
        xdr::xdr_from_msg(msg, am);
        recvMessage(am);
    }
    catch (xdr::xdr_runtime_error &e) {
        CLOG(ERROR, "Overlay") << "received corrupt xdr::msg_ptr " << e.what();
        mDropInRecvMessageDecodeMeter.mark();
        drop();
        return;
    }
}

bool
Peer::isConnected() const {
    return mState != CONNECTING && mState != CLOSING;
}

bool
Peer::isAuthenticated() const {
    return mState == GOT_AUTH;
}

bool
Peer::shouldAbort() const {
    return (mState == CLOSING) || mApp.getOverlayManager().isShuttingDown();
}

void
Peer::recvMessage(AuthenticatedMessage const &msg) {
    if (shouldAbort()) {
        return;
    }

    if (mState >= GOT_HELLO && msg.v0().message.type() != ERROR_MSG) {
        if (msg.v0().sequence != mRecvMacSeq) {
            CLOG(ERROR, "Overlay") << "Unexpected message-auth sequence";
            mDropInRecvMessageSeqMeter.mark();
            ++mRecvMacSeq;
            drop(ERR_AUTH, "unexpected auth sequence");
            return;
        }

        if (!hmacSha256Verify(msg.v0().mac, mRecvMacKey, xdr::xdr_to_opaque(msg.v0().sequence, msg.v0().message))) {
            CLOG(ERROR, "Overlay") << "Message-auth check failed";
            mDropInRecvMessageMacMeter.mark();
            ++mRecvMacSeq;
            drop(ERR_AUTH, "unexpected MAC");
            return;
        }
        ++mRecvMacSeq;
    }
    recvMessage(msg.v0().message);
}

void
Peer::recvMessage(VixalMessage const &vixalMsg) {
    if (shouldAbort()) {
        return;
    }

    if (Logging::logTrace("Overlay"))
        CLOG(TRACE, "Overlay")
                << "(" << mApp.getConfig().toShortString(mApp.getConfig().NODE_SEED.getPublicKey()) << ") recv: "
                << msgSummary(vixalMsg) << " from:" << mApp.getConfig().toShortString(mPeerID);

    if (!isAuthenticated() && (vixalMsg.type() != HELLO) &&
        (vixalMsg.type() != AUTH) && (vixalMsg.type() != ERROR_MSG)) {
        CLOG(WARNING, "Overlay") << "recv: " << vixalMsg.type() << " before completed handshake";
        mDropInRecvMessageUnauthMeter.mark();
        drop();
        return;
    }

    assert(isAuthenticated() || vixalMsg.type() == HELLO ||
           vixalMsg.type() == AUTH || vixalMsg.type() == ERROR_MSG);

    switch (vixalMsg.type()) {
        case ERROR_MSG: {
            auto t = mRecvErrorTimer.timeScope();
            recvError(vixalMsg);
        }
            break;

        case HELLO: {
            auto t = mRecvHelloTimer.timeScope();
            this->recvHello(vixalMsg.hello());
        }
            break;

        case AUTH: {
            auto t = mRecvAuthTimer.timeScope();
            this->recvAuth(vixalMsg);
        }
            break;

        case DONT_HAVE: {
            auto t = mRecvDontHaveTimer.timeScope();
            recvDontHave(vixalMsg);
        }
            break;

        case GET_PEERS: {
            auto t = mRecvGetPeersTimer.timeScope();
            recvGetPeers(vixalMsg);
        }
            break;

        case PEERS: {
            auto t = mRecvPeersTimer.timeScope();
            recvPeers(vixalMsg);
        }
            break;

        case GET_TX_SET: {
            auto t = mRecvGetTxSetTimer.timeScope();
            recvGetTxSet(vixalMsg);
        }
            break;

        case TX_SET: {
            auto t = mRecvTxSetTimer.timeScope();
            recvTxSet(vixalMsg);
        }
            break;

        case TRANSACTION: {
            auto t = mRecvTransactionTimer.timeScope();
            recvTransaction(vixalMsg);
        }
            break;

        case GET_SCP_QUORUMSET: {
            auto t = mRecvGetSCPQuorumSetTimer.timeScope();
            recvGetSCPQuorumSet(vixalMsg);
        }
            break;

        case SCP_QUORUMSET: {
            auto t = mRecvSCPQuorumSetTimer.timeScope();
            recvSCPQuorumSet(vixalMsg);
        }
            break;

        case SCP_MESSAGE: {
            auto t = mRecvSCPMessageTimer.timeScope();
            recvSCPMessage(vixalMsg);
        }
            break;

        case GET_SCP_STATE: {
            auto t = mRecvGetSCPStateTimer.timeScope();
            recvGetSCPState(vixalMsg);
        }
            break;
    }
}

void
Peer::recvDontHave(VixalMessage const &msg) {
    mApp.getHerder().peerDoesntHave(msg.dontHave().type, msg.dontHave().reqHash, shared_from_this());
}

void
Peer::recvGetTxSet(VixalMessage const &msg) {
    auto self = shared_from_this();
    if (auto txSet = mApp.getHerder().getTxSet(msg.txSetHash())) {
        VixalMessage newMsg;
        newMsg.type(TX_SET);
        txSet->toXDR(newMsg.txSet());

        self->sendMessage(newMsg);
    } else {
        sendDontHave(TX_SET, msg.txSetHash());
    }
}

void
Peer::recvTxSet(VixalMessage const &msg) {
    TxSetFrame frame(mApp.getNetworkID(), msg.txSet());
    mApp.getHerder().recvTxSet(frame.getContentsHash(), frame);
}

void
Peer::recvTransaction(VixalMessage const &msg) {
    TransactionFramePtr transaction = TransactionFrame::makeTransactionFromWire(
            mApp.getNetworkID(), msg.transaction());
    if (transaction) {
        // add it to our current set
        // and make sure it is valid
        auto recvRes = mApp.getHerder().recvTransaction(transaction);

        if (recvRes == Herder::TX_STATUS_PENDING ||
            recvRes == Herder::TX_STATUS_DUPLICATE) {
            // record that this peer sent us this transaction
            mApp.getOverlayManager().recvFloodedMsg(msg, shared_from_this());

            if (recvRes == Herder::TX_STATUS_PENDING) {
                // if it's a new transaction, broadcast it
                mApp.getOverlayManager().broadcastMessage(msg, false);
            }
        }
    }
}

void
Peer::recvGetSCPQuorumSet(VixalMessage const &msg) {
    SCPQuorumSetPtr qset = mApp.getHerder().getQSet(msg.qSetHash());

    if (qset) {
        sendSCPQuorumSet(qset);
    } else {
        if (Logging::logTrace("Overlay"))
            CLOG(TRACE, "Overlay") << "No quorum set: "
                                   << hexAbbrev(msg.qSetHash());
        sendDontHave(SCP_QUORUMSET, msg.qSetHash());
        // do we want to ask other people for it?
    }
}

void
Peer::recvSCPQuorumSet(VixalMessage const &msg) {
    Hash hash = sha256(xdr::xdr_to_opaque(msg.qSet()));
    mApp.getHerder().recvSCPQuorumSet(hash, msg.qSet());
}

void
Peer::recvSCPMessage(VixalMessage const &msg) {
    SCPEnvelope const &envelope = msg.envelope();
    if (Logging::logTrace("Overlay"))
        CLOG(TRACE, "Overlay")
                << "recvSCPMessage node: "
                << mApp.getConfig().toShortString(msg.envelope().statement.nodeID);

    mApp.getOverlayManager().recvFloodedMsg(msg, shared_from_this());

    auto type = msg.envelope().statement.pledges.type();
    auto t = (type == SCP_ST_PREPARE
              ? mRecvSCPPrepareTimer.timeScope()
              : (type == SCP_ST_CONFIRM
                 ? mRecvSCPConfirmTimer.timeScope()
                 : (type == SCP_ST_EXTERNALIZE
                    ? mRecvSCPExternalizeTimer.timeScope()
                    : (mRecvSCPNominateTimer.timeScope()))));

    mApp.getHerder().recvSCPEnvelope(envelope);
}

void
Peer::recvGetSCPState(VixalMessage const &msg) {
    uint32 seq = msg.getSCPLedgerSeq();
    CLOG(TRACE, "Overlay") << "get SCP State " << seq;
    mApp.getHerder().sendSCPStateToPeer(seq, shared_from_this());
}

void
Peer::recvError(VixalMessage const &msg) {
    std::string codeStr = "UNKNOWN";
    switch (msg.error().code) {
        case ERR_MISC:
            codeStr = "ERR_MISC";
            break;
        case ERR_DATA:
            codeStr = "ERR_DATA";
            break;
        case ERR_CONF:
            codeStr = "ERR_CONF";
            break;
        case ERR_AUTH:
            codeStr = "ERR_AUTH";
            break;
        case ERR_LOAD:
            codeStr = "ERR_LOAD";
            break;
        default:
            break;
    }
    CLOG(WARNING, "Overlay") << "Received error (" << codeStr
                             << "): " << msg.error().msg;
    mDropInRecvErrorMeter.mark();
    drop();
}

void
Peer::noteHandshakeSuccessInPeerRecord() {
    if (getIP().empty() || getRemoteListeningPort() == 0) {
        CLOG(ERROR, "Overlay") << "unable to handshake with " << getIP() << ":"
                               << getRemoteListeningPort();
        mDropInRecvAuthInvalidPeerMeter.mark();
        drop();
        return;
    }

    auto pr = PeerRecord::loadPeerRecord(mApp.getDatabase(), getIP(),
                                         getRemoteListeningPort());
    if (pr) {
        pr->resetBackOff(mApp.getClock(), mApp.getOverlayManager().isPreferred(this));
    } else {
        pr = make_optional<PeerRecord>(getIP(), mRemoteListeningPort,
                                       mApp.getClock().now());
    }
    CLOG(INFO, "Overlay") << "successful handshake with "
                          << mApp.getConfig().toShortString(mPeerID) << "@"
                          << pr->toString();
    pr->storePeerRecord(mApp.getDatabase());
}

void
Peer::recvHello(Hello const &elo) {
    using xdr::operator==;

    if (mState >= GOT_HELLO) {
        CLOG(ERROR, "Overlay") << "received unexpected HELLO";
        mDropInRecvHelloUnexpectedMeter.mark();
        drop();
        return;
    }

    auto &peerAuth = mApp.getOverlayManager().getPeerAuth();
    if (!peerAuth.verifyRemoteAuthCert(elo.peerID, elo.cert)) {
        CLOG(ERROR, "Overlay") << "failed to verify remote peer auth cert";
        mDropInRecvHelloCertMeter.mark();
        drop();
        return;
    }

    if (mApp.getBanManager().isBanned(elo.peerID)) {
        CLOG(ERROR, "Overlay") << "Node is banned";
        mDropInRecvHelloBanMeter.mark();
        drop();
        return;
    }

    mRemoteListeningPort = static_cast<unsigned short>(elo.listeningPort);
    mRemoteOverlayMinVersion = elo.overlayMinVersion;
    mRemoteOverlayVersion = elo.overlayVersion;
    mRemoteVersion = elo.versionStr;
    mPeerID = elo.peerID;
    mRecvNonce = elo.nonce;
    mSendMacSeq = 0;
    mRecvMacSeq = 0;
    mSendMacKey = peerAuth.getSendingMacKey(elo.cert.pubkey, mSendNonce,
                                            mRecvNonce, mRole);
    mRecvMacKey = peerAuth.getReceivingMacKey(elo.cert.pubkey, mSendNonce,
                                              mRecvNonce, mRole);

    mState = GOT_HELLO;
    CLOG(DEBUG, "Overlay") << "recvHello from " << toString();

    if (mRole == REMOTE_CALLED_US) {
        // Send a HELLO back, even if it's going to be followed
        // immediately by ERROR, because ERROR is an authenticated
        // message type and the caller won't decode it right if
        // still waiting for an unauthenticated HELLO.
        sendHello();
    }

    if (mRemoteOverlayMinVersion > mRemoteOverlayVersion ||
        mRemoteOverlayVersion < mApp.getConfig().OVERLAY_PROTOCOL_MIN_VERSION ||
        mRemoteOverlayMinVersion > mApp.getConfig().OVERLAY_PROTOCOL_VERSION) {
        CLOG(ERROR, "Overlay") << "connection from peer with incompatible "
                "overlay protocol version";
        CLOG(DEBUG, "Overlay")
                << "Protocol = [" << mRemoteOverlayMinVersion << ","
                << mRemoteOverlayVersion << "] expected: ["
                << mApp.getConfig().OVERLAY_PROTOCOL_VERSION << ","
                << mApp.getConfig().OVERLAY_PROTOCOL_VERSION << "]";
        mDropInRecvHelloVersionMeter.mark();
        drop(ERR_CONF, "wrong protocol version");
        return;
    }

    if (elo.peerID == mApp.getConfig().NODE_SEED.getPublicKey()) {
        CLOG(WARNING, "Overlay") << "connecting to self";
        mDropInRecvHelloSelfMeter.mark();
        drop(ERR_CONF, "connecting to self");
        return;
    }

    if (elo.networkID != mApp.getNetworkID()) {
        CLOG(WARNING, "Overlay")
                << "connection from peer with different NetworkID";
        CLOG(DEBUG, "Overlay")
                << "NetworkID = " << hexAbbrev(elo.networkID)
                << " expected: " << hexAbbrev(mApp.getNetworkID());
        mDropInRecvHelloNetMeter.mark();
        drop(ERR_CONF, "wrong network passphrase");
        return;
    }

    auto const &authenticated =
            mApp.getOverlayManager().getAuthenticatedPeers();
    auto authenticatedIt = authenticated.find(mPeerID);
    // no need to self-check here as this one cannot be in authenticated yet
    if (authenticatedIt != std::end(authenticated)) {
        if (&(authenticatedIt->second->mPeerID) != &mPeerID) {
            CLOG(WARNING, "Overlay")
                    << "connection from already-connected peerID "
                    << mApp.getConfig().toShortString(mPeerID);
            mDropInRecvHelloPeerIDMeter.mark();
            drop(ERR_CONF, "connecting already-connected peer");
            return;
        }
    }
    for (auto const &p : mApp.getOverlayManager().getPendingPeers()) {
        if (&(p->mPeerID) == &mPeerID) {
            continue;
        }
        if (p->getPeerID() == mPeerID) {
            CLOG(WARNING, "Overlay")
                    << "connection from already-connected peerID "
                    << mApp.getConfig().toShortString(mPeerID);
            mDropInRecvHelloPeerIDMeter.mark();
            drop(ERR_CONF, "connecting already-connected peer");
            return;
        }
    }

    if (elo.listeningPort <= 0 || elo.listeningPort > UINT16_MAX) {
        CLOG(WARNING, "Overlay") << "bad port in recvHello";
        mDropInRecvHelloPortMeter.mark();
        drop(ERR_CONF, "bad port number");
        return;
    }

    if (mRole == WE_CALLED_REMOTE) {
        sendAuth();
    }
}

void
Peer::recvAuth(VixalMessage const &msg) {
    if (mState != GOT_HELLO) {
        CLOG(INFO, "Overlay") << "Unexpected AUTH message before HELLO";
        mDropInRecvAuthUnexpectedMeter.mark();
        drop(ERR_MISC, "out-of-order AUTH message");
        return;
    }

    if (isAuthenticated()) {
        CLOG(INFO, "Overlay") << "Unexpected AUTH message";
        mDropInRecvAuthUnexpectedMeter.mark();
        drop(ERR_MISC, "out-of-order AUTH message");
        return;
    }

    mState = GOT_AUTH;

    auto self = shared_from_this();

    if (!mApp.getOverlayManager().acceptAuthenticatedPeer(self)) {
        CLOG(WARNING, "Overlay") << "New peer rejected, all slots taken";
        mDropInRecvAuthRejectMeter.mark();
        drop(ERR_LOAD, "peer rejected");
        return;
    }

    noteHandshakeSuccessInPeerRecord();

    if (mRole == REMOTE_CALLED_US) {
        sendAuth();
        sendPeers();
    }

    // send SCP State
    // remove when all known peers implements the next line
    mApp.getHerder().sendSCPStateToPeer(0, self);
    // ask for SCP state if not synced
    sendGetScpState(mApp.getLedgerManager().getLastClosedLedgerNum() + 1);
}

void
Peer::recvGetPeers(VixalMessage const &msg) {
    sendPeers();
}

void
Peer::recvPeers(VixalMessage const &msg) {
    const uint32 NEW_PEER_WINDOW_SECONDS = 10;

    for (auto const &peer : msg.peers()) {
        if (peer.port == 0 || peer.port > UINT16_MAX) {
            CLOG(WARNING, "Overlay") << "ignoring received peer with bad port "
                                     << peer.port;
            continue;
        }
        if (peer.ip.type() == IPv6) {
            CLOG(WARNING, "Overlay") << "ignoring received IPv6 address"
                                     << " (not yet supported)";
            continue;
        }
        // randomize when we'll try to connect to this peer next if we don't know it
        auto defaultNextAttempt = mApp.getClock().now() +
                                  std::chrono::seconds(random_t::get<uint32>(0, NEW_PEER_WINDOW_SECONDS));

        stringstream ip;
        ip << (int) peer.ip.ipv4()[0] << "." << (int) peer.ip.ipv4()[1] << "."
           << (int) peer.ip.ipv4()[2] << "." << (int) peer.ip.ipv4()[3];
        // don't use peer.numFailures here as we may have better luck
        // (and we don't want to poison our failure count)
        PeerRecord pr{ip.str(), static_cast<unsigned short>(peer.port),
                      defaultNextAttempt, 0};

        if (pr.isPrivateAddress()) {
            CLOG(WARNING, "Overlay") << "ignoring received private address "
                                     << pr.toString();
        } else if (pr.isSelfAddressAndPort(getIP(), mApp.getConfig().PEER_PORT)) {
            CLOG(WARNING, "Overlay") << "ignoring received self-address "
                                     << pr.toString();
        } else if (pr.isLocalhost() &&
                   !mApp.getConfig().ALLOW_LOCALHOST_FOR_TESTING) {
            CLOG(WARNING, "Overlay") << "ignoring received localhost";
        } else {
            pr.insertIfNew(mApp.getDatabase());
        }
    }
}
}
