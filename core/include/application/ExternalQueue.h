#pragma once

// Copyright 2015 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "Application.h"
#include "xdr/types.h"
#include <string>

namespace vixal {

class ExternalQueue {
public:
    explicit ExternalQueue(Application &app);

    static void dropAll(Database &db);

    // checks if a given resource ID is well formed
    static bool validateResourceID(std::string const &resid);

    // sets the cursor of a given resource
    void setCursorForResource(std::string const &resid, uint32 cursor);

    // deletes the subscription for the resource
    void deleteCursor(std::string const &resid);

    // safely delete data
    void process();

private:
    void checkID(std::string const &resid);

    std::string getCursor(std::string const &resid);

    static const std::string kSQLCreateStatement;

    Application &mApp;
};
}
