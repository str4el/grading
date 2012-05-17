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
        QObject(parent)
{
        mAssessmentTextRect = Presets::instance().assessmentTextRect();
}




void Layout::load(QSettings &settings)
{
        settings.beginGroup("layout");
        mGradeSelectionXPos = settings.value("grade_selection_x_position").toHash();
        mGradeSelectionYPos = settings.value("grade_selection_y_position").toHash();
        mAssessmentTextRect = settings.value("assessment_text_rect").toRect();
        settings.endGroup();
}





void Layout::save(QSettings &settings)
{
        settings.beginGroup("layout");
        settings.setValue("grade_selection_x_position", mGradeSelectionXPos);
        settings.setValue("grade_selection_y_position", mGradeSelectionYPos);
        settings.setValue("assessment_text_rect", mAssessmentTextRect);
        settings.endGroup();
}





int Layout::gradeSelectionXPos(const QString &name) const
{
        if (mGradeSelectionXPos.contains(name)) {
                return mGradeSelectionXPos[name].toInt();
        }

        return Presets::instance().gradeSelectionXPos(name);
}




int Layout::gradeSelectionYPos(const QString &name) const
{
        if (mGradeSelectionYPos.contains(name)) {
                return mGradeSelectionYPos[name].toInt();
        }

        return Presets::instance().gradeSelectionYPos(name);
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
        mGradeSelectionXPos[name].setValue(pos);
        emit changed();
}




void Layout::setGradeSelectionYPos(const QString &name, const int pos)
{
        mGradeSelectionYPos[name].setValue(pos);
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
