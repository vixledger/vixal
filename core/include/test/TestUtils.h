#pragma once

// Copyright 2016 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include <ctime>
#include "invariant/InvariantDoesNotHold.h"
#include "../../src/application/ApplicationImpl.h"
#include "../../src/invariant/InvariantManagerImpl.h"
#include <type_traits>


namespace vixal {

class LoopbackPeerConnection;

namespace testutil {

void setCurrentLedgerVersion(LedgerManager &lm, uint32_t currentLedgerVersion);

void crankSome(VirtualClock &clock);

void injectSendPeersAndReschedule(VirtualClock::time_point &end,
                                  VirtualClock &clock, VirtualTimer &timer,
                                  LoopbackPeerConnection &connection);

class BucketListDepthModifier {
    uint32_t const mPrevDepth;

public:
    explicit BucketListDepthModifier(uint32_t newDepth);

    ~BucketListDepthModifier();
};
}

class TestInvariantManager : public InvariantManagerImpl {
public:
    explicit TestInvariantManager(medida::MetricsRegistry &registry);

private:
    void
    handleInvariantFailure(std::shared_ptr<Invariant> invariant,
                           std::string const &message) const override;
};

class TestApplication : public ApplicationImpl {
public:
    TestApplication(VirtualClock &clock, Config const &cfg);

private:
    std::unique_ptr<InvariantManager> createInvariantManager() override;
};


template<typename T = TestApplication,
        typename = typename std::enable_if<
                std::is_base_of<TestApplication, T>::value>::type>
std::shared_ptr<T>
createTestApplication(VirtualClock &clock, Config const &cfg) {
    Config c2(cfg);
    c2.USE_CONFIG_FOR_GENESIS = true;
    auto app = Application::create<T>(clock, c2);
    return app;
}

time_t getTestDate(int day, int month, int year);

std::tm getTestDateTime(int day, int month, int year, int hour, int minute, int second);

VirtualClock::time_point genesis(int minute, int second);

}

