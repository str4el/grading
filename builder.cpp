#include "builder.h"

#include <QPainter>
#include <QFontDialog>

#include <QDebug>

Builder::Builder(QObject *parent) :
        QObject(parent)
{
}




bool Builder::paint(QPainter &p)
{
        qDebug() << p.viewport() << p.window();

        //p.setFont(QFontDialog::getFont(0, p.font()));
        drawBlockText(p, mText, mTextRect);
        return true;
}




qreal Builder::drawBlockTextLine(QPainter &p, QStringList &words, QRectF &place, qreal freeSpace)
{
        QRectF boundingRect(place.topLeft(), place.bottomLeft());

        if (words.count() < 2) {
                p.drawText(place, Qt::AlignLeft, words.join(" "), &boundingRect);
        } else {

                qreal space = freeSpace / (words.count() - 1);

                foreach (QString word, words) {
                        p.drawText(QRectF(boundingRect.topRight(), place.bottomRight()), Qt::AlignLeft, word, &boundingRect);
                        boundingRect.setRight(boundingRect.right() + space);
                }
        }

        return boundingRect.height();
}




void Builder::drawBlockText(QPainter &p, const QString &text, QRectF place)
{
        qreal spaceSize = p.boundingRect(QRectF(), Qt::AlignLeft, " ").width();
        qreal spaceLeft = place.width();
        qreal lineWidth = 0;
        qreal wordWidth;
        qreal wordHeight;
        QString word;
        QStringList lineWords;

        foreach (word, text.split(' ')) {
                wordWidth = p.boundingRect(QRectF(), Qt::AlignLeft, word).width();

                if (lineWidth + spaceSize + wordWidth < place.width()) {
                        lineWidth += wordWidth + spaceSize;
                        spaceLeft -= wordWidth;
                } else {
                        wordHeight = drawBlockTextLine(p, lineWords, place, spaceLeft);
                        place.setTop(place.top() + wordHeight + spaceSize);

                        lineWords.clear();
                        lineWidth = wordWidth;
                        spaceLeft = place.width() - wordWidth;
                }
                lineWords.append(word);
        }
        p.drawText(place, Qt::AlignLeft, lineWords.join(" "));

}



