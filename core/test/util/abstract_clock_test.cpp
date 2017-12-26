#include <thread>
#include "catch.hpp"
#include <chrono>

#include "util/abstract_clock.hpp"
#include "util/manual_clock.hpp"
#include "util/Logging.h"

using namespace vixal;

using namespace std::chrono_literals;

template<typename Clock>
void test(abstract_clock<Clock> &c) {

    auto const t1(c.now());
    std::this_thread::sleep_for(500ms);
    auto const t2(c.now());

    LOG(DEBUG) << "t1= " << t1.time_since_epoch().count()
               << ", t2= " << t2.time_since_epoch().count()
               << ", elapsed= " << (t2 - t1).count();

    CHECK(((t2 - t1) >= 500ms));
}

void test_manual() {
    using clock_type = manual_clock<std::chrono::steady_clock>;
    clock_type c;

    std::stringstream ss;

    auto c1 = c.now().time_since_epoch();
    c.set(clock_type::time_point(std::chrono::seconds(1)));
    auto c2 = c.now().time_since_epoch();
    c.set(clock_type::time_point(std::chrono::seconds(2)));
    auto c3 = c.now().time_since_epoch();
    ++c;
    auto c4 = c.now().time_since_epoch();

    LOG(DEBUG) << "[" << c1.count() << "," << c2.count() << "," << c3.count() << "," << c4.count() << "]";
    CHECK(c4 == 3s);

    c.advance(1500ms);
    auto c5 = c.now().time_since_epoch();

    CHECK(c5 == 3s + 1500ms);

}


TEST_CASE("abstract_clock", "[clock]") {
    test(get_abstract_clock<std::chrono::steady_clock>());
    test(get_abstract_clock<std::chrono::system_clock>());
    test(get_abstract_clock<std::chrono::high_resolution_clock>());
    test_manual();
}
