#ifndef WORLDTEST_H
#define WORLDTEST_H

#include "tests/testhelper.h"
#include "child/world.h"

namespace Child {
    class WorldTest : public QObject
    {
        Q_OBJECT
    private:
        long long int _initialModuleCount;
    private slots:
        void init() {
            _initialModuleCount = Module::moduleCount();
        }

        void cleanup() {
            QVERIFY(Module::moduleCount() == _initialModuleCount);
        }

        void new_();
    };
}

#endif // WORLDTEST_H
