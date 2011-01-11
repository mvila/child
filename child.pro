#-------------------------------------------------
#
# Project created by QtCreator 2010-08-23T15:21:38
#
#-------------------------------------------------

QT       += core
QT       += testlib
QT       -= gui

TARGET = child
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    child.cpp \
    child/global.cpp \
    child/node.cpp \
    child/exception.cpp \
    tests/nodetest.cpp \
    child/nativemethod.cpp \
    child/object.cpp \
    tests/objecttest.cpp \
    child/element.cpp \
    child/boolean.cpp \
    child/number.cpp \
    child/character.cpp \
    child/text.cpp \
    tests/texttest.cpp \
    child/pair.cpp \
    child/list.cpp \
    tests/listtest.cpp \
    child/bunch.cpp \
    child/dictionary.cpp \
    tests/dictionarytest.cpp \
    child/block.cpp \
    child/method.cpp \
    child/message.cpp \
    child/controlflow.cpp \
    child/application.cpp \
    child/language.cpp \
    child/language/token.cpp \
    child/language/operator.cpp \
    child/language/operatortable.cpp \
    child/language/primitive.cpp \
    child/language/argument.cpp \
    child/language/argumentbunch.cpp \
    child/language/parameter.cpp \
    child/language/parameterlist.cpp \
    child/language/section.cpp \
    child/language/sourcecode.cpp \
    child/language/sourcecodedictionary.cpp \
    child/language/test.cpp \
    child/language/testsuite.cpp \
    child/language/lexer.cpp \
    child/language/parser.cpp \
    child/language/interpreter.cpp \
    child/property.cpp

HEADERS += \
    child.h \
    child/global.h \
    child/node.h \
    child/exception.h \
    tests/testbegin.h \
    tests/testend.h \
    tests/runalltests.h \
    tests/nodetest.h \
    child/nativemethod.h \
    child/object.h \
    tests/objecttest.h \
    child/element.h \
    child/boolean.h \
    child/number.h \
    child/character.h \
    child/text.h \
    tests/texttest.h \
    child/pair.h \
    child/list.h \
    tests/listtest.h \
    child/bunch.h \
    child/dictionary.h \
    tests/dictionarytest.h \
    child/block.h \
    child/method.h \
    child/message.h \
    child/controlflow.h \
    child/application.h \
    child/language.h \
    child/language/token.h \
    child/language/operator.h \
    child/language/operatortable.h \
    child/language/primitive.h \
    child/language/argument.h \
    child/language/argumentbunch.h \
    child/language/parameter.h \
    child/language/parameterlist.h \
    child/language/section.h \
    child/language/sourcecode.h \
    child/language/sourcecodedictionary.h \
    child/language/test.h \
    child/language/testsuite.h \
    child/language/lexer.h \
    child/language/parser.h \
    child/language/interpreter.h \
    child/property.h

OTHER_FILES += \
    notes/ideas.txt \
    notes/syntax.txt \
    notes/pseudocode.txt \
    README.txt \
    notes/todo.txt \
    notes/old.txt \
    examples/lexertest.child \
    notes/specification.txt \
    examples/test.child \
    notes/features.txt \
    child/node.child \
    child/exception.child
