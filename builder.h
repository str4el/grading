#ifndef BUILDER_H
#define BUILDER_H

#include <QObject>
#include <QString>
#include <QRect>

class QPainter;

class Builder : public QObject
{
        Q_OBJECT

private:
        QString mText;
        QRect mTextRect;

        void drawBlockText(QPainter &p, const QString &text, QRectF place);
        qreal drawBlockTextLine(QPainter &p, QStringList &words, QRectF &place, qreal freeSpace);

public:
        explicit Builder(QObject *parent = 0);
        bool paint (QPainter &p);

        // getter
        QString text (void) const { return mText; }
        QRect textRect (void) const { return mTextRect; }

public slots:
        // setter
        void setText (const QString & t) { mText = t; }
        void setTextRect (const QRect &  r) { mTextRect = r; }

};

#endif // BUILDER_H
