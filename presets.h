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
* \class Presets2
* \brief Meyers-Singleton Klasse, welches die Grundeinstellungen des Programms enthält
*
* \author Stephan Reinhard
*/

class Presets2 {

private:
        const QString mProgramVersion;

        QMap <QString, int> mGradeSelectionXPos;
        QMap <QString, int> mGradeSelectionYPos;
        const QRect mAssessmentTextRect;


        // Privater Standard- und Copykonstruktor verhindert neue Objekte
        Presets2();
        Presets2(const Presets2 &);
        Presets2 & operator=(const Presets2 &);

public:
        static Presets2 & instance (void)
        {
                static Presets2 mInstance;
                return mInstance;
        }

        inline QString programVersion(void) const { return mProgramVersion; }
        inline QStringList gradeSelectionXNames(void) const { return mGradeSelectionXPos.keys(); }
        inline QStringList gradeSelectionYNames(void) const { return mGradeSelectionYPos.keys(); }
        int gradeSelectionXPos(const QString & name) const;
        int gradeSelectionYPos(const QString & name) const;
        inline const QRect & assessmentTextRect(void) const { return mAssessmentTextRect; }

};


#include <QString>

class Presets {

private:
        static QString mVersion;
        static int mTopOffset;
        static int mLeftOffset;
        static int mWidth;
        static int mTickOffset[5];
        static int mTopToTick;
        static int mTickToTick[2];
        static int mTickToText;

public:
        static QString version() { return mVersion; }
        static int topOffset() { return mTopOffset; }
        static int leftOffset() { return mLeftOffset; }
        static int width() { return mWidth; }
        static int topToTick() { return mTopToTick; }
        static int tickToText() { return mTickToText; }

        static int tickOffset(int n)
        {
                if (n <= 0 || n > 5) return 0;
                return mTickOffset[n - 1];
        }

        static int tickToTick(int n)
        {
                if (n <= 0 || n > 2) return 0;
                return mTickToTick[n - 1];
        }

        static QString saveDir();


};


#endif // PRESETS_H
