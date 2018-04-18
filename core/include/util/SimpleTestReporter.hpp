#pragma once

#include <catch.hpp>

namespace Catch {

struct SimpleTestReporter : public ConsoleReporter {

    SimpleTestReporter(ReporterConfig const &_config)
            : ConsoleReporter(_config) {

    }

    ~SimpleTestReporter() = default;

    static std::string
    getDescription() {
        return "Reports minimal information on tests";
    }

    ReporterPreferences
    getPreferences() const override {
        ReporterPreferences prefs;
        prefs.shouldRedirectStdOut = false;
        return prefs;
    }

    void
    testCaseStarting(TestCaseInfo const &_testInfo) override {
        ConsoleReporter::testCaseStarting(_testInfo);
        stream << "\"" << _testInfo.name << "\" " << _testInfo.lineInfo << std::endl;
    }

    void
    sectionStarting(SectionInfo const &_sectionInfo) override {
        printDot();
        ConsoleReporter::sectionStarting(_sectionInfo);
    }

    void
    assertionStarting(AssertionInfo const &ai) override {
        mLastAssertInfo = std::make_unique<AssertionInfo>(ai);
    }

    bool
    assertionEnded(AssertionStats const &_assertionStats) override {
        AssertionResult const &result = _assertionStats.assertionResult;

        if (result.isOk()) {
            return true;
        }
        ConsoleReporter::assertionStarting(*mLastAssertInfo);
        return ConsoleReporter::assertionEnded(_assertionStats);
    }

    void
    testCaseEnded(TestCaseStats const &) override {
        stream << "<done>";
        printNewLine();
    }


private:
    int mDots{0};

    std::unique_ptr<AssertionInfo> mLastAssertInfo;

    void printDot() {
        stream << '.' << std::flush;
        mDots++;
        if (mDots == 40) {
            printNewLine();
        }
    }

    void printNewLine() {
        stream << '\n';
        mDots = 0;
    }


};

CATCH_REGISTER_REPORTER("simple", SimpleTestReporter)

}

