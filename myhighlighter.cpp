#include "myhighlighter.h"

extern int red;

MyHighLighter::MyHighLighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    if (red > 55)
        keywordFormat.setForeground(Qt::blue);
    else
        keywordFormat.setForeground(Qt::green);

    //keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns
        << "\\barray\\b"
        << "\\bdict\\b"
        << "\\bdate\\b"
        << "\\breal\\b"
        << "\\bkey\\b"

        << "\\binteger\\b"
        << "\\bbool\\b"
        << "\\bstring\\b"
        << "\\bdata\\b"
        << "\\b<\\b"
        << "\\b>\\b"
        << "\\b/\\b";

    foreach (const QString& pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    if (red > 55)
        quotationFormat.setForeground(Qt::blue);
    else
        quotationFormat.setForeground(Qt::green);
    rule.pattern = QRegExp("/");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("<");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp(">");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen);
    quotationFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("#");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void MyHighLighter::highlightBlock(const QString& text)
{
    foreach (const HighlightingRule& rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
