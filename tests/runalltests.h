#ifndef TEST_H
#define TEST_H

#include "tests/testhelper.h"
#include "tests/moduletest.h"
#include "tests/objecttest.h"
#include "tests/worldtest.h"
#include "tests/texttest.h"

namespace Child {
    static int testFailureCount;

    static void beginTests() {
        testFailureCount = 0;
        p("=================== Running all tests ===================");
    }

    static void test(QObject *testClass) {
        testFailureCount += QTest::qExec(testClass);
    }

    static void endTests() {
        if(testFailureCount == 0) {
            p("==================== All tests passed ===================");
        } else {
            p(QString("==================== %1 test(s) failed ===================").arg(testFailureCount));
        }
    }

    static void runAllTests() {
        beginTests();
        test(new ModuleTest);
        test(new ObjectTest);
        test(new WorldTest);
        test(new TextTest);
        endTests();
    }
}

#endif // TEST_H
