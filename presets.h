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


#ifndef PRESETS_H
#define PRESETS_H

#include <QMap>
#include <QStringList>
#include <QRect>


/*!
* \class Presets
* \brief Meyers-Singleton Klasse, welches die Grundeinstellungen des Programms enthält
*
* \author Stephan Reinhard
*/

class Presets {

private:
        const QString mProgramVersion;

        QMap <QString, int> mGradeSelectionXPos;
        QMap <QString, int> mGradeSelectionYPos;
        const QRect mAssessmentTextRect;


        // Privater Standard- und Copykonstruktor verhindert neue Objekte
        Presets();
        Presets(const Presets &);
        Presets & operator=(const Presets &);

public:
        static Presets & instance (void)
        {
                static Presets mInstance;
                return mInstance;
        }

        inline QString programVersion(void) const { return mProgramVersion; }
        inline QStringList gradeSelectionXNames(void) const { return mGradeSelectionXPos.keys(); }
        inline QStringList gradeSelectionYNames(void) const { return mGradeSelectionYPos.keys(); }
        int gradeSelectionXPos(const QString & name) const;
        int gradeSelectionYPos(const QString & name) const;
        inline const QRect & assessmentTextRect(void) const { return mAssessmentTextRect; }

};


#endif // PRESETS_H
