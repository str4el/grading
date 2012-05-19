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


#include "layout.h"

#include "presets.h"

#include <QSettings>

Layout::Layout(QObject *parent) :
        QObject(parent),
        mXOffset(0),
        mYOffset(0)
{
        setDefaults();
}




void Layout::setDefaults()
{
        mAssessmentTextRect = Presets::instance().assessmentTextRect();
        mGradeSelectionXPos.clear();
        mGradeSelectionYPos.clear();
        mFont = Presets::instance().font();
}


#include <QDebug>

void Layout::load(QSettings &settings)
{
        settings.beginGroup("layout");

        QString str = settings.value("assessment_text_position").toString();
        bool ok1, ok2, ok3, ok4;

        mAssessmentTextRect.setLeft   (str.section(',', 0, 0).toInt(&ok1));
        mAssessmentTextRect.setTop    (str.section(',', 1, 1).toInt(&ok2));
        mAssessmentTextRect.setWidth  (str.section(',', 2, 2).toInt(&ok3));
        mAssessmentTextRect.setHeight (str.section(',', 3, 3).toInt(&ok4));

        if (!(ok1 && ok2 && ok3 && ok4)) {
                mAssessmentTextRect = Presets::instance().assessmentTextRect();
        }



        str = settings.value("grade_x_position").toString();
        foreach (str, str.split(',')) {
                bool ok;
                int val = str.section(':', 1).toInt(&ok);
                if (ok) {
                        mGradeSelectionXPos[str.section(':', 0, 0).simplified()] = val;
                }
        }

        str = settings.value("grade_y_position").toString();
        foreach (str, str.split(',')) {
                bool ok;
                int val = str.section(':', 1).toInt(&ok);
                if (ok) {
                        mGradeSelectionYPos[str.section(':', 0, 0).simplified()] = val;
                }
        }

        QString fontFamily = settings.value("font_family").toString();
        int fontSize = settings.value("font_size", -1).toInt();

        mFont = Presets::instance().font();
        if (!fontFamily.isEmpty() && fontSize > 1) {
                mFont.setFamily(fontFamily);
                mFont.setPointSize(fontSize);
        }

        settings.endGroup();
}





void Layout::save(QSettings &settings)
{
        settings.beginGroup("layout");

        settings.setValue("assessment_text_position",
                          QString("%1, %2, %3, %4")
                          .arg(mAssessmentTextRect.left())
                          .arg(mAssessmentTextRect.top())
                          .arg(mAssessmentTextRect.width())
                          .arg(mAssessmentTextRect.height())
                          );


        QString str;
        foreach (QString key, mGradeSelectionXPos.keys()) {
                str.append(key + ": " + QString::number(mGradeSelectionXPos[key]) + ", ");
        }
        str.chop(1);
        settings.setValue("grade_x_position", str);

        str.clear();
        foreach (QString key, mGradeSelectionYPos.keys()) {
                str.append(key + ": " + QString::number(mGradeSelectionYPos[key]) + ", ");
        }
        str.chop(2);
        settings.setValue("grade_y_position", str);

        settings.setValue("font_family", mFont.family());
        settings.setValue("font_size", mFont.pointSize());

        settings.endGroup();
}





int Layout::gradeSelectionXPos(const QString &name) const
{
        if (mGradeSelectionXPos.contains(name)) {
                return mGradeSelectionXPos[name] + mXOffset;
        }

        return Presets::instance().gradeSelectionXPos(name) + mXOffset;
}




int Layout::gradeSelectionYPos(const QString &name) const
{
        if (mGradeSelectionYPos.contains(name)) {
                return mGradeSelectionYPos[name] + mYOffset;
        }

        return Presets::instance().gradeSelectionYPos(name) + mYOffset;
}




QPoint Layout::gradeSelectionPos(const QString &xName, const QString &yName) const
{
        QPoint ret;
        ret.setX(gradeSelectionXPos(xName));
        ret.setY(gradeSelectionYPos(yName));
        return ret;
}




void Layout::setGradeSelectionXPos(const QString &name, const int pos)
{
        mGradeSelectionXPos[name] = pos;
        emit changed();
}




void Layout::setGradeSelectionYPos(const QString &name, const int pos)
{
        mGradeSelectionYPos[name] = pos;
        emit changed();
}




QList <QPoint> Layout::gradeSelectionPoints() const
{
        QList <QPoint> points;

        foreach (QString grade, mGradeSelection.keys()) {
                points.append(gradeSelectionPos(mGradeSelection.value(grade), grade));
        }

        return points;
}
