#include <QtCore/QStringList>

#include "child/node.h"
#include "child/exception.h"
#include "child/nativemethod.h"
//#include "child/boolean.h"
//#include "child/message.h"

CHILD_BEGIN

//using namespace Language;

const bool Node::isInitialized = Node::root();

Node::~Node() {
    if(_extensions) delete _extensions;
    if(_children) {
        foreach(Node *child, *_children) if(child) child->_removeParent(this);
        delete _children;
    }
    if(_parents) delete _parents;
}

Node *Node::root() {
    static Node *_root = NULL;
    if(!_root) {
        GC_INIT();
        _root = new Node(NULL);
        initRoot();
    }
    return _root;
}

void Node::initRoot() {
    root()->addChild("Node", root());
//    CHILD_NATIVE_METHOD_ADD(Node, fork);
//    CHILD_NATIVE_METHOD_ADD(Node, define, :=);
//    CHILD_NATIVE_METHOD_ADD(Node, assign, =);

//    CHILD_NATIVE_METHOD_ADD(Node, or, ||);
//    CHILD_NATIVE_METHOD_ADD(Node, and, &&);
//    CHILD_NATIVE_METHOD_ADD(Node, not, !);

//    CHILD_NATIVE_METHOD_ADD(Node, or_assign, ||=);
//    CHILD_NATIVE_METHOD_ADD(Node, and_assign, &&=);

//    CHILD_NATIVE_METHOD_ADD(Node, equal_to, ==);
//    CHILD_NATIVE_METHOD_ADD(Node, different_from, !=);
//    CHILD_NATIVE_METHOD_ADD(Node, assert, ?:);
//    CHILD_NATIVE_METHOD_ADD(Node, print);
//    CHILD_NATIVE_METHOD_ADD(Node, inspect);
}

void Node::setOrigin(Node *node) {
    CHILD_CHECK_POINTER(node);
    _origin = node;
}

void Node::addExtension(Node *node) {
    CHILD_CHECK_POINTER(node);
    if(!_extensions) { _extensions = new QList<Node *>; }
    if(hasExtension(node)) CHILD_THROW(DuplicateException, "cannot add an extension which is already there");
    _extensions->append(node);
}

void Node::prependExtension(Node *node)  {
    CHILD_CHECK_POINTER(node);
    if(!_extensions) { _extensions = new QList<Node *>; }
    if(hasExtension(node)) CHILD_THROW(DuplicateException, "cannot add an extension which is already there");
    _extensions->prepend(node);
}

void Node::removeExtension(Node *node)  {
    CHILD_CHECK_POINTER(node);
    if(!hasExtension(node)) CHILD_THROW(NotFoundException, "cannot remove an extension which is not there");
    _extensions->removeOne(node);
}

void Node::removeAllExtensions() {
    _extensions->clear();
}

bool Node::hasExtension(Node *node) const {
    CHILD_CHECK_POINTER(node); return _extensions && _extensions->contains(node);
}

Node *Node::child(const QString &name) const {
    Node *node = hasChild(name);
    if(!node) CHILD_THROW(NotFoundException, "child not found");
    return node;
}

Node *Node::addChild(const QString &name, Node *value) {
    CHILD_CHECK_POINTER(value);
    if(hasChild(name, false)) CHILD_THROW(DuplicateException, "child with same name is already there");
    _setChild(name, value);
    return value;
}

Node *Node::setChild(const QString &name, Node *value, bool addOrSetMode) {
    CHILD_CHECK_POINTER(value);
    bool isDirect;
    if(Node *current = hasChild(name, !addOrSetMode, false, &isDirect)) {
        if(isDirect) {
            if(current == value) return value;
            current->_removeParent(this);
        }
    } else if(!addOrSetMode) CHILD_THROW(NotFoundException, "child not found");
    _setChild(name, value);
    return value;
}

void Node::_setChild(const QString &name, Node *value) {
    if(!_children) _children = new QHash<QString, Node *>;
    _children->insert(name, value);
    if(value) value->_addParent(this);
}

void Node::removeChild(const QString &name) {
    bool isDirect;
    if(Node *current = hasChild(name, true, false, &isDirect)) {
        if(isDirect) current->_removeParent(this);
    } else CHILD_THROW(NotFoundException, "child not found");
    _setChild(name, NULL);
}

Node *Node::hasChild(const QString &name, bool searchInParents,
                             bool forkChildFoundInFirstOrigin, bool *isDirectPtr) const {
    bool isRemoved;
    Node *node = hasDirectChild(name, &isRemoved);
    bool isDirect = node || isRemoved;
    if(!isDirect)
        if(origin() && (node = origin()->hasChild(name, searchInParents)))
            if(forkChildFoundInFirstOrigin) {
                node = node->fork();
                constCast(this)->_setChild(name, node);
            }
    if(isDirectPtr) *isDirectPtr = isDirect;
    return node;
}

void Node::_addParent(Node *parent) const {
    HugeUnsignedInteger count = 0;
    if(_parents)
        count = _parents->value(parent);
    else
        _parents = new QHash<Node *, HugeUnsignedInteger>;
    _parents->insert(parent, count + 1);
}

