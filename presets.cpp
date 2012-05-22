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
        mAssessmentTextRect(20, 160, 175, 35),
        mFont("Sans Serif" ,12)
{
        mDomainCaption.insert("Knowledge",      QString::fromUtf8("Fachkenntnisse"));
        mDomainCaption.insert("Skills",         QString::fromUtf8("Fertigkeiten"));
        mDomainCaption.insert("Safety",         QString::fromUtf8("Arbeitssicheres Verhalten"));
        mDomainCaption.insert("Reliability",    QString::fromUtf8("Zuverlässigkeit"));
        mDomainCaption.insert("Activity",       QString::fromUtf8("Lern- und Arbeitsbereitschaft"));
        mDomainCaption.insert("ProperHandling", QString::fromUtf8("Umgang mit Einrichtungen / Arbeitsmitteln"));
        mDomainCaption.insert("Teamwork",       QString::fromUtf8("Teamfähigkeit"));
        mDomainCaption.insert("Responsibility", QString::fromUtf8("Verantwortungsbewustsein"));

        mGradeCaption.insert("NoText",   QString::fromUtf8("Nicht erwähnen"));
        mGradeCaption.insert("VeryGood", QString::fromUtf8("Sehr gut"));
        mGradeCaption.insert("Good",     QString::fromUtf8("Gut"));
        mGradeCaption.insert("Normal",   QString::fromUtf8("Durchschntt"));
        mGradeCaption.insert("Bad",      QString::fromUtf8("Schlecht"));
        mGradeCaption.insert("VeryBad",  QString::fromUtf8("Sehr schlecht"));


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

        // Standard Beurteilungstexte
        mAssessmentTextBlock.insert("Knowledge" "VeryGood",      QString::fromUtf8("Fachkenntnisse sind für den Ausbildungsstand außerordentlich hoch."));
        mAssessmentTextBlock.insert("Knowledge" "Good",          QString::fromUtf8("Fachkenntnisse sind für den Ausbildungsstund hoch."));
        mAssessmentTextBlock.insert("Knowledge" "Normal",        QString::fromUtf8("Fachkenntnisse sind dem Ausbildungsstand angemessen."));
        mAssessmentTextBlock.insert("Knowledge" "Bad",           QString::fromUtf8("Fachkenntnisse weisen für den Stand der Ausbildung teilweise Lücken auf."));
        mAssessmentTextBlock.insert("Knowledge" "VeryBad",       QString::fromUtf8("Fachkenntnisse weisen für den Stand der Ausbildung erhebliche Lücken auf."));

        mAssessmentTextBlock.insert("Skills" "VeryGood",         QString::fromUtf8("Verfügt für den Stand der Ausbildung über ein sehr hohes Maß an Fertigkeiten. Führt die übertragenen Tätigkeiten mit großer Geschicklichkeit aus."));
        mAssessmentTextBlock.insert("Skills" "Good",             QString::fromUtf8("Verfügt für den Stand der Ausbildung über ein hohes Maß an Fertigkeiten. Arbeitet zügig und geschickt."));
        mAssessmentTextBlock.insert("Skills" "Normal",           QString::fromUtf8("Verfügt für den Stand der Ausbildung über ein angemessenes Maß an Fertigkeiten. Die Arbeitsausführung ist zufriedenstellend."));
        mAssessmentTextBlock.insert("Skills" "Bad",              QString::fromUtf8("Fertigkeiten sind für den Stand der Ausbildung lückenhaft. Die Arbeitsausführung ist teilweise fehlerhaft."));
        mAssessmentTextBlock.insert("Skills" "VeryBad",          QString::fromUtf8("Fertigkeiten sind für den Stand der Ausbildung sehr lückenhaft. Die Arbeitsausführung ist häufig fehlerhaft."));

        mAssessmentTextBlock.insert("Safety" "VeryGood",         QString::fromUtf8("Arbeitet stets absolut sicher und ist stets vorbildlich bei der Einhaltung der Arbeitssicherheitsvorschriften."));
        mAssessmentTextBlock.insert("Safety" "Good",             QString::fromUtf8("Arbeitet stets sicher und hält die jeweiligen Arbeitssicherheitsvorschriften ein."));
        mAssessmentTextBlock.insert("Safety" "Normal",           QString::fromUtf8("Arbeitet im Wesentlichen sicher und hält meist die jeweiligen Arbeitssicherheitsvorschriften ein."));
        mAssessmentTextBlock.insert("Safety" "Bad",              QString::fromUtf8("Zeigt im Arbeitsverhalten teilweise Unsicherheiten. Benötigt oft Hilfestellung, damit Arbeitssicherheitsvorschriften eingehalten werden können."));
        mAssessmentTextBlock.insert("Safety" "VeryBad",          QString::fromUtf8("Zeigt im Arbeitsverhalten große Unsicherheiten. Benötigt stets Überwachung, damit Arbeitssicherheitsvorschriften eingehalten werden können."));

        mAssessmentTextBlock.insert("Reliability" "VeryGood",    QString::fromUtf8("Ist sehr zuverlässig und verantwortungsbewusst in der Erledigung der gestellten Aufgaben und insbesondere bei der Einhaltung von Vorschriften, Anweisungen und Terminen."));
        mAssessmentTextBlock.insert("Reliability" "Good",        QString::fromUtf8("Ist zuverlässig und verantwortungsbewusst in der Erledigung gestellter Aufgaben. Vorschriften, Anweisungen und Termine werden eingehalten."));
        mAssessmentTextBlock.insert("Reliability" "Normal",      QString::fromUtf8("Übertragene Aufgaben werden im Allgemeinen zuverlässig durchgeführt. In der Regel werden Vorschriften, Anweisungen und Termine eingehalten."));
        mAssessmentTextBlock.insert("Reliability" "Bad",         QString::fromUtf8("Zuverlässigkeit lässt zu wünschen übrig. Vorschriften und Anweisungen werden oft nicht ausreichend beachtet. Es gibt Schwierigkeiten bei der Einhaltung von Terminen."));
        mAssessmentTextBlock.insert("Reliability" "VeryBad",     QString::fromUtf8("Vorschriften und Anweisungen werden nur ungenügend beachtet. Ist nicht zuverlässig bei der Einhaltung von Terminen."));

        mAssessmentTextBlock.insert("Activity" "VeryGood",       QString::fromUtf8("Interessierte sich in sehr hohem Maße für alle praktischen Lernmöglichkeiten und hatte eine sehr hohe Leistungsbereitschaft."));
        mAssessmentTextBlock.insert("Activity" "Good",           QString::fromUtf8("Interessierte sich in hohem Maße für alle praktischen Lernmöglichkeiten und hatte stets eine hohe Leistungsbereitschaft."));
        mAssessmentTextBlock.insert("Activity" "Normal",         QString::fromUtf8("Interessierte sich für die praktischen Lernmöglichkeiten und hatte eine durchschnittliche Leistungsbereitschaft."));
        mAssessmentTextBlock.insert("Activity" "Bad",            QString::fromUtf8("Interessierte sich nicht immer für die praktischen Lernmöglichkeiten. Seine Leistungsbereitschaft ließ häufig zu wünschen übrig."));
        mAssessmentTextBlock.insert("Activity" "VeryBad",        QString::fromUtf8("Interessierte sich nicht ausreichend für die praktischen Lernmöglichkeiten. Seine Leistungsbereitschaft ließ stets zu wünschen übrig."));

        mAssessmentTextBlock.insert("ProperHandling" "VeryGood", QString::fromUtf8("Geht stets sehr sorgsam mit Einrichtungen und Arbeitsmitteln um."));
        mAssessmentTextBlock.insert("ProperHandling" "Good",     QString::fromUtf8("Geht sehr sorgsam mit Einrichtungen und Arbeitsmitteln um."));
        mAssessmentTextBlock.insert("ProperHandling" "Normal",   QString::fromUtf8("Der Umgang mit Einrichtungen und Arbeitsmitteln entspricht den Erwartungen."));
        mAssessmentTextBlock.insert("ProperHandling" "Bad",      QString::fromUtf8("Geht nicht immer sorgsam mit Einrichtungen und Arbeitsmitteln um."));
        mAssessmentTextBlock.insert("ProperHandling" "VeryBad",  QString::fromUtf8("Der Umgang mit Einrichtungen und Arbeitsmitteln ist häufig mangelhaft."));

        mAssessmentTextBlock.insert("Teamwork" "VeryGood",       QString::fromUtf8("Fügt sich stets sehr gut in die Gruppe ein. Zeigt sehr gute Zusammenarbeit und Hilfsbereitschaft. Bereichert die Gruppe."));
        mAssessmentTextBlock.insert("Teamwork" "Good",           QString::fromUtf8("Fügt sich stets gut in die Gruppe ein. Ist hilfsbereit und fähig zu guter Zusammenarbeit."));
        mAssessmentTextBlock.insert("Teamwork" "Normal",         QString::fromUtf8("Fügt sich gut in die Gruppe ein. Hat den Willen zu Hilfsbereitschaft und Zusammenarbeit."));
        mAssessmentTextBlock.insert("Teamwork" "Bad",            QString::fromUtf8("Zeigt Unsicherheiten im Umgang mit anderen. Benötigt für die Zusammenarbeit häufig Hilfestellung."));
        mAssessmentTextBlock.insert("Teamwork" "VeryBad",        QString::fromUtf8("Kann sich nur schwer in die Gruppe einfügen. Zeigt Unsicherheiten bei der Arbeit mit anderen. Arbeitet lieber alleine."));

        mAssessmentTextBlock.insert("Responsibility" "VeryGood", QString::fromUtf8("Ist stets in hohem Maße gewissenhaft und umsichtig bei Arbeitsaufgaben und im Umgang mit Arbeitsmitteln. Zeigt ein hohes Maß an Eigenverantwortung."));
        mAssessmentTextBlock.insert("Responsibility" "Good",     QString::fromUtf8("Ist stets gewissenhaft und umsichtig bei Arbeitsaufgaben und im Umgang mit Arbeitsmitteln. Zeigt bereits Eigenverantwortung."));
        mAssessmentTextBlock.insert("Responsibility" "Normal",   QString::fromUtf8("Ist in der Regel gewissenhaft und umsichtig bei Arbeitsaufgaben und im Umgang mit Arbeitsmitteln."));
        mAssessmentTextBlock.insert("Responsibility" "Bad",      QString::fromUtf8("Gewissenhaftigkeit und Umsicht fehlen teilweise. Muss häufig zu verantwortungsbewusstem Handeln aufgefordert werden."));
        mAssessmentTextBlock.insert("Responsibility" "VeryBad",  QString::fromUtf8("Gewissenhaftigkeit und Umsicht fehlen häufig. Gleichgültigkeit und Verantwortungslosigkeit überwiegen."));

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
