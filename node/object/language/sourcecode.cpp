#include "node/object/language/sourcecode.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(SourceCode, Object, Language);

    SourceCode::SourceCode(Node *origin, const QString &theUrl, const QString &txt, Block *block) :
        Object(origin), _text(txt), _block(block) {
        setUrl(theUrl);
        if(!url().isEmpty() && txt.isEmpty()) load();
    }

    void SourceCode::initRoot() {
    }

    void SourceCode::load(const QString &newUrl) {
        if(!newUrl.isEmpty()) setUrl(newUrl);
        if(url().isEmpty()) CHILD_THROW(ArgumentException, "URL is empty");
        if(url().startsWith("file://"))
            setText(readTextFile(url().mid(7)));
        else if(url().startsWith("child:"))
            setText(url().mid(6));
        else
            CHILD_TODO;
    }

    void SourceCode::parse(const QString &newText) {
        if(!newText.isEmpty()) setText(newText);
        Parser *parser = Parser::cast(child("parser")); // search a Parser in parents
        setBlock(parser->parse(text(), url()));
    }

    QString SourceCode::toString(bool debug, short level) const {
        QString str;
        if(!url().isEmpty()) str += QString("/* %1 */\n").arg(url());
        if(block()) str += block()->toString(debug, level);
        return str;
    }
}

CHILD_END
