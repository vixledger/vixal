#include  <util/Logging.h>
#include <sodium.h>

#define CATCH_CONFIG_RUNNER

#include <catch.hpp>

#if !defined(AUTO_INITIALIZE_EASYLOGGINGPP)
INITIALIZE_EASYLOGGINGPP
#endif

#include "util/SimpleTestReporter.hpp"

using namespace vixal;

int main(int argc, char *argv[]) {
    std::vector<std::string> metrics;

    // global setup...
    Logging::init();
    Logging::setFmt("<test>");
    Logging::setLogLevel(el::Level::Error, nullptr);
    if (sodium_init() != 0) {
        LOG(FATAL) << "Could not initialize crypto";
        return 1;
    }

    Catch::Session session{};
    auto r = session.applyCommandLine(argc, argv);
    if (r != 0) {
        return r;
    }
    r = session.run();

    // global clean-up...

    return r;
}



