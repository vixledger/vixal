#pragma once

// Copyright 2014 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include <application/Config.h>
#include "test/TestPrinter.h"
#include "util/Logging.h"

namespace vixal {

int test(int argc, char *argv[], el::Level logLevel,
         std::vector<std::string> const &metrics);

extern bool force_sqlite;

class Application;

void for_versions_to(uint32 to, Application &app, std::function<void(void)> const &f);

void for_versions_from(uint32 from, Application &app, std::function<void(void)> const &f);

void for_versions_from(std::vector<uint32> const &versions, Application &app, std::function<void(void)> const &f);

void for_all_versions(Application &app, std::function<void(void)> const &f);

void for_versions(uint32 from, uint32 to, Application &app, std::function<void(void)> const &f);

void for_versions(std::vector<uint32> const &versions, Application &app, std::function<void(void)> const &f);

void for_all_versions_except(std::vector<uint32> const &versions, Application &app, std::function<void(void)> const &f);
}
