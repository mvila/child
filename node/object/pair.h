#ifndef CHILD_PAIR_H
#define CHILD_PAIR_H

#include "node/object.h"

CHILD_BEGIN

template<class T1, class T2>
class GenericPair : public Object {
public:
    explicit GenericPair(Node *origin, const T1 &first, const T2 &second) :
        Object(origin), _first(first), _second(second) {}

    T1 first() const { return _first; }

    void setFirst(const T1 &first) {
        if(first != _first) {
            firstValueWillChange();
            _first = first;
            firstValueHasChanged();
        }
    }

    virtual void firstValueWillChange() {}
    virtual void firstValueHasChanged() {}

    T2 second() const { return _second; }

    void setSecond(const T2 &second) {
        if(second != _second) {
            secondValueWillChange();
            _second = second;
            secondValueHasChanged();
        }
    }

    virtual void secondValueWillChange() {}
    virtual void secondValueHasChanged() {}

    // aliases...
    T1 key() const { return first(); }
    void setKey(const T1 &key) { setFirst(key); }
    T2 value() const { return second(); }
    void setValue(const T2 &value) { setSecond(value); }
private:
    T1 _first;
    T2 _second;
};

#define CHILD_PAIR(ARGS...) new Pair(context()->child("Object", "Pair"), ##ARGS)

class Pair : public GenericPair<Node *, Node *> {
    CHILD_DECLARE(Pair, Object, Object);
public:
    explicit Pair(Node *origin, Node *first = NULL, Node *second = NULL) :
        GenericPair<Node *, Node *>(origin, NULL, NULL) { setFirst(first); setSecond(second); }

    virtual ~Pair() {
        setFirst(NULL);
        setSecond(NULL);
    }

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Pair);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Pair, CHILD_FORK_IF_NOT_NULL(first()), CHILD_FORK_IF_NOT_NULL(second()));

    virtual void firstValueWillChange() { if(first()) removeAnonymousChild(first()); }
    virtual void firstValueHasChanged() { if(first()) addAnonymousChild(first()); }
    virtual void secondValueWillChange() { if(second()) removeAnonymousChild(second()); }
    virtual void secondValueHasChanged() { if(second()) addAnonymousChild(second()); }

    virtual QString toString(bool debug = false, short level = 0) const {
        return QString("%1: %2").arg(first()->toString(debug, level), second()->toString(debug, level));
    }
};

CHILD_END

#endif // CHILD_PAIR_H
