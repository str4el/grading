/*
 *
 *  Copyright (C) 2009, 2010 Stephan Reinhard <Stephan-Reinhard@gmx.de>
 *                           Wolfgang Forstmeier <wolfgang.forstmeier@gmail.com>
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

        // Group radio buttons.
        this->groupRadioButtions();

	connect(stack, SIGNAL(clicked()), this, SLOT(stack_text()));
	connect(build, SIGNAL(clicked()), this, SLOT(build_pdf()));
	connect(latex, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(view(int,QProcess::ExitStatus)));

        connect(save, SIGNAL(clicked()), this, SLOT(save_data()));
        connect(load, SIGNAL(clicked()), this, SLOT(load_data()));

        browser->setSource(QUrl("./help.htm"));

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
	source.replace("VAR_TICK_A", QString::number(tick_pos[radioGroupA->checkedId()]));
	source.replace("VAR_TICK_B", QString::number(tick_pos[radioGroupB->checkedId()]));
	source.replace("VAR_TICK_C", QString::number(tick_pos[radioGroupC->checkedId()]));
	source.replace("VAR_TICK_D", QString::number(tick_pos[radioGroupD->checkedId()]));
	source.replace("VAR_TICK_E", QString::number(tick_pos[radioGroupE->checkedId()]));
	source.replace("VAR_TICK_F", QString::number(tick_pos[radioGroupF->checkedId()]));
        source.replace("VAR_TICK_G", QString::number(tick_pos[radioGroupG->checkedId()]));
	source.replace("VAR_TICK_H", QString::number(tick_pos[radioGroupH->checkedId()]));
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


void GradingWindow::groupRadioButtions()
{
        this->radioGroupA = new QButtonGroup;
        this->radioGroupA->addButton(check_a1, GradingWindow::VERY_GOOD);
        this->radioGroupA->addButton(check_a2, GradingWindow::GOOD);
        this->radioGroupA->addButton(check_a3, GradingWindow::NORMAL);
        this->radioGroupA->addButton(check_a4, GradingWindow::BAD);
        this->radioGroupA->addButton(check_a5, GradingWindow::VERY_BAD);

        this->radioGroupB = new QButtonGroup;
        this->radioGroupB->addButton(check_b1, GradingWindow::VERY_GOOD);
        this->radioGroupB->addButton(check_b2, GradingWindow::GOOD);
        this->radioGroupB->addButton(check_b3, GradingWindow::NORMAL);
        this->radioGroupB->addButton(check_b4, GradingWindow::BAD);
        this->radioGroupB->addButton(check_b5, GradingWindow::VERY_BAD);

        this->radioGroupC = new QButtonGroup;
        this->radioGroupC->addButton(check_c1, GradingWindow::VERY_GOOD);
        this->radioGroupC->addButton(check_c2, GradingWindow::GOOD);
        this->radioGroupC->addButton(check_c3, GradingWindow::NORMAL);
        this->radioGroupC->addButton(check_c4, GradingWindow::BAD);
        this->radioGroupC->addButton(check_c5, GradingWindow::VERY_BAD);

        this->radioGroupD = new QButtonGroup;
        this->radioGroupD->addButton(check_d1, GradingWindow::VERY_GOOD);
        this->radioGroupD->addButton(check_d2, GradingWindow::GOOD);
        this->radioGroupD->addButton(check_d3, GradingWindow::NORMAL);
        this->radioGroupD->addButton(check_d4, GradingWindow::BAD);
        this->radioGroupD->addButton(check_d5, GradingWindow::VERY_BAD);

        this->radioGroupE = new QButtonGroup;
        this->radioGroupE->addButton(check_e1, GradingWindow::VERY_GOOD);
        this->radioGroupE->addButton(check_e2, GradingWindow::GOOD);
        this->radioGroupE->addButton(check_e3, GradingWindow::NORMAL);
        this->radioGroupE->addButton(check_e4, GradingWindow::BAD);
        this->radioGroupE->addButton(check_e5, GradingWindow::VERY_BAD);

        this->radioGroupF = new QButtonGroup;
        this->radioGroupF->addButton(check_f1, GradingWindow::VERY_GOOD);
        this->radioGroupF->addButton(check_f2, GradingWindow::GOOD);
        this->radioGroupF->addButton(check_f3, GradingWindow::NORMAL);
        this->radioGroupF->addButton(check_f4, GradingWindow::BAD);
        this->radioGroupF->addButton(check_f5, GradingWindow::VERY_BAD);

        this->radioGroupG = new QButtonGroup;
        this->radioGroupG->addButton(check_g1, GradingWindow::VERY_GOOD);
        this->radioGroupG->addButton(check_g2, GradingWindow::GOOD);
        this->radioGroupG->addButton(check_g3, GradingWindow::NORMAL);
        this->radioGroupG->addButton(check_g4, GradingWindow::BAD);
        this->radioGroupG->addButton(check_g5, GradingWindow::VERY_BAD);

        this->radioGroupH = new QButtonGroup;
        this->radioGroupH->addButton(check_h1, GradingWindow::VERY_GOOD);
        this->radioGroupH->addButton(check_h2, GradingWindow::GOOD);
        this->radioGroupH->addButton(check_h3, GradingWindow::NORMAL);
        this->radioGroupH->addButton(check_h4, GradingWindow::BAD);
        this->radioGroupH->addButton(check_h5, GradingWindow::VERY_BAD);
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

        // Magic Byte und Version der Datei
        out << 0xEFAA << 2;

        out << save_name_apprentice->text();
        out << save_name_instructor->text();
        out << save_year->value();
        out << save_date1->date();
        out << save_date2->date();
        out << save_comment->toPlainText();

        out << radioGroupA->checkedId();
        out << radioGroupB->checkedId();
        out << radioGroupC->checkedId();
        out << radioGroupD->checkedId();
        out << radioGroupE->checkedId();
        out << radioGroupF->checkedId();
        out << radioGroupG->checkedId();
        out << radioGroupH->checkedId();

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
        int radioButtonState;

	in >> value;
	if (value != 0xEFAA) {
		QMessageBox(QMessageBox::Warning, "Fehler", "Unbekanntes Dateiformat!", QMessageBox::Close, this).exec();
		return;
	}

	int version;
	in >> version;

        if (version < 2) {
                QMessageBox(QMessageBox::Warning, "Fehler", "Die Datei ist von einer inkompatiblen veralteten Version erstellt worden!", QMessageBox::Close, this).exec();
                return;
        }

        if (version > 2) {
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


        in >> radioButtonState;
        radioGroupA->button(radioButtonState)->setChecked(true);

        in >> radioButtonState;
        radioGroupB->button(radioButtonState)->setChecked(true);

        in >> radioButtonState;
        radioGroupC->button(radioButtonState)->setChecked(true);

        in >> radioButtonState;
        radioGroupD->button(radioButtonState)->setChecked(true);

        in >> radioButtonState;
        radioGroupE->button(radioButtonState)->setChecked(true);

        in >> radioButtonState;
        radioGroupF->button(radioButtonState)->setChecked(true);

        in >> radioButtonState;
        radioGroupG->button(radioButtonState)->setChecked(true);

        in >> radioButtonState;
        radioGroupH->button(radioButtonState)->setChecked(true);

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
