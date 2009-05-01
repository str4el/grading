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



#include <QtGui>
#include "GradingWindow.h"


GradingWindow::GradingWindow(QWidget *parrent) : QMainWindow(parrent)
{

	setupUi(this);


/* Die checks Grupieren
 */
	QButtonGroup *g1 = new QButtonGroup;
	g1->addButton(check_a1);
	g1->addButton(check_b1);
	g1->addButton(check_c1);
	g1->addButton(check_d1);
	g1->addButton(check_e1);

	QButtonGroup *g2 = new QButtonGroup;
	g2->addButton(check_a2);
	g2->addButton(check_b2);
	g2->addButton(check_c2);
	g2->addButton(check_d2);
	g2->addButton(check_e2);

	QButtonGroup *g3 = new QButtonGroup;
	g3->addButton(check_a3);
	g3->addButton(check_b3);
	g3->addButton(check_c3);
	g3->addButton(check_d3);
	g3->addButton(check_e3);

	QButtonGroup *g4 = new QButtonGroup;
	g4->addButton(check_a4);
	g4->addButton(check_b4);
	g4->addButton(check_c4);
	g4->addButton(check_d4);
	g4->addButton(check_e4);

	QButtonGroup *g5 = new QButtonGroup;
	g5->addButton(check_a5);
	g5->addButton(check_b5);
	g5->addButton(check_c5);
	g5->addButton(check_d5);
	g5->addButton(check_e5);

	QButtonGroup *g6 = new QButtonGroup;
	g6->addButton(check_a6);
	g6->addButton(check_b6);
	g6->addButton(check_c6);
	g6->addButton(check_d6);
	g6->addButton(check_e6);

	QButtonGroup *g7 = new QButtonGroup;
	g7->addButton(check_a7);
	g7->addButton(check_b7);
	g7->addButton(check_c7);
	g7->addButton(check_d7);
	g7->addButton(check_e7);

	QButtonGroup *g8 = new QButtonGroup;
	g8->addButton(check_a8);
	g8->addButton(check_b8);
	g8->addButton(check_c8);
	g8->addButton(check_d8);
	g8->addButton(check_e8);

	show();


}


GradingWindow::~GradingWindow()
{

}


void GradingWindow::closeEvent(QCloseEvent *event)
{
  event->accept();
}

