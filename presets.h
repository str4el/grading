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


#ifndef PRESETS_H
#define PRESETS_H

#include <QHash>
#include <QStringList>
#include <QRect>
#include <QFont>


/*!
* \class Presets
* \brief Meyers-Singleton Klasse, welches die Grundeinstellungen des Programms enthält
*
* \author Stephan Reinhard
*/

class Presets {

private:
        const QString mProgramName;
        const QString mProgramVersion;
        const int mFileVersion;

        QHash <QString, QString> mDomainCaption;
        QHash <QString, QString> mGradeCaption;

        QHash <QString, int> mGradeSelectionXPos;
        QHash <QString, int> mGradeSelectionYPos;
        QHash <QString, QString> mAssessmentTextBlock;
        const QRect mAssessmentTextRect;
        const QFont mFont;


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

        inline QString programName (void) const { return mProgramName; }
        inline QString programVersion(void) const { return mProgramVersion; }
        inline int fileVersion (void) const { return mFileVersion; }

        QString domainCaption (const QString & domain) const;
        QString gradeCaption (const QString & grade) const;
        inline QStringList domainNames (void) const { return mDomainCaption.keys(); }
        inline QStringList gradeNames (void) const { return mGradeCaption.keys(); }

        inline QStringList gradeSelectionXNames(void) const { return mGradeSelectionXPos.keys(); }
        inline QStringList gradeSelectionYNames(void) const { return mGradeSelectionYPos.keys(); }
        int gradeSelectionXPos(const QString & name) const;
        int gradeSelectionYPos(const QString & name) const;
        QString assessmentTextBlock(const QString & name) const;
        inline const QRect & assessmentTextRect(void) const { return mAssessmentTextRect; }
        inline const QFont & font (void) const { return mFont; }

};


#endif // PRESETS_H
