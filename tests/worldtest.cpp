#include "tests/worldtest.h"

namespace Child {
    void WorldTest::new_() {
        World w; World *world = &w;
        QVERIFY(world->isBasedOn(Module::root()->child("Object")));
        QVERIFY(world->hasChild("NativeMethod"));
    }
}
