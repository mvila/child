#ifndef CHILD_LANGUAGE_TEST_H
#define CHILD_LANGUAGE_TEST_H

#include "node/object/element.h"
#include "node/object/language/section.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_TEST(ARGS...) \
    new Language::Test(context()->child("Object", "Language", "Test"), ##ARGS)

    class Test : public GenericNodeElement<Section> {
        CHILD_DECLARE(Test, Element, Language);
    public:
        explicit Test(Node *origin, Section *section = NULL, Node *receiver = NULL) :
            GenericNodeElement<Section>(origin), _receiver(receiver) { setSection(section); }

        CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Test);
        CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Test, CHILD_FORK_IF_NOT_NULL(section()), CHILD_FORK_IF_NOT_NULL(receiver()));

        // aliases...
        Section *section() const { return value(); }
        void setSection(Section *section) { setValue(section); }

        Node *receiver() const { return _receiver; }
        void setReceiver(Node *receiver) { _receiver = receiver; }

        virtual Node *run(Node *receiver = context()) {
            Q_UNUSED(receiver);
            CHILD_PUSH_RUN(this);
            CHILD_PUSH_CONTEXT(this->receiver());
            return section()->run();
        }

        virtual QString toString(bool debug = false, short level = 0) const {
            return section() ? section()->toString(debug, level) : "NULL";
        }
    private:
        Node *_receiver;
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_TEST_H
