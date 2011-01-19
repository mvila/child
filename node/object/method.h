#ifndef CHILD_METHOD_H
#define CHILD_METHOD_H

#include "node/object/element.h"
#include "node/object/block.h"
#include "node/object/message.h"
#include "node/object/language/parameterlist.h"

CHILD_BEGIN

#define CHILD_METHOD(ARGS...) new Method(context()->child("Object", "Method"), ##ARGS)

class Method : public GenericNodeElement<Block> {
    CHILD_DECLARE(Method, Element, Object);
public:
    explicit Method(Node *origin, ParameterList *inputs = NULL, ParameterList *outputs = NULL, Block *block = NULL) :
        GenericNodeElement<Block>(origin), _inputs(inputs), _outputs(outputs) { setBlock(block); }

    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Method, CHILD_FORK_IF_NOT_NULL(inputs(false)), CHILD_FORK_IF_NOT_NULL(outputs(false)),
                      CHILD_FORK_IF_NOT_NULL(block()));

    CHILD_DECLARE_NATIVE_METHOD(init);

    Block *block() const { return value(); } // aliases
    void setBlock(Block *block) { setValue(block); }

    virtual void hasChanged() {
        setIsAutoRunnable(block());
        if(block()) block()->runMetaSections(this);
    }

    ParameterList *inputs(bool createIfNull = true) const {
        if(!_inputs && createIfNull) constCast(this)->_inputs = CHILD_PARAMETER_LIST();
        return _inputs;
    }

    void setInputs(ParameterList *inputs) { _inputs = inputs; }

    void setInputs(ArgumentBunch *arguments) {
        if(hasAnInput()) inputs()->clear();
        ArgumentBunch::Iterator i(arguments);
        while(Argument *argument = i.next()) {
            Primitive *label = argument->label();
            Primitive *defaultValue = argument->value();
            if(!label) {
                label = defaultValue;
                defaultValue = NULL;
            }
            if(label->hasNext())
                CHILD_THROW(ArgumentException, "illegal label parameter found in method definition (should be a Message");
            Message *labelMsg = Message::dynamicCast(label->value());
            if(!labelMsg)
                CHILD_THROW(ArgumentException, "illegal label parameter found in method definition (should be a Message)");
            inputs()->append(CHILD_PARAMETER(labelMsg->name(), defaultValue,
                                             labelMsg->isEscaped(), labelMsg->isParented()));
        }
    }

    Parameter *input(short i) const { return inputs(false)->get(i); }
    Parameter *input(const QString &label) const { return inputs(false)->get(label); }
    bool hasInput(short i) const { return inputs(false) && inputs()->hasIndex(i); }
    bool hasInput(const QString &label) const { return inputs(false) && inputs()->hasLabel(label); }
    bool hasAnInput() const { return hasInput(0); }

    ParameterList *outputs(bool createIfNull = true) const {
        if(!_outputs && createIfNull) constCast(this)->_outputs = CHILD_PARAMETER_LIST();
        return _outputs;
    }

    void setOutputs(ParameterList *outputs) { _outputs = outputs; }

    virtual Node *run(Node *receiver) {
        Q_UNUSED(receiver);
        CHILD_FIND_LAST_MESSAGE;
        Method *forkedMethod = this; //->fork();
        QHash<QString, Parameter *> labels(inputs()->labels());
        if(message->inputs(false)) {
            ArgumentBunch::Iterator iterator(message->inputs());
            short i = -1;
            bool labelFound = false;
            while(Argument *argument = iterator.next()) {
                Parameter *parameter;
                if(argument->label()) {
                    QString labelName = argument->labelName();
                    if(!hasInput(labelName)) CHILD_THROW(NotFoundException, "unknown parameter label");
                    if(!labels.contains(labelName)) CHILD_THROW(DuplicateException, "duplicated parameter label");
                    parameter = input(labelName);
                    labelFound = true;
                } else {
                    if(labelFound) CHILD_THROW(ArgumentException, "positional arguments are forbidden after labeled ones");
                    i++;
                    if(!hasInput(i)) CHILD_THROW(IndexOutOfBoundsException, "too many arguments");
                    parameter = input(i);
                }
                Node *rcvr = parameter->isParented() ? forkedMethod->parent() : forkedMethod;
                Node *val = parameter->isEscaped() ? argument->value() : argument->run();
                rcvr->addOrSetChild(parameter->label(), val);
                labels.remove(parameter->label());
            }
        }
        foreach(Parameter *parameter, labels) {
            if(!parameter->defaultValue()) CHILD_THROW(ArgumentException, "missing mandatory parameter");
            Node *rcvr = parameter->isParented() ? forkedMethod->parent() : forkedMethod;
            Node *val = parameter->isEscaped() ? parameter->defaultValue() : parameter->run();
            rcvr->addOrSetChild(parameter->label(), val);
        }
        Section *body = NULL;
        if(forkedMethod->block()) body = forkedMethod->block()->bodySection();
        if(body) {
            try {
                CHILD_PUSH_CONTEXT(forkedMethod);
                CHILD_PUSH_RUN(this);
                return body->run();
            } catch(const Return &ret) {
                return ret.result;
            }
        } else
            return CHILD_NODE();
    }

    virtual void hasBeenDefined(Message *message) {
        setNodeName(message->name());
        if(message->hasAnInput()) setInputs(message->inputs());
    }

    virtual QString toString(bool debug = false, short level = 0) const {
        QString str = nodeName();
        str += "(";
        if(inputs(false)) str += inputs()->toString(debug, level);
        str += ")";
        if(outputs(false) && outputs()->isNotEmpty())
            str += " -> " + outputs()->toString(debug, level);
        if(block()) str += " " + block()->toString(debug, level + 1);
        return str;
    }
private:
    ParameterList *_inputs;
    ParameterList *_outputs;
public:
    // === Return ===

    class Return {
    public:
        Return(Node *result = NULL) : result(result) {}

        Node *result;
    };

    CHILD_DECLARE_NATIVE_METHOD(return);
};

CHILD_END

#endif // CHILD_METHOD_H
