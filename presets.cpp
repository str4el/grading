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
        mProgramVersion("0.7.1"),
        mFileVersion(4),
        mAssessmentTextRect(20, 160, 175, 35),
        mMeasuresTextRect(mAssessmentTextRect.left(), 210, mAssessmentTextRect.width(), 30),
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



        mDomainCaption.insert("Skills",   QString::fromUtf8("Fertigkeiten"));
        mDomainCaption.insert("Care",     QString::fromUtf8("Sorgfalt"));
        mDomainCaption.insert("Interest", QString::fromUtf8("Interesse"));
        mDomainCaption.insert("Teamwork", QString::fromUtf8("Zusammenarbeit"));
        mDomainCaption.insert("Total",    QString::fromUtf8("Gesamtnote"));

        mGradeCaption.insert("NoText",   QString::fromUtf8("Nicht erwähnen"));
        mGradeCaption.insert("VeryGood", QString::fromUtf8("Sehr gut"));
        mGradeCaption.insert("Good",     QString::fromUtf8("Gut"));
        mGradeCaption.insert("Normal",   QString::fromUtf8("Befriedigend"));
        mGradeCaption.insert("Bad",      QString::fromUtf8("Mangelhaft"));

        // Standard Beurteilungstexte
        mAssessmentTextBlock.insert("Skills" "VeryGood",   QString::fromUtf8("Durch [seine/ihre] sehr zügige und exakte Arbeitsweise erbrachte [er/sie] auch in Ausnahmesituationen eine vollst zufriedenstellende Leistung."));
        mAssessmentTextBlock.insert("Skills" "Good",       QString::fromUtf8("Durch [seine/ihre] zügige und exakte Arbeitsweise erbrachte [er/sie] eine voll zufriedenstellende Leistung."));
        mAssessmentTextBlock.insert("Skills" "Normal",     QString::fromUtf8("Durch [seine/ihre] recht zügige und exakte Arbeitsweise erbrachte [er/sie] eine zufriedenstellende Leistung."));
        mAssessmentTextBlock.insert("Skills" "Bad",        QString::fromUtf8("Durch [seine/ihre] Arbeitsweise erbrachte [er/sie] mitunter eine zufriedenstellende Leistung."));

        mAssessmentTextBlock.insert("Care" "VeryGood",     QString::fromUtf8("[Er/Sie] arbeitete sehr genau, gründlich und äußerst gewissenhaft."));
        mAssessmentTextBlock.insert("Care" "Good",         QString::fromUtf8("[Er/Sie] arbeitete gründlich, gewissenhaft und sorgfältig."));
        mAssessmentTextBlock.insert("Care" "Normal",       QString::fromUtf8("[Er/Sie] war ordentlich und handelte mit Sorgfalt."));
        mAssessmentTextBlock.insert("Care" "Bad",          QString::fromUtf8("[Er/Sie] bemühte sich um Sorgfalt."));

        mAssessmentTextBlock.insert("Interest" "VeryGood", QString::fromUtf8("[Er/Sie] zeigte außergewöhnliches Interesse und besonders ausgeprägte Initiative."));
        mAssessmentTextBlock.insert("Interest" "Good",     QString::fromUtf8("[Er/Sie] zeigte ausgeprägtes Interesse und Initiative."));
        mAssessmentTextBlock.insert("Interest" "Normal",   QString::fromUtf8("[Er/Sie] zeigte Interesse und ergriff selbst die Initative."));
        mAssessmentTextBlock.insert("Interest" "Bad",      QString::fromUtf8("[Er/Sie] war meist interessiert bei [ihm/ihr] übertragenen Aufgaben."));

        mAssessmentTextBlock.insert("Teamwork" "VeryGood", QString::fromUtf8("[Er/Sie] fügte sich stets sehr gut in die Gruppe ein. Zeigte sehr gute Zusammenarbeit und Hilfsbereitschaft. Bereicherte die Gruppe."));
        mAssessmentTextBlock.insert("Teamwork" "Good",     QString::fromUtf8("[Er/Sie] fügte sich stets gut in die Gruppe ein. Ist hilfsbereit und fähig zu guter Zusammenarbeit."));
        mAssessmentTextBlock.insert("Teamwork" "Normal",   QString::fromUtf8("[Er/Sie] fügte sich gut in die Gruppe ein. Hat den Willen zu Hilfsbereitschaft und Zusammenarbeit."));
        mAssessmentTextBlock.insert("Teamwork" "Bad",      QString::fromUtf8("[Er/Sie] fügte sich in die Gruppe ein. Ist fähig zur Zusammenarbeit."));

        mAssessmentTextBlock.insert("Total" "VeryGood",    QString::fromUtf8("[Er/Sie] führte alle Aufgaben stets zu unserer vollsten Zufriedenheit aus."));
        mAssessmentTextBlock.insert("Total" "Good",        QString::fromUtf8("[Er/Sie] führte alle Aufgaben stets zu unserer vollen Zufriedenheit aus."));
        mAssessmentTextBlock.insert("Total" "Normal",      QString::fromUtf8("[Er/Sie] führte alle Aufgaben zu unserer vollen Zufriedenheit aus."));
        mAssessmentTextBlock.insert("Total" "Bad",         QString::fromUtf8("[Er/Sie] führte alle Aufgaben zu unserer Zufriedenheit aus."));

}




QString Presets::domainCaption(const QString &domain) const
{
        QString ret;
        if (mDomainCaption.contains(domain)) {
                ret = mDomainCaption[domain];
        } else {
                ret = domain;
        }
        return ret;
}




QString Presets::gradeCaption(const QString &grade) const
{
        QString ret;
        if (mGradeCaption.contains(grade)) {
                ret = mGradeCaption[grade];
        } else {
                ret = grade;
        }
        return ret;
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




QString Presets::assessmentTextBlock(const QString &name) const
{
        QString ret;
        if (mAssessmentTextBlock.contains(name)) {
                ret = mAssessmentTextBlock[name];
        }

        return ret;
}
