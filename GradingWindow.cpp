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

        connect(save, SIGNAL(clicked()), this, SLOT(save_data()));
        connect(load, SIGNAL(clicked()), this, SLOT(load_data()));


        browser->setSource(QUrl("help.htm"));


        Option config("config", this);
        save_dir = config.getOption("save_dir");

        if (save_dir.isEmpty()) {
#ifdef Q_OS_LINUX
                save_dir = QProcessEnvironment::systemEnvironment().value("HOME");
#endif

#ifdef Q_OS_WIN32
                QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", QSettings::NativeFormat);
                save_dir = settings.value("Personal").toString();
#endif
                if (save_dir.isEmpty()) {
                        save_dir = ".";
                }
        }
        save_dir += "/";

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

	if (text.isEmpty()) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Es konnte kein Text zusammengestellt werden. Möglicherweise ist die config Datei fehlerhaft.", QMessageBox::Close, this).exec();
		return;
	}
	
	if (combo_mw->currentIndex() == 0) 
		text.remove(QRegExp("/[\\w\\s]*\\]")).remove('[');
	else 
		text.remove(QRegExp("\\[[\\w\\s]*/")).remove(']');

	edit->setPlainText(text);
	tab->setCurrentWidget(tab_edit);
}



/* Die Funktion build_pdf liest den inhalt aus frame.tex modifiziert die Variablen und schreib es in grading.tex.
 * grading.tex wird an LaTeX übergeben und die resultierende pdf Datei angezeigt.
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

        QFile out(QDir::tempPath() + "/grading.tex");
	if (!out.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox(QMessageBox::Warning, "Fehler", "LaTeX Quelldatei konnte nicht erstellt werden!", QMessageBox::Close, this).exec();
		return;
	}

	QTextStream out_stream(&out);
	out_stream.setCodec("UTF-8");

	out_stream << source;
	out.close();

        latex->setWorkingDirectory(QDir::tempPath());
        latex->start(config.getOption("bin_pdflatex"), QStringList("grading.tex"));
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

                viewer->start(config.getOption("bin_pdfview"), QStringList(QDir::tempPath() + "/grading.pdf"));
		if (!viewer->waitForStarted(3000)) {
			QMessageBox(QMessageBox::Warning, "Fehler", "Der PDF Betrachter konnte nicht gestartet werden!", QMessageBox::Close, this).exec();
			viewer->close();
			return;
		}
	} else {
	QMessageBox(QMessageBox::Warning, "Fehler", "LaTeX wurde nicht ordnugsgemäß beendet!", QMessageBox::Close, this).exec();
	}
}





void GradingWindow::closeEvent(QCloseEvent *event)
{
	event->accept();
}





void GradingWindow::save_data()
{
        if (save_name.isEmpty()) {
                save_name = save_name_apprentice->text() + " " + QString::number(save_year->value());
        }

	QString filename = QFileDialog::getSaveFileName(this, "Speichern", save_dir + save_name, "Beurteilung (*.grd)");
	if (filename.isEmpty()) {
		return;
	}

	if (QFileInfo(filename).suffix().isEmpty()) 
		filename.append(".grd");
	
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Datei konnte nicht geöffnet werden!", QMessageBox::Close, this).exec();
		return;
	}

	QDataStream out(&file);
	out.setVersion(QDataStream::Qt_4_5);

	out << 0xEFAA << 1;
	
	out << save_name_apprentice->text();
	out << save_name_instructor->text();
	out << save_year->value();
	out << save_date1->date();
	out << save_date2->date();
	out << save_comment->toPlainText();
	
	out << check_a1->isChecked();
	out << check_a2->isChecked();
	out << check_a3->isChecked();
	out << check_a4->isChecked();
	out << check_a5->isChecked();
	
	out << check_b1->isChecked();
	out << check_b2->isChecked();
	out << check_b3->isChecked();
	out << check_b4->isChecked();
	out << check_b5->isChecked();
	
	out << check_c1->isChecked();
	out << check_c2->isChecked();
	out << check_c3->isChecked();
	out << check_c4->isChecked();
	out << check_c5->isChecked();
	
	out << check_d1->isChecked();
	out << check_d2->isChecked();
	out << check_d3->isChecked();
	out << check_d4->isChecked();
	out << check_d5->isChecked();
	
	out << check_e1->isChecked();
	out << check_e2->isChecked();
	out << check_e3->isChecked();
	out << check_e4->isChecked();
	out << check_e5->isChecked();
	
	out << check_f1->isChecked();
	out << check_f2->isChecked();
	out << check_f3->isChecked();
	out << check_f4->isChecked();
	out << check_f5->isChecked();
	
	out << check_g1->isChecked();
	out << check_g2->isChecked();
	out << check_g3->isChecked();
	out << check_g4->isChecked();
	out << check_g5->isChecked();
	
	out << check_h1->isChecked();
	out << check_h2->isChecked();
	out << check_h3->isChecked();
	out << check_h4->isChecked();
	out << check_h5->isChecked();
	

	out << combo_mw->currentIndex();

	out << combo_a->currentIndex();
	out << combo_b->currentIndex();
	out << combo_c->currentIndex();
	out << combo_d->currentIndex();
	out << combo_e->currentIndex();
	
	out << edit->toPlainText();

	file.close();
}



void GradingWindow::load_data()
{
	QString filename = QFileDialog::getOpenFileName(this, "Speichern", save_dir, "Beurteilung (*.grd)");
	if (filename.isEmpty()) {
		return;
	}
	
	
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Datei konnte nicht geöffnet werden!", QMessageBox::Close, this).exec();
		return;
	}

	save_dir = QFileInfo(file).path() + "/";
	save_name = QFileInfo(file).fileName();

	QDataStream in(&file);
	in.setVersion(QDataStream::Qt_4_5);

	QString text;
	QDate date;
	int value;
	bool flag;

	in >> value;
	if (value != 0xEFAA) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Unbekanntes Dateiformat!", QMessageBox::Close, this).exec();
		return;
	}

	int version;
	in >> version;
	if (version > 1) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Die Datei wurde von einer neueren Version des Programms gespeichert!", QMessageBox::Close, this).exec();
		return;
	}
	
	in >> text;
	save_name_apprentice->setText(text);
	in >> text;
	save_name_instructor->setText(text);
	in >> value;
	save_year->setValue(value);
	in >> date;
	save_date1->setDate(date);
	in >> date;
	save_date2->setDate(date);
	in >> text;
	save_comment->setPlainText(text);


	in >> flag;
	check_a1->setChecked(flag);
	in >> flag;
	check_a2->setChecked(flag);
	in >> flag;
	check_a3->setChecked(flag);
	in >> flag;
	check_a4->setChecked(flag);
	in >> flag;
	check_a5->setChecked(flag);

	in >> flag;
	check_b1->setChecked(flag);
	in >> flag;
	check_b2->setChecked(flag);
	in >> flag;
	check_b3->setChecked(flag);
	in >> flag;
	check_b4->setChecked(flag);
	in >> flag;
	check_b5->setChecked(flag);

	in >> flag;
	check_c1->setChecked(flag);
	in >> flag;
	check_c2->setChecked(flag);
	in >> flag;
	check_c3->setChecked(flag);
	in >> flag;
	check_c4->setChecked(flag);
	in >> flag;
	check_c5->setChecked(flag);

	in >> flag;
	check_d1->setChecked(flag);
	in >> flag;
	check_d2->setChecked(flag);
	in >> flag;
	check_d3->setChecked(flag);
	in >> flag;
	check_d4->setChecked(flag);
	in >> flag;
	check_d5->setChecked(flag);

	in >> flag;
	check_e1->setChecked(flag);
	in >> flag;
	check_e2->setChecked(flag);
	in >> flag;
	check_e3->setChecked(flag);
	in >> flag;
	check_e4->setChecked(flag);
	in >> flag;
	check_e5->setChecked(flag);

	in >> flag;
	check_f1->setChecked(flag);
	in >> flag;
	check_f2->setChecked(flag);
	in >> flag;
	check_f3->setChecked(flag);
	in >> flag;
	check_f4->setChecked(flag);
	in >> flag;
	check_f5->setChecked(flag);

	in >> flag;
	check_g1->setChecked(flag);
	in >> flag;
	check_g2->setChecked(flag);
	in >> flag;
	check_g3->setChecked(flag);
	in >> flag;
	check_g4->setChecked(flag);
	in >> flag;
	check_g5->setChecked(flag);

	in >> flag;
	check_h1->setChecked(flag);
	in >> flag;
	check_h2->setChecked(flag);
	in >> flag;
	check_h3->setChecked(flag);
	in >> flag;
	check_h4->setChecked(flag);
	in >> flag;
	check_h5->setChecked(flag);

	
	in >> value;
	combo_mw->setCurrentIndex(value);

	in >> value;
	combo_a->setCurrentIndex(value);
	in >> value;
	combo_b->setCurrentIndex(value);
	in >> value;
	combo_c->setCurrentIndex(value);
	in >> value;
	combo_d->setCurrentIndex(value);
	in >> value;
	combo_e->setCurrentIndex(value);

	in >> text;
	edit->setPlainText(text);

	
	file.close();
}




