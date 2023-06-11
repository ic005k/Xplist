#ifndef MYHIGHLIGHTER_H
#define MYHIGHLIGHTER_H

#include <QString>

#if QT_VERSION_MAJOR >= 6
#include <QRegularExpression>
#else
#include <QRegExp>
#endif

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>

class MyHighLighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    MyHighLighter(QTextDocument* parent = 0);

protected:
    void highlightBlock(const QString& text) Q_DECL_OVERRIDE;

private:
    struct HighlightingRule {
#if QT_VERSION_MAJOR >= 6
        QRegularExpression pattern;
#else
        QRegExp pattern;
#endif
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

#if QT_VERSION_MAJOR >= 6
    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
#else
    QRegExp commentStartExpression;
    QRegExp commentEndExpression;
#endif

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineKey;
    QTextCharFormat singleLineValue;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};
#endif // MYHIGHLIGHTER_H
