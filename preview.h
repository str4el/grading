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


#ifndef PREVIEW_H
#define PREVIEW_H

#include <QWidget>
#include <QPointer>

#include "layout.h"


class QPainter;
class QRect;

class Preview : public QWidget
{
        Q_OBJECT

private:
        QRect mWindow;
        QPoint mOffset;
        qreal mZoom;

        QPointer <Layout> mLayout;

protected:
        void resizeEvent(QResizeEvent *event);
        void paintEvent(QPaintEvent *event);


public:
        explicit Preview(QWidget *parent = 0);

        void setLayout (Layout *l) { mLayout = l; }


};

#endif // PREVIEW_H
