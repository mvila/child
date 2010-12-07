#ifndef CHILD_TOKEN_H
#define CHILD_TOKEN_H

#include "child/language.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(Token, Object);

    #define CHILD_TOKEN(ARGS...) \
    TokenPtr(new Token(Node::findInContext("Object")->child("Language")->child("Token"), ##ARGS))

    class Token : public Object {
        CHILD_DECLARATION(Token, Object);
    public:
        enum Type {
            Null,
            Eof,
            Name,
            Label,
            Operator,
            Boolean,
            Number,
            Character,
            Text,
            LeftParenthesis,
            RightParenthesis,
            LeftBracket,
            RightBracket,
            LeftBrace,
            RightBrace,
            Semicolon,
            Newline
        };

        static const char *typeNames[];

        Type type;
        QStringRef sourceCodeRef;

        Token(const NodePtr &origin, const Type type = Null, const QStringRef &sourceCodeRef = QStringRef()) :
            Object(origin), type(type), sourceCodeRef(sourceCodeRef) {}

        static void initRoot() { Language::root()->addChild("Token", root()); }
        virtual NodePtr fork() const { return new Token(this, type, sourceCodeRef); }

        const QString typeName() const { return Token::typeName(type); }
        const QString text() const { return sourceCodeRef.toString(); }

        static const QString typeName(const Type type) { return typeNames[type]; }

        virtual const QString toString(bool debug = false) const {
            #pragma unused(debug)
            return QString("%1: '%2'").arg(typeName(), text());
        }
    };

    CHILD_PTR_DEFINITION(Token, Object);
}

CHILD_END

#endif // CHILD_TOKEN_H
