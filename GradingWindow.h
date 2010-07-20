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
#include "ui_GradingWindow.h"
#include "GradingBuild.h"

/*!
* \class GradingWindow
* \brief Class for creating main window of grading program.
*
* \author Stephan Reinhard
* \author Wolfgang Forstmeier
*/
class GradingWindow : public QMainWindow, public Ui::GradingWindow
{
        Q_OBJECT

public:
        // Constructor.
        GradingWindow(QWidget *parrent = 0);
        // Overwrite compiler generated constructor.
        ~GradingWindow();

private:
        QSettings config;

        QButtonGroup *radioGroupA;
        QButtonGroup *radioGroupB;
        QButtonGroup *radioGroupC;
        QButtonGroup *radioGroupD;
        QButtonGroup *radioGroupE;
        QButtonGroup *radioGroupF;
        QButtonGroup *radioGroupG;
        QButtonGroup *radioGroupH;

        enum GRADE { VERY_GOOD = 0, GOOD, NORMAL, BAD, VERY_BAD };

        QGraphicsScene *layout_scene;
        GradingBuild *builder;

        QProcess *latex;
        QProcess *viewer;

        QString save_name;
        QString save_dir;

        void groupRadioButtions(void);
        void draw_arrow(QGraphicsScene *scene, const QLineF line, const QPen pen);

private slots:
        void build_pdf();
        void stack_text();
        void draw_preview();

        void save_data();
        void load_data();

        void save_pos();
        void load_pos();

        void save_set();
        void load_set();

        void view(int exitCode, QProcess::ExitStatus exitStatus );

protected:
        void closeEvent(QCloseEvent *event);


};
