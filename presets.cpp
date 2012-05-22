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
        mProgramVersion("0.5.1"),
        mFileVersion(3),
        mAssessmentTextRect(20, 160, 175, 35),
        mFont("Sans Serif" ,12)
{
        // Standardpositionen
        mGradeSelectionXPos.insert("VeryGood",       142);
        mGradeSelectionXPos.insert("Good",           154);
        mGradeSelectionXPos.insert("Normal",         167);
        mGradeSelectionXPos.insert("Bad",            180);
        mGradeSelectionXPos.insert("VeryBad",        193);

        mGradeSelectionYPos.insert("Knowledge",       93);
        mGradeSelectionYPos.insert("Skills",          98);
        mGradeSelectionYPos.insert("Safety",         103);
        mGradeSelectionYPos.insert("Reliability",    120);
        mGradeSelectionYPos.insert("Activity",       125);
        mGradeSelectionYPos.insert("ProperHandling", 129);
        mGradeSelectionYPos.insert("Teamwork",       139);
        mGradeSelectionYPos.insert("Responsibility", 144);
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


