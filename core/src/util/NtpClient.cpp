// Copyright 2016 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "util/NtpClient.h"

#include "util/Logging.h"

#include <array>

namespace vixal {

NtpClient::NtpClient(asio::io_context &ioContext, std::string server,
                     std::function<void(long)> successCallback,
                     std::function<void()> failureCallback)
        : mIoContext(ioContext), mServer(std::move(server)), mSuccessCallback(std::move(successCallback)),
          mFailureCallback(std::move(failureCallback)), mResolver(ioContext), mCallbackCalled(false) {
}

NtpClient::~NtpClient() {
    failure(); // we can do that now because failure() prevents from calling
    // a callback method second time
}

void
NtpClient::getTime() {
    std::weak_ptr<NtpClient> weak =
            std::static_pointer_cast<NtpClient>(shared_from_this());
    mResolver.async_resolve(
            mServer, "ntp",
            [weak](asio::error_code ec, asio::ip::udp::resolver::results_type res) {
                auto self = weak.lock();
                if (self) {
                    self->onServerResolved(ec, res);
                }
            });
}

void
NtpClient::success(long time) {
    if (!mCallbackCalled) {
        mCallbackCalled = true;
        mSuccessCallback(time);
    }
}

void
NtpClient::failure() {
    if (!mCallbackCalled) {
        mCallbackCalled = true;
        mFailureCallback();
    }
}

void
NtpClient::onServerResolved(asio::error_code ec,
                            asio::ip::udp::resolver::results_type res) {
    if (ec || res.empty()) {
        failure();
        return;
    }

    mSocket = std::make_unique<asio::ip::udp::socket>(mIoContext);

    std::weak_ptr<NtpClient> weak =
            std::static_pointer_cast<NtpClient>(shared_from_this());
    mSocket->async_connect(res.begin()->endpoint(), [weak](asio::error_code ec) {
        auto self = weak.lock();
        if (self) {
            self->onConnectFinished(ec);
        }
    });
}

void
NtpClient::onConnectFinished(asio::error_code ec) {
    if (ec) {
        failure();
        return;
    }

    const auto SEND_PACKET = packet<char>{010};
    std::weak_ptr<NtpClient> weak =
            std::static_pointer_cast<NtpClient>(shared_from_this());
    mSocket->async_send(asio::buffer(SEND_PACKET),
                        [weak](asio::error_code ec, std::size_t) {
                            auto self = weak.lock();
                            if (self) {
                                self->onWriteFinished(ec);
                            }
                        });
}

void
NtpClient::onWriteFinished(asio::error_code ec) {
    if (ec) {
        failure();
        return;
    }

    std::weak_ptr<NtpClient> weak =
            std::static_pointer_cast<NtpClient>(shared_from_this());
    mSocket->async_receive(asio::buffer(mReadBuffer),
                           [weak](asio::error_code ec, std::size_t) {
                               auto self = weak.lock();
                               if (self) {
                                   self->onReadFinished(ec);
                               }
                           });
}

void
NtpClient::onReadFinished(asio::error_code ec) {
    if (ec) {
        failure();
        return;
    }

    // 2208988800U is difference between first second of NTP time and first
    // second of unix epoch
    success(ntohl((time_t) mReadBuffer[4]) - 2208988800U);
}
}
