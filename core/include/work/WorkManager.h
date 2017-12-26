#pragma once

// Copyright 2015 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include <string>
#include "work/AbstractWork.h"
#include <application/Application.h>

namespace vixal {

class Application;

/**
 * WorkManager is a collection of trees of Work, each of which describes
 * dependencies between asynchronous or long-running activities that each
 * might soft-fail and require retrying, or require breaking up into pieces
 * to avoid monopolizing the main thread for too long.
 */
class WorkManager : public AbstractWork {
public:
    explicit WorkManager(Application &app);

    virtual ~WorkManager();

    static std::shared_ptr<WorkManager> create(Application &app);

    void notify(std::string const &changed) override = 0;

    template <typename T, typename... Args>
    std::shared_ptr<T>
    executeWork(bool block, Args&&... args) {
        auto work = addWork<T>(std::forward<Args>(args)...);
        auto& clock = mApp.getClock();
        advanceChildren();
        while (!clock.io_context().stopped() && !allChildrenDone()) {
            clock.crank(block);
        }
        return work;
    }
};
}
