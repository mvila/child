#ifndef CHILD_APPLICATION_H
#define CHILD_APPLICATION_H

#include "node/object.h"

CHILD_BEGIN

#define CHILD_APPLICATION(ARGS...) \
new Application(context()->child("Object", "Application"), ##ARGS)

class Application : public Object {
    CHILD_DECLARE(Application, Object, Object);
public:
    explicit Application(Node *origin) : Object(origin) {}

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Application);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Application);
};

CHILD_END

#endif // CHILD_APPLICATION_H
