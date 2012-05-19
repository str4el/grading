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
#include <QMouseEvent>
#include "painter.h"
#include "presets.h"

Preview::Preview(QWidget *parent) :
        QWidget(parent),
        mWindow(0,0, 794, 1123),
        mDpm(1.0),
        mAssessmentTextActive(false)

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
        mDpm = qreal(p.device()->logicalDpiX()) / 25.4;

        p.setRenderHint(QPainter::Antialiasing, true);
        p.setViewport(mOffset.rx(), mOffset.ry(), mWindow.width() * mZoom, mWindow.height() * mZoom);
        p.setWindow(mWindow);

        p.fillRect(mWindow, Qt::white);
        p.drawRect(mWindow);

        if (mLayout) {
                QPen mark(Qt::red);
                mark.setStyle(Qt::DashLine);
                p.setPen(mark);

                if (mAssessmentTextActive) {
                        QRectF rect = mLayout->assessmentTextRect();
                        p.drawRect(QRectF(rect.topLeft() * mDpm, rect.bottomRight() * mDpm));
                }

                if (!mActiveX.isEmpty()) {
                        qreal x = mLayout->gradeSelectionXPos(mActiveX);
                        x *= mDpm;
                        p.drawLine(x, mWindow.top(), x, mWindow.bottom());
                }

                if (!mActiveY.isEmpty()) {
                        qreal y = mLayout->gradeSelectionYPos(mActiveY);
                        y *= mDpm;
                        p.drawLine(mWindow.left(), y, mWindow.right(), y);
                }

                p.setPen(Qt::black);

                Painter ph;
                ph.setFactor(mDpm);
                foreach (QPoint point, mLayout->gradeSelectionPoints()) {
                        ph.drawCheck(p, point, 5);
                }

                p.setFont(mLayout->font());
                ph.drawBlockText(p, mLayout->assessmentText(), mLayout->assessmentTextRect());
        }
}




void Preview::mousePressEvent(QMouseEvent *event)
{
        event->accept();

        if (!mLayout) {
                return;
        }

        if (event->button() == Qt::RightButton) {
                emit deactivated();
                mAssessmentTextActive = false;
                mActiveX.clear();
                mActiveY.clear();
                update();
                return;
        }

        if (event->button() == Qt::LeftButton) {
                QPoint mousePos = (event->pos() - mOffset) / mZoom / mDpm;

                mActiveX.clear();
                mActiveY.clear();
                mAssessmentTextActive = false;

                if (mLayout->assessmentTextRect().contains(mousePos)) {
                        mAssessmentTextActive = true;
                        emit assessmentTextActivated();
                        update();
                        return;
                }


                foreach(QString grade, Presets::instance().gradeSelectionXNames()) {
                        int pos = mLayout->gradeSelectionXPos(grade);
                        if (mousePos.x() > pos - 5 && mousePos.x() < pos + 5) {
                                mActiveX = grade;
                        }
                }

                if (mActiveX.isEmpty()) {
                        emit deactivated();
                        update();
                        return;
                }

                foreach(QString grade, Presets::instance().gradeSelectionYNames()) {
                        int pos = mLayout->gradeSelectionYPos(grade);
                        if (mousePos.y() > pos - 5 && mousePos.y() < pos + 5) {
                                mActiveY = grade;
                        }
                }

                if (mActiveY.isEmpty()) {
                        emit deactivated();
                        mActiveX.clear();
                        update();
                        return;
                }

                emit gradeActivated(mActiveY, mActiveX);
                update();
        }
}
