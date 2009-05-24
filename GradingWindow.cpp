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

	latex = new QProcess(this);
	viewer = new QProcess(this);

/* Die checks Grupieren
 */
	ga = new QButtonGroup;
	ga->addButton(check_a1, 0);
	ga->addButton(check_a2, 1);
	ga->addButton(check_a3, 2);
	ga->addButton(check_a4, 3);
	ga->addButton(check_a5, 4);

	gb = new QButtonGroup;
	gb->addButton(check_b1, 0);
	gb->addButton(check_b2, 1);
	gb->addButton(check_b3, 2);
	gb->addButton(check_b4, 3);
	gb->addButton(check_b5, 4);

	gc = new QButtonGroup;
	gc->addButton(check_c1, 0);
	gc->addButton(check_c2, 1);
	gc->addButton(check_c3, 2);
	gc->addButton(check_c4, 3);
	gc->addButton(check_c5, 4);

	gd = new QButtonGroup;
	gd->addButton(check_d1, 0);
	gd->addButton(check_d2, 1);
	gd->addButton(check_d3, 2);
	gd->addButton(check_d4, 3);
	gd->addButton(check_d5, 4);

	ge = new QButtonGroup;
	ge->addButton(check_e1, 0);
	ge->addButton(check_e2, 1);
	ge->addButton(check_e3, 2);
	ge->addButton(check_e4, 3);
	ge->addButton(check_e5, 4);

	gf = new QButtonGroup;
	gf->addButton(check_f1, 0);
	gf->addButton(check_f2, 1);
	gf->addButton(check_f3, 2);
	gf->addButton(check_f4, 3);
	gf->addButton(check_f5, 4);

	gg = new QButtonGroup;
	gg->addButton(check_g1, 0);
	gg->addButton(check_g2, 1);
	gg->addButton(check_g3, 2);
	gg->addButton(check_g4, 3);
	gg->addButton(check_g5, 4);

	gh = new QButtonGroup;
	gh->addButton(check_h1, 0);
	gh->addButton(check_h2, 1);
	gh->addButton(check_h3, 2);
	gh->addButton(check_h4, 3);
	gh->addButton(check_h5, 4);

	connect(stack, SIGNAL(clicked()), this, SLOT(stack_text()));
	connect(build, SIGNAL(clicked()), this, SLOT(build_pdf()));
	connect(latex, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(view(int,QProcess::ExitStatus)));

	browser->setSource(QUrl("help.htm"));

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



/* Die Funktion build_pdf liest den inhalt aus frame.tex modifiziert die Variablen und schreib es in out.tex.
 * out.tex wird an LaTeX übergeben und die resultierende pdf Datei angezeigt.
 */
