// Copyright 2015 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "work/Work.h"

#include "util/format.h"
#include "util/Logging.h"

#include "medida/meter.h"
#include "medida/metrics_registry.h"
#include <application/Application.h>

#include "WorkManagerImpl.h"

namespace vixal {

WorkManager::WorkManager(Application &app) : AbstractWork(app) {
}

WorkManager::~WorkManager() = default;

WorkManagerImpl::WorkManagerImpl(Application &app) : WorkManager(app) {
}

WorkManagerImpl::~WorkManagerImpl() = default;

void
WorkManagerImpl::notify(std::string const &child) {
    auto i = mChildren.find(child);
    if (i == mChildren.end()) {
        CLOG(WARNING, "Work") << "WorkManager notified by unknown child " << child;
        return;
    }

    if (i->second->getState() == Work::WORK_SUCCESS) {
        CLOG(INFO, "Work") << "WorkManager got SUCCESS from " << child;
        mApp.getMetrics().newMeter({"work", "root", "success"}, "unit").mark();
        mChildren.erase(child);
    } else if (i->second->getState() == Work::WORK_FAILURE_RAISE) {
        CLOG(WARNING, "Work") << "WorkManager got FAILURE_RAISE from " << child;
        mApp.getMetrics().newMeter({"work", "root", "failure"}, "unit").mark();
        mChildren.erase(child);
    } else if (i->second->getState() == Work::WORK_FAILURE_FATAL) {
        CLOG(WARNING, "Work") << "WorkManager got FAILURE_FATAL from " << child;
        mApp.getMetrics().newMeter({"work", "root", "failure"}, "unit").mark();
        mChildren.erase(child);
    }
    advanceChildren();
}

std::shared_ptr<WorkManager>
WorkManager::create(Application &app) {
    return std::make_shared<WorkManagerImpl>(app);
}
}
