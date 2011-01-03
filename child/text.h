#ifndef CHILD_TEXT_H
#define CHILD_TEXT_H

#include "child/element.h"
#include "child/boolean.h"
#include "child/number.h"
#include "child/message.h"

CHILD_BEGIN

#define CHILD_TEXT(ARGS...) new Text(Node::context()->child("Object", "Text"), ##ARGS)

class Text : public GenericElement<QString> {
    CHILD_DECLARE(Text, Element);
public:
    explicit Text(Node *origin, const QString &value = "") : GenericElement<QString>(origin, value) {}

//    static Node *defaultOrigin() { return hasContext() ? context()->child(classPath()) : rootOrigin(); }

//    static Node *rootOrigin() { return Element::root(); }
//    static Node *rootParent() { return Object::root(); }

//    static const QString &classPath() {} // déterminé à partir de rootParent() et className()

    static void initRoot() {
        Object::root()->addChild("Text", root());
        CHILD_NATIVE_METHOD_ADD(Text, init);
        CHILD_NATIVE_METHOD_ADD(Text, concatenate, +);
        CHILD_NATIVE_METHOD_ADD(Text, multiply, *);
        CHILD_NATIVE_METHOD_ADD(Text, upcase);
        CHILD_NATIVE_METHOD_ADD(Text, equal_to, ==);
        CHILD_NATIVE_METHOD_ADD(Text, compare, <=>);
    }

    CHILD_FORK_METHOD(Text, value());

    CHILD_NATIVE_METHOD_DECLARE(init) {
        CHILD_CHECK_INPUT_SIZE(0, 1);
        if(message->hasInput(0)) setValue(message->runFirstInput()->toString());
        return this;
    }

    CHILD_NATIVE_METHOD_DECLARE(concatenate) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_TEXT(value() + message->runFirstInput()->toString());
    }

    CHILD_NATIVE_METHOD_DECLARE(multiply) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_TEXT(value().repeated(message->runFirstInput()->toDouble()));
    }

    CHILD_NATIVE_METHOD_DECLARE(upcase) {
        CHILD_CHECK_INPUT_SIZE(0);
        return CHILD_TEXT(value().toUpper());
    }

    virtual bool isEqualTo(const Node *other) const {
        return value() == Text::cast(other)->value();
    }

    CHILD_NATIVE_METHOD_DECLARE(equal_to) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_BOOLEAN(value() == message->runFirstInput()->toString());
    }

    virtual short compare(const Node *other) const {
        return compare(Text::cast(other)->value());
    }

    short compare(const QString &other) const {
        int result = value().compare(other);
        if(result > 0) return 1;
        else if(result < 0) return -1;
        else return 0;
    }

    CHILD_NATIVE_METHOD_DECLARE(compare) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(compare(message->runFirstInput()->toString()));
    }

    static QString unescapeSequence(const QString &source);
    static QChar unescapeSequenceNumber(const QString &source, int &i);

    virtual double toDouble() const {
        bool ok;
        double number = value().toDouble(&ok);
        if(!ok) CHILD_THROW_CONVERSION_EXCEPTION("conversion from Text to Number failed");
        return number;
    };

    virtual QChar toChar() const {
        if(value().size() != 1) CHILD_THROW_CONVERSION_EXCEPTION("conversion from Text to Character failed (size should be equal to 1)");
        return value().at(0);
    };

    virtual QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(level);
        return debug ? "\"" + value() + "\"" : value();
    }
};

CHILD_END

#endif // CHILD_TEXT_H
