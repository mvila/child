#include "node/controlflow.h"
#include "node/object/boolean.h"
#include "node/object/message.h"
#include "node/object/language/block.h"

CHILD_BEGIN

CHILD_DEFINE(ControlFlow, Node, Node);

void ControlFlow::initRoot() {
    CHILD_ADD_NATIVE_METHOD(ControlFlow, if);
    CHILD_ADD_NATIVE_METHOD(ControlFlow, unless);

    CHILD_ADD_NATIVE_METHOD(ControlFlow, loop);

    CHILD_ADD_NATIVE_METHOD(ControlFlow, while);
    CHILD_ADD_NATIVE_METHOD(ControlFlow, until);

    CHILD_ADD_NATIVE_METHOD(ControlFlow, break);
    CHILD_ADD_NATIVE_METHOD(ControlFlow, continue);
}

Node *ControlFlow::ifOrUnless(bool isIf) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1, 3);
    Node *testResult = message->runFirstInput();
    if(message->hasASecondInput()) {
        if(testResult->toBool() == isIf) return message->runSecondInput(this);
        if(message->hasAThirdInput()) return message->runThirdInput(this);
        return testResult;
    }
    CHILD_FIND_LAST_PRIMITIVE;
    if(Primitive *nextPrimitive = primitive->next()) {
        if(testResult->toBool() == isIf) Primitive::skip(nextPrimitive->run(this));
        Block *block = Block::dynamicCast(nextPrimitive->value());
        if(block && block->elseSection()) Primitive::skip(block->elseSection()->run(this));
        Primitive::skip(testResult);
    }
    CHILD_THROW(InterpreterException, QString("missing code after an %1 statement").arg(isIf ? "if" : "unless"));
}

CHILD_DEFINE_NATIVE_METHOD(ControlFlow, loop) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0, 1);
    CHILD_FIND_LAST_PRIMITIVE;
    Primitive *nextPrimitive = primitive->next();
    if(!nextPrimitive)
        CHILD_THROW(InterpreterException, "missing code after a loop statement");
    Block *block = Block::dynamicCast(nextPrimitive->value());
    Section *between = block ? block->betweenSection() : NULL;
    HugeInteger count;
    if(message->hasAnInput()) { // finite loop
        count = message->runFirstInput()->toDouble();
        if(count < 0) CHILD_THROW(ArgumentException, "loop count must be greater (or equal) than 0");
    } else
        count = -1; // infinite loop
    Node *result = NULL;
    if(count != 0) {
        try {
            HugeInteger i = 0;
            bool first = true;
            while(true) {
                if(count > 0 && i == count) break;
                if(!first) {
                    if(between) try { between->run(); } catch(Continue) {}
                } else
                    first = false;
                try { result = nextPrimitive->run(); } catch(Continue) {}
                i++;
            }
        } catch(const Break &brk) {
            result = brk.result;
        }
    } else
        result = CHILD_NODE();
    Primitive::skip(result);
}

Node *ControlFlow::whileOrUntil(bool isWhile) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    CHILD_FIND_LAST_PRIMITIVE;
    Primitive *nextPrimitive = primitive->next();
    if(!nextPrimitive)
        CHILD_THROW(InterpreterException, QString("missing code after %1 statement").arg(isWhile ? "a while" : "an until"));
    Block *block = Block::dynamicCast(nextPrimitive->value());
    Section *between = block ? block->betweenSection() : NULL;
    Node *result = NULL;
    try {
        Node *test = NULL;
        bool first = true;
        while(true) {
            if(isWhile) {
                test = message->runFirstInput();
                if(test->toBool()) result = test; else break;
            }
            if(!first) {
                if(between) try { between->run(); } catch(Continue) {}
            } else
                first = false;
            try { result = nextPrimitive->run(); } catch(Continue) {}
            if(!isWhile) {
                test = message->runFirstInput();
                if(test->toBool()) break;
            }
        }
    } catch(const Break &brk) {
        result = brk.result;
    }
    Primitive::skip(result);
}

CHILD_DEFINE_NATIVE_METHOD(ControlFlow, break) {
    CHILD_FIND_LAST_PRIMITIVE;
    if(primitive->next())
        CHILD_THROW(InterpreterException, "dead code found after a break statement");
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0, 1);
    Node *result = message->hasAnInput() ? message->runFirstInput() : CHILD_NODE();
    throw Break(result);
}

CHILD_DEFINE_NATIVE_METHOD(ControlFlow, continue) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    throw Continue();
}

CHILD_END
