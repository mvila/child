#include "child/language/interpreter.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Interpreter, Object);

    void Interpreter::initRoot() {
        Language::root()->addChild("Interpreter", this);
        initOperatorTable();
        _lexer = CHILD_LEXER();
        addChild("lexer", _lexer);
        _parser = CHILD_PARSER();
        addChild("parser", _parser);
        _sourceCodes = CHILD_SOURCE_CODE_DICTIONARY();
        addChild("source_codes", _sourceCodes);
    }

    void Interpreter::initOperatorTable() {
        OperatorTable *ops = CHILD_OPERATOR_TABLE();

        ops->append("++", Operator::Postfix, Operator::namePrecedence, Operator::LeftAssociative, true, false, "postfix++");
        ops->append("--", Operator::Postfix, Operator::namePrecedence, Operator::LeftAssociative, true, false, "postfix--");

        ops->append(".", Operator::Binary, Operator::namePrecedence);
        ops->append("...", Operator::Postfix, Operator::namePrecedence, Operator::NonAssociative, true, true);

        ops->append("\\", Operator::Prefix, 531, Operator::NonAssociative, true, true);
        ops->append("@", Operator::Prefix, 541, Operator::NonAssociative, true, true);
        ops->append("#", Operator::Prefix, 511);
        ops->append("$", Operator::Prefix, 511);

        ops->append("++", Operator::Prefix, 521, Operator::LeftAssociative, true, false, "prefix++");
        ops->append("--", Operator::Prefix, 521, Operator::LeftAssociative, true, false, "prefix--");

        ops->append("+", Operator::Prefix, 511, Operator::RightAssociative, true, false, "unary+");
        ops->append("-", Operator::Prefix, 511, Operator::RightAssociative, true, false, "unary-");
        ops->append("!", Operator::Prefix, 511, Operator::RightAssociative);

        ops->append("+", Operator::Binary, 451);
        ops->append("-", Operator::Binary, 451);
        ops->append("*", Operator::Binary, 461);
        ops->append("/", Operator::Binary, 461);
        ops->append("%", Operator::Binary, 461);

        ops->append("&", Operator::Binary, 461);
        ops->append("|", Operator::Binary, 451);
        ops->append("^", Operator::Binary, 451);
        ops->append("<<", Operator::Binary, 461);
        ops->append(">>", Operator::Binary, 461);

        ops->append("..", Operator::Binary, 451);

        ops->append("<=>", Operator::Binary, 441);

        ops->append("==", Operator::Binary, 431, Operator::NonAssociative);
        ops->append("!=", Operator::Binary, 431, Operator::NonAssociative);
        ops->append("<", Operator::Binary, 431, Operator::NonAssociative);
        ops->append(">", Operator::Binary, 431, Operator::NonAssociative);
        ops->append("<=", Operator::Binary, 431, Operator::NonAssociative);
        ops->append(">=", Operator::Binary, 431, Operator::NonAssociative);

        ops->append("&&", Operator::Binary, 421);
        ops->append("||", Operator::Binary, 411);

        ops->append(",", Operator::Binary, 321, Operator::LeftAssociative, false, true);
        ops->append("->", Operator::Binary, 311, Operator::LeftAssociative, false, true);

        ops->append("*=", Operator::Binary, 261, Operator::RightAssociative, false);
        ops->append("/=", Operator::Binary, 261, Operator::RightAssociative, false);
        ops->append("%=", Operator::Binary, 261, Operator::RightAssociative, false);
        ops->append("<<=", Operator::Binary, 261, Operator::RightAssociative, false);
        ops->append(">>=", Operator::Binary, 261, Operator::RightAssociative, false);
        ops->append("+=", Operator::Binary, 251, Operator::RightAssociative, false);
        ops->append("-=", Operator::Binary, 251, Operator::RightAssociative, false);
        ops->append("^=", Operator::Binary, 251, Operator::RightAssociative, false);
        ops->append("&=", Operator::Binary, 241, Operator::RightAssociative, false);
        ops->append("|=", Operator::Binary, 231, Operator::RightAssociative, false);
        ops->append("&&=", Operator::Binary, 221, Operator::RightAssociative, false);
        ops->append("||=", Operator::Binary, 211, Operator::RightAssociative, false);
        ops->append("=", Operator::Binary, 206, Operator::RightAssociative, false);
        ops->append(":=", Operator::Binary, 201, Operator::RightAssociative, false);

        ops->append("?:", Operator::Prefix, 121, Operator::NonAssociative, false, true);

        ops->append(":", Operator::Binary, 111, Operator::NonAssociative, false, true);

        _operatorTable = ops;
        addChild("operatorTable", _operatorTable);
    }

    SourceCode *Interpreter::loadSourceCode(QString url) {
        url = QFileInfo(url).absoluteFilePath();
        SourceCode *source;
        if(!(source = sourceCodeIsAlreadyLoaded(url))) {
            source = CHILD_SOURCE_CODE(url);
            sourceCodes()->set(CHILD_TEXT(url), source);
            source->parse();
        }
        return source;
    }

    SourceCode *Interpreter::sourceCodeIsAlreadyLoaded(QString url) {
        url = QFileInfo(url).absoluteFilePath();
        return sourceCodes()->hasKey(CHILD_TEXT(url));
    }
}

CHILD_END