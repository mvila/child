#ifndef CHILD_NUMBER_H
#define CHILD_NUMBER_H

#include <math.h>

#include "node/object/element.h"
#include "node/object/boolean.h"
#include "node/object/message.h"

CHILD_BEGIN

#define CHILD_NUMBER(ARGS...) new Number(context()->child("Object", "Number"), ##ARGS)

class Number : public GenericElement<double> {
    CHILD_DECLARE(Number, Element, Object);
public:
    explicit Number(Node *origin, const double value = 0) : GenericElement<double>(origin, 0) { setValue(value); }

    Number(const Number &other) : GenericElement<double>(other) { setValue(other.value()); }

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Number);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Number, value());

    CHILD_DECLARE_NATIVE_METHOD(init) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(0, 1);
        if(message->hasInput(0)) {
            bool ok = true;
            double value = message->runFirstInput()->toDouble(message->isQuestioned() ? &ok : NULL);
            if(ok) setValue(value);
        }

        // === TODO: DRY ===
        CHILD_FIND_LAST_PRIMITIVE;
        Primitive *nextPrimitive = primitive->next();
        if(nextPrimitive) {
            nextPrimitive->run(this);
            Primitive::skip(this);
        }

        return this;
    }

    CHILD_DECLARE_NATIVE_METHOD(add) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(value() + message->runFirstInput()->toDouble());
    }

    CHILD_DECLARE_NATIVE_METHOD(subtract) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(value() - message->runFirstInput()->toDouble());
    }

    CHILD_DECLARE_NATIVE_METHOD(multiply) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(value() * message->runFirstInput()->toDouble());
    }

    CHILD_DECLARE_NATIVE_METHOD(divide) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(value() / message->runFirstInput()->toDouble());
    }

    CHILD_DECLARE_NATIVE_METHOD(modulo) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(llround(value()) % llround(message->runFirstInput()->toDouble()));
    }

    CHILD_DECLARE_NATIVE_METHOD(unary_plus) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(0);
        return CHILD_NUMBER(value());
    }

    CHILD_DECLARE_NATIVE_METHOD(unary_minus) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(0);
        return CHILD_NUMBER(-value());
    }

    CHILD_DECLARE_NATIVE_METHOD(prefix_increment) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(0);
        setValue(value() + 1);
        return this;
    }

    CHILD_DECLARE_NATIVE_METHOD(prefix_decrement) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(0);
        setValue(value() - 1);
        return this;
    }

    virtual bool isEqualTo(const Node *other) const {
        return value() == Number::cast(other)->value();
    }

    CHILD_DECLARE_NATIVE_METHOD(equal_to) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_BOOLEAN(value() == message->runFirstInput()->toDouble());
    }

    virtual short compare(const Node *other) const {
        return compare(Number::cast(other)->value());
    }

    short compare(const double &other) const {
        if(value() > other) return 1;
        else if(value() < other) return -1;
        else return 0;
    }

    CHILD_DECLARE_NATIVE_METHOD(compare) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(compare(message->runFirstInput()->toDouble()));
    }

    virtual double toDouble(bool *okPtr = NULL) const {
        Q_UNUSED(okPtr);
        return value();
    };

    virtual QChar toChar() const { return QChar(int(value())); };

    virtual QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(debug);
        Q_UNUSED(level);
        return QString("%1").arg(value(), 0, 'g', 13);
    }
};

CHILD_END

#endif // CHILD_NUMBER_H
