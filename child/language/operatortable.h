#ifndef CHILD_OPERATORTABLE_H
#define CHILD_OPERATORTABLE_H

#include "child/object.h"
#include "child/list.h"
#include "child/dictionary.h"
#include "child/language.h"
#include "child/language/operator.h"

namespace Child {
    namespace Language {
        class OperatorTable : public Object {
            CHILD_DECLARATION(OperatorTable, Object, Language);
        public:
            OperatorTable() : _operators(NULL) {}

            virtual ~OperatorTable() {
                delete _operators;
            }

            Dictionary *operators() const {
                return _operators;
            }

            void append(const QString &text, Operator::Type type, short precedence,
                             Operator::Associativity associativity = Operator::LeftAssociative,
                             const QString &name = "");

            Operator *find(const QString &text, const Operator::Type type) const;

            bool hasOperator(const QString &text) const {
                return operators()->hasKey(text);
            }

            bool hasOperatorStartingWith(const QChar c) const {
                return _firstChars.contains(c);
            }

            virtual const QString inspect() const { return operators()->inspect(); }
        private:
            Dictionary *_operators;
            QString _firstChars;
        };
    }
}

#endif // CHILD_OPERATORTABLE_H