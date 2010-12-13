#ifndef CHILD_DICTIONARY_TEST_H
#define CHILD_DICTIONARY_TEST_H

#include "child/text.h"
#include "child/dictionary.h"
#include "child/list.h"

#include "tests/testbegin.h"

class DictionaryTest : public QObject {
    Q_OBJECT
    CHILD_TEST

    void initialize();
    void getAndSet();
    void keys();
    void remove();
    void clear();
};

#include "tests/testend.h"

#endif // CHILD_DICTIONARY_TEST_H
