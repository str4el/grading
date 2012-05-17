/*
 *
 *  Copyright (C) 2010 Wolfgang Forstmeier <wolfgang.forstmeier@gmail.com>
 *  Copyright (C) 2010 Stephan Reinhard <stephan-reinhard@gmx.de>
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


Presets2::Presets2 () :
        mProgramVersion("0.5.0"),
        mAssessmentTextRect(10, 200, 150, 50)
{
        // Standardpositionen
        mGradeSelectionXPos.insert("VeryGood",       138);
        mGradeSelectionXPos.insert("Good",           151);
        mGradeSelectionXPos.insert("Normal",         164);
        mGradeSelectionXPos.insert("Bad",            177);
        mGradeSelectionXPos.insert("VeryBad",        190);

        mGradeSelectionYPos.insert("Knowledge",       75);
        mGradeSelectionYPos.insert("Skills",          85);
        mGradeSelectionYPos.insert("Savety",          95);
        mGradeSelectionYPos.insert("Reliability",    115);
        mGradeSelectionYPos.insert("Activity",       125);
        mGradeSelectionYPos.insert("ProperHandling", 135);
        mGradeSelectionYPos.insert("Teamwork",       150);
        mGradeSelectionYPos.insert("Responsibility", 160);
}




int Presets2::gradeSelectionXPos(const QString &name) const
{
        if (mGradeSelectionXPos.contains(name)) {
                return mGradeSelectionXPos[name];
        }

        return 0;
}




int Presets2::gradeSelectionYPos(const QString &name) const
{
        if (mGradeSelectionYPos.contains(name)) {
                return mGradeSelectionYPos[name];
        }

        return 0;
}




#include <QProcessEnvironment>
#include <QSettings>

QString Presets::mVersion = "0.4.5";

/* Default positions
 */
int Presets::mTopOffset = 15;
int Presets::mLeftOffset = 14;
int Presets::mWidth = 180;
int Presets::mTickOffset[5] = { 122, 135, 147, 160, 173 };
int Presets::mTopToTick = 60;
int Presets::mTickToTick[2] = { 12, 5 };
int Presets::mTickToText = 10;


QString Presets::saveDir()
{
        QString path;
#ifdef Q_OS_LINUX
        path = QProcessEnvironment::systemEnvironment().value("HOME");
#endif

#ifdef Q_OS_WIN32
        QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", QSettings::NativeFormat);
        path = settings.value("Personal").toString();
#endif
        if (path.isEmpty()) {
                path = ".";
        }
        path += "/";

        return path;
}


