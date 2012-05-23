/*
 *
 *  Copyright (C) 2009 - 2012 Stephan Reinhard <Stephan-Reinhard@gmx.de>
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

class Layout;
class QPrinter;

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
        QPrinter mPrinter;
        Layout *mLayout;

        QString mActiveLayoutX;
        QString mActiveLayoutY;
        bool mAssessmentTextLayoutActive;
        bool mMeasuresTextLayoutActive;

        QString saveName;
        QString saveDir;

private slots:
        void updateLayoutTextRect (void);
        void updateLayoutXPos (int);
        void updateLayoutYPos (int);
        void getFont (void);

        void deactivateLayout (void);
        void activateLayoutGrade (QString, QString);
        void activateAssessmentTextLayout (void);
        void activateMeasuresTextLayout (void);

        void readDomainText (void);
        void writeDomainText (void);

        void print (void);

        void saveData(void);
        void loadData(void);
        void saveSettings(void);
        void loadSettings(void);
        void saveLayout(void);
        void loadLayout(void);
        void setDefaultLayout(void);

        void updateLayout(void);
        void stackText(void);

protected:
        void closeEvent(QCloseEvent *event);


};


#endif // MAINWINDOW_H
