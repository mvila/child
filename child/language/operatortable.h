#ifndef CHILD_OPERATORTABLE_H
#define CHILD_OPERATORTABLE_H

#include <QtCore/QSet>

#include "child/exception.h"
#include "child/list.h"
#include "child/language/operator.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(OperatorTable, List);

    #define CHILD_OPERATOR_TABLE(ARGS...) \
    OperatorTablePtr(new OperatorTable(Node::findInContext("Object")->child("Language")->child("OperatorTable"), ##ARGS))

    class OperatorTable : public GenericList<OperatorTablePtr, OperatorPtr> {
        CHILD_DECLARATION(OperatorTable, List);
    public:
        OperatorTable(const NodePtr &origin) : GenericList<OperatorTablePtr, OperatorPtr>(origin),
            _texts(NULL), _textsAndTypes(NULL) {}

        static void initRoot() { Language::root()->addChild("OperatorTable", root()); }
        virtual NodePtr fork() const { CHILD_TODO; return new OperatorTable(this); }

        virtual ~OperatorTable() {
            delete _texts;
            delete _textsAndTypes;
        }

        void append(const QString &text, Operator::Type type, short precedence,
                         Operator::Associativity associativity = Operator::LeftAssociative,
                         const QString &name = "") {
            GenericList<OperatorTablePtr, OperatorPtr>::append(
                        CHILD_OPERATOR(text, type, precedence, associativity, name));
        }

//        Operator *find(const QString &text, const Operator::Type type) const;

//        bool hasOperator(const QString &text) const {
//            return operators()->hasKey(text);
//        }

//        bool hasOperatorStartingWith(const QChar c) const {
//            return _firstChars.contains(c);
//        }

        virtual void hasChanged() {
            if(_texts) _texts->clear(); else _texts = new QSet<QString>;
            if(_textsAndTypes)
                _textsAndTypes->clear();
            else
                _textsAndTypes = new QHash<QPair<QString, Operator::Type>, OperatorPtr>;
            Iterator i(this);
            while(OperatorPtr op = i.next()) {
                _texts->insert(op->text);
                _textsAndTypes->insert(QPair<QString, Operator::Type>(op->text, op->type), op);
                if(!_firstChars.contains(op->text.at(0))) _firstChars += op->text.at(0);
            }
        }
    private:
        QSet<QString> *_texts;
        QHash<QPair<QString, Operator::Type>, OperatorPtr> *_textsAndTypes;
        QString _firstChars;
    };

    CHILD_PTR_DEFINITION(OperatorTable, List);
}

CHILD_END

#endif // CHILD_OPERATORTABLE_H
