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


#ifndef PAINTER_H
#define PAINTER_H

#include <QtGlobal>

class QPoint;
class QPainter;
class QString;
class QStringList;
class QRectF;
class QFont;

class Painter
{

private:
        qreal mFactor;

        static qreal drawBlockTextLine(QPainter &p, QStringList &words, QRectF &place, qreal freeSpace);

public:
        Painter();

        void setFactor (const qreal factor) { mFactor = factor; }
        void drawBlockText(QPainter &p, const QString &text, const QRectF & place);
        void drawCheck(QPainter &p, const QPoint & pos, const int size);

};

#endif // PAINTER_H
