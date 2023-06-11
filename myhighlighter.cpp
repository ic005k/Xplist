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
#if QT_VERSION_MAJOR >= 6
        rule.pattern = QRegularExpression(pattern);
#else
        rule.pattern = QRegExp(pattern);
#endif
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    if (red > 55)
        quotationFormat.setForeground(Qt::blue);
    else
        quotationFormat.setForeground(Qt::green);
#if QT_VERSION_MAJOR >= 6
    rule.pattern = QRegularExpression("/");
#else
    rule.pattern = QRegExp("/");
#endif
    rule.format = quotationFormat;
    highlightingRules.append(rule);

#if QT_VERSION_MAJOR >= 6
    rule.pattern = QRegularExpression("<");
#else
    rule.pattern = QRegExp("<");
#endif
    rule.format = quotationFormat;
    highlightingRules.append(rule);

#if QT_VERSION_MAJOR >= 6
    rule.pattern = QRegularExpression(">");
#else
    rule.pattern = QRegExp(">");
#endif
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen);
    quotationFormat.setForeground(Qt::red);
#if QT_VERSION_MAJOR >= 6
    rule.pattern = QRegularExpression("#");
#else
    rule.pattern = QRegExp("#");
#endif
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
#if QT_VERSION_MAJOR >= 6
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
#else
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
#endif
    rule.format = functionFormat;
    highlightingRules.append(rule);
#if QT_VERSION_MAJOR >= 6
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
#else
    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
#endif
}

void MyHighLighter::highlightBlock(const QString& text)
{
    foreach (const HighlightingRule& rule, highlightingRules) {
#if QT_VERSION_MAJOR >= 6
        QRegularExpression expression(rule.pattern);
        QRegularExpressionMatch expmatch = expression.match(text);
        int index = expmatch.lastCapturedIndex();
        while (index >= 0) {
            int length = expmatch.capturedLength(index);
            setFormat(index, length, rule.format);
            index = expmatch.capturedLength(index + length);
        }
#else
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
#endif
    }

    setCurrentBlockState(0);

    int startIndex = 0;
  
#if QT_VERSION_MAJOR >= 6
    QRegularExpressionMatch cSEM = commentStartExpression.match(text);
    QRegularExpressionMatch cEEM = commentEndExpression.match(text);

    if (previousBlockState() != 1)
        startIndex = cSEM.capturedStart(text);

    while (startIndex >= 0) {
        int endIndex = cEEM.capturedEnd(text);
#else
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
#endif

        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
#if QT_VERSION_MAJOR >= 6
            commentLength = endIndex - startIndex
                + cEEM.lastCapturedIndex();
#else
            commentLength = endIndex - startIndex
                + commentEndExpression.matchedLength();
#endif
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
#if QT_VERSION_MAJOR >= 6
        startIndex = cSEM.capturedEnd(startIndex);
#else
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
#endif
    }
}
