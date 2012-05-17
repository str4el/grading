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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

namespace Ui {
        class MainWindow;
}


/*!
* \class MainWindow
* \brief Class for careating main window of grading program.
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
        ~MainWindow() {}

private:
        Ui::MainWindow *ui;

        QSettings config;

        QMap<QString, QString> domainNames;

        QGraphicsScene *layoutScene;

        QString saveName;
        QString saveDir;

        QString getText(QString domain, int grade);

private slots:
        void settingsGradeInit(void);
        void settingsGradeRead(void);
        void settingsGradeWrite(void);

        void stackText(void);

        void saveData(void);
        void loadData(void);

        void saveSettings(void);
        void loadSettings(void);

protected:
        void closeEvent(QCloseEvent *event);


};


#endif // MAINWINDOW_H
