/*
 *
 *  Copyright (C) 2010 Wolfgang Forstmeier <wolfgang.forstmeier@gmail.com>
 *  Copyright (C) 2010 - 2012 Stephan Reinhard <stephan-reinhard@gmx.de>
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

#include "presets.h"

#include <QString>
#include <QRect>


Presets::Presets () :
        mProgramName("Grading"),
        mProgramVersion("0.5.0"),
        mFileVersion(3),
        mAssessmentTextRect(10, 200, 150, 50),
        mFont("Sans Serif" ,12)
{
        // Standardpositionen
        mGradeSelectionXPos.insert("VeryGood",       138);
        mGradeSelectionXPos.insert("Good",           151);
        mGradeSelectionXPos.insert("Normal",         164);
        mGradeSelectionXPos.insert("Bad",            177);
        mGradeSelectionXPos.insert("VeryBad",        190);

        mGradeSelectionYPos.insert("Knowledge",       75);
        mGradeSelectionYPos.insert("Skills",          85);
        mGradeSelectionYPos.insert("Safety",          95);
        mGradeSelectionYPos.insert("Reliability",    115);
        mGradeSelectionYPos.insert("Activity",       125);
        mGradeSelectionYPos.insert("ProperHandling", 135);
        mGradeSelectionYPos.insert("Teamwork",       150);
        mGradeSelectionYPos.insert("Responsibility", 160);
}




int Presets::gradeSelectionXPos(const QString &name) const
{
        if (mGradeSelectionXPos.contains(name)) {
                return mGradeSelectionXPos[name];
        }

        return 0;
}




int Presets::gradeSelectionYPos(const QString &name) const
{
        if (mGradeSelectionYPos.contains(name)) {
                return mGradeSelectionYPos[name];
        }

        return 0;
}


