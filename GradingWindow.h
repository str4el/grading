/*
 *
 *  Copyright (C) 2009 Stephan Reinhard <Stephan-Reinhard@gmx.de>
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


#ifndef GRADINGWINDOW_H
#define GRADINGWINDOW_H

#include <QtGui>
#include "ui_GradingWindow.h"

class GradingWindow : public QMainWindow, public Ui::GradingWindow
{

	Q_OBJECT

public:
// Funktionen
	GradingWindow(QWidget *parrent = 0);
	~GradingWindow();

// Klassen

public slots:

protected:
// Variablen

// Klassen

// Funktionen
	void closeEvent(QCloseEvent *event);

private:
// Variablen

// Klassen

// Funktionen

private slots:
	void stack_text();

};

#endif