void Node::_removeParent(Node *parent) const {
    if(!_parents) CHILD_THROW(NotFoundException, "parent not found");
    HugeUnsignedInteger count = _parents->value(parent) - 1;
    if(count > 0)
        _parents->insert(parent, count);
    else if(count == 0)
        _parents->remove(parent);
    else
        CHILD_THROW(NotFoundException, "parent not found");
}

QHash<QString, Node *> Node::children() const {
    QHash<QString, Node *> children;
    if(_children) {
        QHashIterator<QString, Node *> i(*_children);
        while(i.hasNext()) if(i.next().value()) children.insert(i.key(), i.value());
    }
    return children;
}

QList<Node *> Node::parents() const {
    QList<Node *> parents;
    if(_parents) foreach(Node *parent, _parents->keys()) parents.append(parent);
    return parents;
}

//Node *Node::run(Node *receiver, Message *message) {
//    Q_UNUSED(receiver);
//    if(message->inputs(false) || message->outputs(false) || message->block()) {
//        Message *forkMessage = message->fork();
//        forkMessage->setName("fork");
//        return forkMessage->run(this);
//    } else
//        return this;
//}

//CHILD_NATIVE_METHOD_DEFINE(Node, fork) {
//    Node *node = fork();
//    if(node->hasChild("init")) {
//        Message* init = message->fork();
//        init->setName("init");
//        init->run(node);
//    }
//    return node;
//}

//Node *Node::defineOrAssign(Message *message, bool isDefine) {
//    CHILD_CHECK_INPUT_SIZE(2);
//    PrimitiveChain *chain = message->firstInput()->value();
//    Node *context = chain->runExceptLast();
//    Message *msg(chain->last()->value(), true);
//    if(!msg) CHILD_THROW(ArgumentException, "left-hand side is not a message");
//    Node *value;
//    Block *block(message->secondInput()->value()->first()->value(), true);
//    if(block) // if rhs is a block, its a method definition shorthand
//        value = CHILD_MESSAGE("Method", NULL, NULL, block)->run();
//    else // rhs is not a block
//        value = message->runSecondInput();
//    Node *result = context->setChild(msg->name(), value, isDefine);
//    value->hasBeenAssigned(msg);
//    return result;
//}

//CHILD_NATIVE_METHOD_DEFINE(Node, or) {
//    CHILD_CHECK_INPUT_SIZE(1);
//    return CHILD_BOOLEAN(toBool() || message->runFirstInput()->toBool());
//}

//CHILD_NATIVE_METHOD_DEFINE(Node, and) {
//    CHILD_CHECK_INPUT_SIZE(1);
//    return CHILD_BOOLEAN(toBool() && message->runFirstInput()->toBool());
//}

//CHILD_NATIVE_METHOD_DEFINE(Node, not) {
//    CHILD_CHECK_INPUT_SIZE(0);
//    return CHILD_BOOLEAN(!toBool());
//}

//CHILD_NATIVE_METHOD_DEFINE(Node, or_assign) {
//    CHILD_CHECK_INPUT_SIZE(2);
//    Node *lhs = message->runFirstInput();
//    if(!lhs->toBool())
//        return CHILD_MESSAGE("=", message->firstInput(), message->secondInput())->run();
//    else
//        return lhs;
//}

//CHILD_NATIVE_METHOD_DEFINE(Node, and_assign) {
//    CHILD_CHECK_INPUT_SIZE(2);
//    Node *lhs = message->runFirstInput();
//    if(lhs->toBool())
//        return CHILD_MESSAGE("=", message->firstInput(), message->secondInput())->run();
//    else
//        return lhs;
//}

//CHILD_NATIVE_METHOD_DEFINE(Node, equal_to) {
//    CHILD_CHECK_INPUT_SIZE(1);
//    return CHILD_BOOLEAN(isEqualTo(message->runFirstInput()));
//}

//CHILD_NATIVE_METHOD_DEFINE(Node, different_from) {
//    return CHILD_BOOLEAN(!Boolean::cast(CHILD_MESSAGE("==", message->inputs(false))->run(this))->value());
//}

//CHILD_NATIVE_METHOD_DEFINE(Node, assert) {
//    CHILD_CHECK_INPUT_SIZE(0);
//    if(toBool())
//        return this;
//    else
//        CHILD_THROW(AssertionException, "assertion failed");
//}

//CHILD_NATIVE_METHOD_DEFINE(Node, print) {
//    CHILD_CHECK_INPUT_SIZE(0);
//    return print();
//}

//CHILD_NATIVE_METHOD_DEFINE(Node, inspect) {
//    CHILD_CHECK_INPUT_SIZE(0);
//    return inspect();
//}

QString Node::toString(bool debug, short level) const {
    Q_UNUSED(debug);
    Q_UNUSED(level);
    return QString("%1: [%2]").arg(hexMemoryAddress(), QStringList(children().keys()).join(", "));
}

CHILD_END
