#ifndef CHILD_DICTIONARY_H
#define CHILD_DICTIONARY_H

#include "child/exception.h"
#include "child/object.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Dictionary, Object);

#define CHILD_DICTIONARY(...) DictionaryPtr(new Dictionary(__VA_ARGS__))

#define CHILD_CHECK_KEY(KEY) \
if(!(KEY)) CHILD_THROW(NullPointerException, "key is NULL")

#define CHILD_CHECK_VALUE(VALUE) \
if(!(VALUE)) CHILD_THROW(NullPointerException, "value is NULL")

class Dictionary : public Object {
    CHILD_DECLARATION(Dictionary, Object);
public:
    Dictionary(const NodeHash &other = NodeHash(), // default constructor
               const DictionaryPtr &origin = find("Object")->child("Dictionary")) :
        Object(origin), _hash(NULL) {
        if(!other.isEmpty()) {
            QHashIterator<NodeRef, NodePtr> i(other);
            while(i.hasNext()) { i.next(); set(i.key(), i.value()); }
        }
    }

    Dictionary(const ObjectPtr &origin) : Object(origin), _hash(NULL) {} // root constructor

    Dictionary(const DictionaryPtr &origin) : Object(origin), _hash(NULL) { // fork constructor
        if(origin->isNotEmpty()) {
            QHashIterator<NodeRef, NodePtr> i(*origin->_hash);
            while(i.hasNext()) { i.next(); set(i.key(), i.value()->fork()); }
        }
    }

    Dictionary(const Dictionary &other) : Object(other), _hash(NULL) { // copy constructor
        if(other.isNotEmpty()) {
            QHashIterator<NodeRef, NodePtr> i(*other._hash);
            while(i.hasNext()) { i.next(); set(i.key(), i.value()); }
        }
    }

    virtual ~Dictionary() {
        if(_hash) {
            foreach(NodePtr node, *_hash) removeAnonymousChild(node);
            delete _hash;
        }
    }

    NodePtr get(const NodeRef &key) const {
        CHILD_CHECK_KEY(key);
        if(!hasKey(key)) CHILD_THROW(NotFoundException, "key not found");
        return _hash->value(key);
    }

    NodePtr set(const NodeRef &key, const NodePtr &value) {
        CHILD_CHECK_KEY(key);
        CHILD_CHECK_VALUE(value);
        if(!_hash) { _hash = new NodeHash; }
        if(NodePtr oldValue = _hash->value(key)) removeAnonymousChild(oldValue);
        _hash->insert(key, value);
        addAnonymousChild(value);
        return value;
    }

//        Node *key(Node *value) const;
    NodePtr value(const NodeRef &key) const { return get(key); }
    int size() const { return _hash ? _hash->size() : 0; }
    bool isEmpty() const { return size() == 0; }
    bool isNotEmpty() const { return size() > 0; }

    NodeList keys() const {
        NodeList list;
        if(_hash) foreach(NodeRef node, _hash->keys()) list.append(node);
        return(list);
    }
//        NodeList values() const;
    bool hasKey(const NodeRef &key) const { CHILD_CHECK_KEY(key); return _hash && _hash->contains(key); }
//        bool hasValue(Node *value) const;

    DictionaryPtr remove(const NodeRef &key) {
        CHILD_CHECK_KEY(key);
        NodePtr oldValue;
        if(!(_hash && (oldValue = _hash->value(key)))) CHILD_THROW(NotFoundException, "key not found");
        removeAnonymousChild(oldValue);
        _hash->remove(key);
        return DictionaryPtr(this);
    }

    DictionaryPtr clear() {
        foreach(NodeRef key, keys()) remove(key);
        return DictionaryPtr(this);
    }

    virtual const QString inspect() const {
        QString str = "[";
        bool first = true;
        foreach(NodeRef key, keys()) {
            if(!first) str.append(", "); else first = false;
            str.append(QString("%1: %2").arg(key->inspect(), get(key)->inspect()));
        }
        str.append("]");
        return str;
    }
private:
    NodeHash *_hash;
};

CHILD_PTR_DEFINITION(Dictionary, Object);

CHILD_END

#endif // CHILD_DICTIONARY_H
