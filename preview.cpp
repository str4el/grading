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


#include "preview.h"

#include <QPainter>
#include "painter.h"

Preview::Preview(QWidget *parent) :
        QWidget(parent),
        mWindow(0,0, 2100, 2970)
{
}




void Preview::resizeEvent(QResizeEvent *event)
{
        mZoom = qMin(qreal(width()) / mWindow.width(), qreal(height()) / mWindow.height());
        mOffset.setX((width()  - mWindow.width()  * mZoom) / 2);
        mOffset.setY((height() - mWindow.height() * mZoom) / 2);

}




void Preview::paintEvent(QPaintEvent *event)
{
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setViewport(mOffset.rx(), mOffset.ry(), mWindow.width() * mZoom, mWindow.height() * mZoom);
        p.setWindow(mWindow);

        p.fillRect(mWindow, Qt::white);
        p.drawRect(mWindow);


        if (mLayout) {
                foreach (QPoint point, mLayout->gradeSelectionPoints()) {
                        Painter::drawCheck(p, point * 10, 50);
                }

                QRectF textRect(mLayout->assessmentTextRect().topLeft() * 10, mLayout->assessmentTextRect().bottomRight() * 10);
                Painter::drawBlockText(p, mLayout->assessmentText(), textRect, mLayout->font());
        }
}
