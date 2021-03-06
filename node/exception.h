#ifndef CHILD_EXCEPTION_H
#define CHILD_EXCEPTION_H

#include "node.h"
#include "node/runstack.h"

CHILD_BEGIN

#define CHILD_EXCEPTION(ARGS...) new Exception(context()->child("Exception"), ##ARGS)

#define CHILD_THROW(EXCEPTION, MESSAGE) \
throw EXCEPTION(context()->child(#EXCEPTION), MESSAGE, __FILE__, __LINE__, Q_FUNC_INFO)

#define CHILD_TODO \
CHILD_THROW(Exception, "function not yet implemented")

class Exception : public Node {
    CHILD_DECLARE(Exception, Node, Node);
public:
    QString message;
    QString file;
    int line;
    QString function;
    RunStack *runStackCapture;

    explicit Exception(Node *origin, const QString &message = "", const QString &file = "",
              const int line = 0, const QString &function = "", RunStack *runStackCapture = runStack()->copy()) :
        Node(origin), message(message), file(file), line(line), function(function), runStackCapture(runStackCapture) {}

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Exception);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Exception, message, file, line, function, runStackCapture);

    const QString report() const;

    virtual QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(debug);
        Q_UNUSED(level);
        return report();
    }
};

#define CHILD_EXCEPTION_DECLARATION(NAME, ORIGIN) \
class NAME : public ORIGIN { \
    CHILD_DECLARE(NAME, ORIGIN, Node); \
public: \
    explicit NAME(Node *origin, const QString &message = "", const QString &file = "", \
         const int line = 0, const QString &function = "", RunStack *runStackCapture = runStack()->copy()) : \
        ORIGIN(origin, message, file, line, function, runStackCapture) {} \
    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(NAME); \
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(NAME, message, file, line, function, runStackCapture); \
};

#define CHILD_EXCEPTION_DEFINITION(NAME, ORIGIN) \
CHILD_DEFINE(NAME, ORIGIN, Node); \
void NAME::initRoot() {}

CHILD_EXCEPTION_DECLARATION(LexerException, Exception);
CHILD_EXCEPTION_DECLARATION(ParserException, Exception);
CHILD_EXCEPTION_DECLARATION(RuntimeException, Exception);
CHILD_EXCEPTION_DECLARATION(InterpreterException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(ArgumentException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(NullPointerException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(IndexOutOfBoundsException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(NotFoundException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(DuplicateException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(TypecastException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(ConversionException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(AssertionException, RuntimeException);
CHILD_EXCEPTION_DECLARATION(OperatingSystemException, Exception);
CHILD_EXCEPTION_DECLARATION(FileSystemException, OperatingSystemException);

CHILD_END

#endif // CHILD_EXCEPTION_H
