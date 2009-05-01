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
#include "Option.h"


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

	connect(stack, SIGNAL(clicked()), this, SLOT(stack_text()));

	browser->setSource(QUrl(QString("help.htm")));

	show();


}




GradingWindow::~GradingWindow()
{

}




/* Die Funktion stack_text stellt den Beurteilungstext aus Vorgefertigten Sätzen zusammen.
 * Je nachdem welche bewertung in den ComboBoxen festgelegt wurde.
 */
void GradingWindow::stack_text()
{
	Option config("config", this);
	QString text;

	if (combo_a->currentIndex() > 0) text += config.getOption(QString("text_a%1").arg(combo_a->currentIndex()));
	if (combo_b->currentIndex() > 0) text += config.getOption(QString("text_b%1").arg(combo_b->currentIndex()));
	if (combo_c->currentIndex() > 0) text += config.getOption(QString("text_c%1").arg(combo_c->currentIndex()));
	if (combo_d->currentIndex() > 0) text += config.getOption(QString("text_d%1").arg(combo_d->currentIndex()));
	if (combo_e->currentIndex() > 0) text += config.getOption(QString("text_e%1").arg(combo_e->currentIndex()));
	if (combo_f->currentIndex() > 0) text += config.getOption(QString("text_f%1").arg(combo_f->currentIndex()));
	if (combo_g->currentIndex() > 0) text += config.getOption(QString("text_g%1").arg(combo_g->currentIndex()));
	if (combo_h->currentIndex() > 0) text += config.getOption(QString("text_h%1").arg(combo_h->currentIndex()));
	if (combo_i->currentIndex() > 0) text += config.getOption(QString("text_i%1").arg(combo_i->currentIndex()));
	if (combo_j->currentIndex() > 0) text += config.getOption(QString("text_j%1").arg(combo_j->currentIndex()));

	if (text.isEmpty()) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Es konnte kein Text zusammengestellt werden. Möglicherweise ist die config Datei fehlerhaft.", QMessageBox::Close, this).exec();
		return;
	}

	edit->setPlainText(text);
	tab->setCurrentWidget(tab3);
}




void GradingWindow::closeEvent(QCloseEvent *event)
{
  event->accept();
}

