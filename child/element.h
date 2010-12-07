#ifndef CHILD_ELEMENT_H
#define CHILD_ELEMENT_H

#include "child/object.h"

CHILD_BEGIN

inline uint qHash(const double &number) { CHILD_TODO; return number; }

template<class P, class T>
class GenericElement : public Object {
public:
    GenericElement(const NodePtr &origin, const T &value = NULL) : Object(origin), _value(value) {}

    T value() const { return _value; }
    void setValue(const T &value) { _value = value; }

    virtual uint hash() const { return qHash(_value); }
private:
    T _value;
};

CHILD_PTR_DECLARATION(Element, Object);

#define CHILD_ELEMENT(ARGS...) ElementPtr(new Element(Node::findInContext("Object")->child("Element"), ##ARGS))

class Element : public GenericElement<ElementPtr, NodePtr> {
    CHILD_DECLARATION(Element, Object);
public:
    Element(const NodePtr &origin, const NodePtr &value = NULL) : GenericElement<ElementPtr, NodePtr>(origin, value) {}
    static void initRoot() { Object::root()->addChild("Element", root()); }
    virtual NodePtr fork() const { return new Element(this, value()->fork()); }
    virtual const QString toString(bool debug = false) const { return value() ? value()->toString(debug) : "NULL"; }
};

CHILD_PTR_DEFINITION(Element, Object);

CHILD_END

#endif // CHILD_ELEMENT_H
