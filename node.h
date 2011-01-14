#ifndef CHILD_NODE_H
#define CHILD_NODE_H

#include <QtCore/QHash>

#include "child.h"

CHILD_BEGIN

#define CHILD_NODE(ARGS...) new Node(context()->child("Node"), ##ARGS)

#define CHILD_FORK_METHOD(NAME, ARGS...) \
virtual NAME *fork() const { \
    NAME *node = new NAME(constCast(this), ##ARGS); \
    node->initFork(); \
    return node; \
}

#define CHILD_FORK_METHOD_DECLARE(NAME) \
virtual NAME *fork() const;

#define CHILD_FORK_METHOD_DEFINE(NAME, ARGS...) \
NAME *NAME::fork() const { \
    NAME *node = new NAME(constCast(this), ##ARGS); \
    node->initFork(); \
    return node; \
}

#define CHILD_FORK_IF_NOT_NULL(NODE) \
((NODE) ? (NODE)->fork() : NULL)

#define CHILD_CHECK_POINTER(POINTER) \
if(!(POINTER)) CHILD_THROW_NULL_POINTER_EXCEPTION("Node pointer is NULL")

class Message;

namespace Language {
    class Primitive;
}

using namespace Language;

#define CHILD_NATIVE_METHOD_DECLARE(METHOD) \
Node *_##METHOD##_(Message *message, Primitive *)

#define CHILD_NATIVE_METHOD_DEFINE(NAME, METHOD) \
Node *NAME::_##METHOD##_(Message *message, Primitive *)

#define CHILD_NATIVE_METHOD_WITH_CODE_INPUT_DECLARE(METHOD) \
Node *_##METHOD##_(Message *message, Primitive *code)

#define CHILD_NATIVE_METHOD_WITH_CODE_INPUT_DEFINE(NAME, METHOD) \
Node *NAME::_##METHOD##_(Message *message, Primitive *code)

class Node {
public:
    static const bool isInitialized;

    explicit Node(Node *origin) : _origin(origin), _extensions(NULL), // default constructor
        _children(NULL), _parents(NULL), _isAbstract(true), _isVirtual(false) {}

    Node(const Node &other) : _origin(other._origin), _extensions(NULL), // copy constructor
        _children(NULL), _parents(NULL), _isAbstract(other._isAbstract), _isVirtual(other._isVirtual) {
        if(other._extensions) _extensions = new QList<Node *>(*other._extensions);
        if(other._children) {
            QHashIterator<QString, Node *> i(other.children());
            while(i.hasNext()) { i.next(); addChild(i.key(), i.value()); }
        }
    }

    virtual ~Node();

    inline static Node *constCast(const Node *node) { return const_cast<Node *>(node); }

    static Node *root();
    virtual void initRoot();

    const QString className() const;
    void setClassName(const QString &name);
    const QString classPath() const;

    void declare(const QString &name) const;

    CHILD_FORK_METHOD(Node);
    virtual void initFork() {}

    CHILD_NATIVE_METHOD_DECLARE(self);
    CHILD_NATIVE_METHOD_DECLARE(fork);

    Node *origin() const {
        if(!_origin) CHILD_THROW_NULL_POINTER_EXCEPTION("origin is NULL");
        return _origin;
    }

    CHILD_NATIVE_METHOD_DECLARE(origin_get);

    void setOrigin(Node *node);
    CHILD_NATIVE_METHOD_DECLARE(origin_set);

    bool isAbstract() const { return _isAbstract; }
    void setIsAbstract(bool isAbstract) { _isAbstract = isAbstract; }

    bool isVirtual() const { return _isVirtual; }
    void setIsVirtual(bool isVirtual) { _isVirtual = isVirtual; }

    Node *real();
    const Node *real() const { return constCast(this)->real(); }

    void addExtension(Node *node);
    void prependExtension(Node *node);
    void removeExtension(Node *node);
    void removeAllExtensions();
    bool hasExtension(Node *node) const;
    QList<Node *> extensions() const;

    CHILD_NATIVE_METHOD_DECLARE(extensions_get);

    Node *child(const QString &name) const;

    Node *child(const QString &name1, const QString &name2) const {
        return child(name1)->child(name2);
    }

    Node *child(const QString &name1, const QString &name2, const QString &name3) const {
        return child(name1)->child(name2)->child(name3);
    }

    Node *addChild(const QString &name, Node *value);
    Node *setChild(const QString &name, Node *value, bool addOrSetMode = false);
private:
    void _setChild(const QString &name, Node *value);
public:
    Node *addOrSetChild(const QString &name, Node *value) { return setChild(name, value, true); }

private:
    Node *defineOrAssign(Message *message, bool isDefine);
public:
    CHILD_NATIVE_METHOD_DECLARE(define) { return defineOrAssign(message, true); }
    CHILD_NATIVE_METHOD_DECLARE(assign) { return defineOrAssign(message, false); }

    virtual void hasBeenAssigned(Message *message) const {
        Q_UNUSED(message);
    }

    void removeChild(const QString &name);

    void addAnonymousChild(Node *value) {
        CHILD_CHECK_POINTER(value);
        value->_addParent(this);
    }

    void removeAnonymousChild(Node *value) {
        CHILD_CHECK_POINTER(value);
        value->_removeParent(this);
    }

    Node *hasChild(const QString &name, bool searchInParents = true, Node **parentPtr = NULL,
                   bool autoFork = true, bool *isDirectPtr = NULL) const;
private:
    Node *hasChildInSelfOrOrigins(const QString &name, bool autoFork = true, bool *isDirectPtr = NULL) const;
public:

    Node *hasDirectChild(const QString &name, bool *isRemovedPtr = NULL) const {
        Node *child = NULL;
        if(_children) child = _children->value(name);
        if(isRemovedPtr) *isRemovedPtr = !child && _children && _children->contains(name);
        return child;
    }

    QString hasDirectChild(Node *value) const {
        return _children ? _children->key(value) : QString();
    }

    bool hasDirectParent(Node *parent) const {
        CHILD_CHECK_POINTER(parent);
        return _parents && _parents->contains(parent);
    }

private:
    void _addParent(Node *parent) const;
    void _removeParent(Node *parent) const;
public:

    QHash<QString, Node *> children() const;
    QList<Node *> parents() const;

    Node *parent() const;
    CHILD_NATIVE_METHOD_DECLARE(parent);
    bool hasOneParent() const;
    CHILD_NATIVE_METHOD_DECLARE(parent_qm);

    virtual Node *receive(Primitive *primitive);

    virtual Node *run(Node *receiver = context()) {
        Q_UNUSED(receiver);
        return this;
    }

    virtual Node *run(Node *receiver, Message *message, Primitive *code = NULL);

    CHILD_NATIVE_METHOD_DECLARE(or);
    CHILD_NATIVE_METHOD_DECLARE(and);
    CHILD_NATIVE_METHOD_DECLARE(not);

    CHILD_NATIVE_METHOD_DECLARE(or_assign);
    CHILD_NATIVE_METHOD_DECLARE(and_assign);

    virtual bool isEqualTo(const Node *other) const { return real() == other->real(); }
    CHILD_NATIVE_METHOD_DECLARE(equal_to);
    CHILD_NATIVE_METHOD_DECLARE(different_from);

    static HugeUnsignedInteger &passedAssertionCount() {
        static HugeUnsignedInteger _count = 0;
        return _count;
    }

    CHILD_NATIVE_METHOD_DECLARE(assert);

    void print() const { P(toString().toUtf8()); }
    CHILD_NATIVE_METHOD_DECLARE(print);

    void inspect() const { P(toString(true).toUtf8()); }
    CHILD_NATIVE_METHOD_DECLARE(inspect);

    long long int memoryAddress() const { return reinterpret_cast<long long int>(this); }
    CHILD_NATIVE_METHOD_DECLARE(memory_address);

    QString hexMemoryAddress() const { return QString("0x%1").arg(memoryAddress(), 0, 16); }

    virtual bool toBool() const { return true; };

    virtual double toDouble() const {
        CHILD_THROW_CONVERSION_EXCEPTION(QString("cannot convert from %1 to Number").arg(className()));
        return 0;
    };

    virtual QChar toChar() const {
        CHILD_THROW_CONVERSION_EXCEPTION(QString("cannot convert from %1 to Character").arg(className()));
        return 0;
    };

    virtual QString toString(bool debug = false, short level = 0) const;

    virtual uint hash() const { return ::qHash(this); }

    class Reference {
    public:
        Reference(Node *node) : _node(node) {}
        Node &operator*() { return *_node; }
        const Node &operator*() const { return *_node; }
        Node *operator->() { return _node; }
        const Node *operator->() const { return _node; }
        operator bool() const { return _node; }
        bool operator!() const { return !_node; }
        operator Node *() const { return _node; }
    private:
        Node *_node;
    };
private:
    Node *_origin;
    QList<Node *> *_extensions;
    QHash<QString, Node *> *_children;
    mutable QHash<Node *, HugeUnsignedInteger> *_parents;
    bool _isAbstract : 1;
    bool _isVirtual  : 1;
};

inline bool operator==(const Node &a, const Node &b) { return a.isEqualTo(&b); }
inline bool operator!=(const Node &a, const Node &b) { return !a.isEqualTo(&b); }
inline uint qHash(const Node &node) { return node.hash(); }

inline bool operator==(const Node::Reference &a, const Node::Reference &b) { return a->isEqualTo(b); }
inline bool operator!=(const Node::Reference &a, const Node::Reference &b) { return !a->isEqualTo(b); }
inline uint qHash(const Node::Reference &node) { return node->hash(); }

#define CHILD_DECLARE(NAME, ORIGIN, PARENT) \
public: \
    inline static NAME *cast(Node *node) { return static_cast<NAME *>(node); } \
    inline static const NAME *cast(const Node *node) { return static_cast<const NAME *>(node); } \
    inline static NAME *dynamicCast(Node *node) { return dynamic_cast<NAME *>(node); } \
    inline static const NAME *dynamicCast(const Node *node) { return dynamic_cast<const NAME *>(node); } \
    inline static NAME *constCast(const NAME *node) { return const_cast<NAME *>(node); } \
    static NAME *root(); \
    virtual void initRoot(); \
    static const bool isInitialized; \
private:

#define CHILD_DEFINE(NAME, ORIGIN, PARENT) \
const bool NAME::isInitialized = NAME::root(); \
NAME *NAME::root() { \
    static NAME *_root = NULL; \
    if(!_root) { \
        _root = new NAME(ORIGIN::root()); \
        PARENT::root()->addChild(#NAME, _root); \
        _root->declare(#NAME); \
        _root->initRoot(); \
    } \
    return _root; \
}

CHILD_END

#endif // CHILD_NODE_H