void GradingWindow::build_pdf()
{
	Option config("config", this);
	bool ok;

	int top_pos = config.getOption("top_pos").toInt(&ok);
	if (!ok) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Ungültige Konfiguration der Postion oben.", QMessageBox::Close, this).exec();
		return;
	}
	int left_pos = config.getOption("left_pos").toInt(&ok);
	if (!ok) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Ungültige Konfiguration der Postion links.", QMessageBox::Close, this).exec();
		return;
	}
	int top_to_tick = config.getOption("top_to_tick").toInt(&ok);
	if (!ok) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Ungültige Konfiguration des oberen Abstands.", QMessageBox::Close, this).exec();
		return;
	}
	int tick_to_text = config.getOption("tick_to_text").toInt(&ok);
	if (!ok) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Ungültige Konfiguration des Abstands zum Text.", QMessageBox::Close, this).exec();
		return;
	}
	int tick_to_tick_1 = config.getOption("tick_to_tick_1").toInt(&ok);
	if (!ok) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Ungültige Konfiguration des ersten Abstand zwischen den Haken.", QMessageBox::Close, this).exec();
		return;
	}
	int tick_to_tick_2 = config.getOption("tick_to_tick_2").toInt(&ok);
	if (!ok) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Ungültige Konfiguration des zweiten Abstand zwischen den Haken.", QMessageBox::Close, this).exec();
		return;
	}

	QString tick_pos_string = config.getOption("tick_pos");
	if (tick_pos_string.isEmpty()) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Ungültige Konfiguration der Hackenpositionen! Option nicht Lesbar.", QMessageBox::Close, this).exec();
		return;
	}
	QStringList tick_pos_list = tick_pos_string.split(QChar(';'));
	if (tick_pos_list.size() != 5) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Ungültige Konfiguration der Hackenpositionen! Ungültige Anzahl.", QMessageBox::Close, this).exec();
		return;
	}
	int tick_pos[5];
	for (int i = 0; i < 5; i++) {
		tick_pos[i] = tick_pos_list[i].toInt(&ok);
		if (!ok) {
			QMessageBox(QMessageBox::Warning, "Fehler", "Ungültige Konfiguration der Hackenpositionen! Optionen sind keine Zahlen.", QMessageBox::Close, this).exec();
			return;
		}
	}
	
	tick_pos[0] += offset_tick_1->value();
	tick_pos[1] += offset_tick_2->value();
	tick_pos[2] += offset_tick_3->value();
	tick_pos[3] += offset_tick_4->value();
	tick_pos[4] += offset_tick_5->value();
	


	QFile in("frame.tex");
	if (!in.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox(QMessageBox::Warning, "Fehler", "LaTeX Ramendatei konnte nicht geöffnet werden!", QMessageBox::Close, this).exec();
		return;
	}
	QTextStream in_stream(&in);
	in_stream.setCodec("UTF-8");

	QString source = in_stream.readAll();
	in.close();

	if (source.isEmpty()) {	
		QMessageBox(QMessageBox::Warning, "Fehler", "LaTeX Ramendatei ist leer oder defekt!", QMessageBox::Close, this).exec();
		return;
	}

	source.replace("VAR_TOP_POS", QString::number(top_pos + offset_top->value()));
	source.replace("VAR_LEFT_POS", QString::number(left_pos + offset_left->value()));
	source.replace("VAR_TOP_TO_TICK", QString::number(top_to_tick + offset_top_to_tick->value()));
	source.replace("VAR_TICK_TO_TEXT", QString::number(tick_to_text + offset_tick_to_text->value()));
	source.replace("VAR_TICK_TO_TICK_1", QString::number(tick_to_tick_1 + offset_tick_to_tick_1->value()));
	source.replace("VAR_TICK_TO_TICK_2", QString::number(tick_to_tick_2 + offset_tick_to_tick_2->value()));
	source.replace("VAR_TICK_A", QString::number(tick_pos[ga->checkedId()]));
	source.replace("VAR_TICK_B", QString::number(tick_pos[gb->checkedId()]));
	source.replace("VAR_TICK_C", QString::number(tick_pos[gc->checkedId()]));
	source.replace("VAR_TICK_D", QString::number(tick_pos[gd->checkedId()]));
	source.replace("VAR_TICK_E", QString::number(tick_pos[ge->checkedId()]));
	source.replace("VAR_TICK_F", QString::number(tick_pos[gf->checkedId()]));
	source.replace("VAR_TICK_G", QString::number(tick_pos[gg->checkedId()]));
	source.replace("VAR_TICK_H", QString::number(tick_pos[gh->checkedId()]));
	source.replace("VAR_TEXT", edit->toPlainText());

	QFile out("out.tex");
	if (!out.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox(QMessageBox::Warning, "Fehler", "LaTeX Quelldatei konnte nicht erstellt werden!", QMessageBox::Close, this).exec();
		return;
	}

	QTextStream out_stream(&out);
	out_stream.setCodec("UTF-8");

	out_stream << source;
	out.close();

	latex->start(config.getOption("bin_pdflatex"), QStringList("out.tex"));
	if (!latex->waitForStarted(3000)) {
		QMessageBox(QMessageBox::Warning, "Fehler", "LaTeX konnte nicht gestartet werden!", QMessageBox::Close, this).exec();
		viewer->close();
		return;
	}
}





void GradingWindow::view(int exitCode, QProcess::ExitStatus exitStatus )
{
	Option config("config", this);

	if ((exitStatus == QProcess::NormalExit) && (exitCode == 0)) {

		if (viewer->state() == QProcess::Running) viewer->close();

		viewer->start(config.getOption("bin_pdfview"), QStringList("out.pdf"));
		if (!viewer->waitForStarted(3000)) {
			QMessageBox(QMessageBox::Warning, "Fehler", "Der PDF Betrachter konnte nicht gestartet werden!", QMessageBox::Close, this).exec();
			viewer->close();
			return;
		}
	} else {
	QMessageBox(QMessageBox::Warning, "Fehler", "LaTeX wurde nicht Ordnugsgemäß beendet", QMessageBox::Close, this).exec();
	}
}


void GradingWindow::closeEvent(QCloseEvent *event)
{
	event->accept();
}

