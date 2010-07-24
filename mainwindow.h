/*
 *
 *  Copyright (C) 2009, 2010 Stephan Reinhard <Stephan-Reinhard@gmx.de>
 *  Copyright (C) 2010 Wolfgang Forstmeier <wolfgang.forstmeier@gmail.com>
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

#pragma once

#include <QtGui>
#include "build.h"

namespace Ui {
        class MainWindow;
}


/*!
* \class MainWindow
* \brief Class for creating main window of grading program.
*
* \author Stephan Reinhard
* \author Wolfgang Forstmeier
*/
class MainWindow : public QMainWindow
{
        Q_OBJECT

public:
        // Constructor.
        MainWindow(QWidget *parrent = 0);
        // Overwrite compiler generated constructor.
        ~MainWindow();

private:
        Ui::MainWindow *ui;

        QSettings config;

        QMap<QString, QString> domainNames;

        QButtonGroup *assessmentKnowledgeRadioGroup;
        QButtonGroup *assessmentSkillsRadioGroup;
        QButtonGroup *assessmentSavetyRadioGroup;
        QButtonGroup *assessmentReliabilityRadioGroup;
        QButtonGroup *assessmentActivityRadioGroup;
        QButtonGroup *assessmentProperHandlingRadioGroup;
        QButtonGroup *assessmentTeamworkRadioGroup;
        QButtonGroup *assessmentResponsibilityRadioGroup;

        enum GRADE { VERY_GOOD = 0, GOOD, NORMAL, BAD, VERY_BAD };

        QGraphicsScene *layoutScene;
        Build *builder;

        QProcess *latex;
        QProcess *viewer;

        QString saveName;
        QString saveDir;

        void groupRadioButtions(void);
        void drawArrow(QGraphicsScene *scene, const QLineF line, const QPen pen);

        QString getText(QString domain, int grade);

private slots:
        void settingsGradeInit();
        void settingsGradeRead();
        void settingsGradeWrite();

        void settingsLatexFind();
        void settingsPdfFind();

        void buildPdf();
        void stackText();
        void drawPreview();

        void saveData();
        void loadData();

        void savePos();
        void loadPos();
        void setDefaultPos();

        void viewPdf(int exitCode, QProcess::ExitStatus exitStatus );

protected:
        void closeEvent(QCloseEvent *event);


};
