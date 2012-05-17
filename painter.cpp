/*
 *
 *  Copyright (C) 2012 Stephan Reinhard <Stephan-Reinhard@gmx.de>
 *
 *  This file is part of grading
 *
 *  grading program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "painter.h"

#include <QPainter>
#include <QPainterPath>
#include <QBrush>



qreal Painter::drawBlockTextLine(QPainter &p, QStringList &words, QRectF &place, qreal freeSpace)
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




void Painter::drawBlockText(QPainter &p, const QString &text, QRectF place, const QFont & font)
{
        qreal spaceSize = p.boundingRect(QRectF(), Qt::AlignLeft, " ").width();
        qreal spaceLeft = place.width();
        qreal lineWidth = 0;
        qreal wordWidth;
        qreal wordHeight;
        QString word;
        QStringList lineWords;

        p.setFont(font);

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




void Painter::drawCheck(QPainter &p, const QPoint &pos, const int size)
{

        QPainterPath path;
        path.moveTo(QPointF(-0.30,  0.00) * size);
        path.quadTo(QPointF(-0.10,  0.00) * size, QPointF( 0.00,  0.15) * size);
        path.quadTo(QPointF( 0.15, -0.20) * size, QPointF( 0.50, -0.40) * size);
        path.quadTo(QPointF( 0.10, -0.10) * size, QPointF( 0.00,  0.40) * size);
        path.quadTo(QPointF( 0.00,  0.10) * size, QPointF(-0.30,  0.00) * size);
        path.translate(pos);

        p.fillPath(path, QBrush(Qt::black));
}



