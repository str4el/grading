#ifndef BUILDER_H
#define BUILDER_H

#include <QObject>
#include <QString>
#include <QRect>
#include <QFont>

class QPainter;

class Builder : public QObject
{
        Q_OBJECT

private:
        QFont mFont;
        QString mText;
        QRect mTextRect;

        static qreal drawBlockTextLine(QPainter &p, QStringList &words, QRectF &place, qreal freeSpace);

public:
        explicit Builder(QObject *parent = 0);
        bool paint (QPainter &p);

        static void drawBlockText(QPainter &p, const QString &text, QRectF place, const QFont & font = QFont());
        static void drawCheck(QPainter &p, const QPoint & pos, const int size);

        // getter
        QFont font (void) const { return mFont; }
        QString text (void) const { return mText; }
        QRect textRect (void) const { return mTextRect; }

public slots:
        // setter
        void setFont (const QFont & f) { mFont = f; }
        void setText (const QString & t) { mText = t; }
        void setTextRect (const QRect &  r) { mTextRect = r; }

};

#endif // BUILDER_H
