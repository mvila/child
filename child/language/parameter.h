#ifndef CHILD_LANGUAGE_PARAMETER_H
#define CHILD_LANGUAGE_PARAMETER_H

#include "child/pair.h"
#include "child/list.h"
#include "child/language/primitivechain.h"

CHILD_BEGIN

namespace Language {
    CHILD_POINTER_DECLARE(Parameter, Pair);

    #define CHILD_PARAMETER(ARGS...) \
    Language::ParameterPointer(new Language::Parameter(Node::context()->child("Object", "Language", "Parameter"), ##ARGS))

    class Parameter : public GenericPair<ParameterPointer, QString, PrimitiveChainPointer> {
        CHILD_DECLARE(Parameter, Pair);
    public:
        Parameter(const Pointer &origin, const QString &label = NULL,
                  const PrimitiveChainPointer &defaultValue = NULL) :
            GenericPair<ParameterPointer, QString, PrimitiveChainPointer>(origin, label, defaultValue) {}

        static void initRoot() { Language::root()->addChild("Parameter", root()); }

        virtual Pointer fork() const {
            return new Parameter(this, label(), forkIfNotNull(defaultValue()));
        }

        // aliases...
        QString label() const { return key(); }
        void setLabel(const QString &label) { setKey(label); }
        PrimitiveChainPointer defaultValue() const { return value(); }
        void setDefaultValue(const PrimitiveChainPointer &defaultValue) { setValue(defaultValue); }

        virtual Pointer run(const Pointer &receiver = context()) {
            return defaultValue()->run(receiver);
        }

        virtual QString toString(bool debug = false, short level = 0) const {
            return concatenateStrings(label(),
                                      ": ",
                                      defaultValue() ? defaultValue()->toString(debug, level) : "");
        }
    };

    CHILD_POINTER_DEFINE(Parameter, Pair);

    // === ParameterList ===

    CHILD_POINTER_DECLARE(ParameterList, List);

    #define CHILD_PARAMETER_LIST(ARGS...) \
    Language::ParameterListPointer(new Language::ParameterList( \
        Node::context()->child("Object", "Language", "ParameterList"), ##ARGS))

    class ParameterList : public GenericList<ParameterListPointer, ParameterPointer> {
        CHILD_DECLARE(ParameterList, List);
    public:
        ParameterList(const Pointer &origin) : GenericList<ParameterListPointer, ParameterPointer>(origin) {}

        static void initRoot() { Language::root()->addChild("ParameterList", root()); }
        virtual Pointer fork() const { return ParameterListPointer(new ParameterList(this))->initFork(); }

        virtual QString toString(bool debug = false, short level = 0) const { return join(", ", "", "", debug, level); }
    };

    CHILD_POINTER_DEFINE(ParameterList, List);
}

CHILD_END

#endif // CHILD_LANGUAGE_PARAMETER_H
