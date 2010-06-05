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
#include <QString>

#include "GradingWindow.h"
#include "Option.h"
#include "GradingSave.h"
#include "GradingVersion.h"

GradingWindow::GradingWindow(QWidget *parrent) : QMainWindow(parrent)
{
        setupUi(this);

        latex = new QProcess(this);
        viewer = new QProcess(this);

        layout_scene = new QGraphicsScene(layout_preview);
        draw_preview();

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

        version_label->setText(QString("Grading ") + GradingVersion::getVersion());

        show();
}




GradingWindow::~GradingWindow()
{
        delete latex;
        delete viewer;
        delete layout_scene;
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





void GradingWindow::draw_arrow(QGraphicsScene *scene, const QLineF line, const QPen pen)
{
        scene->addLine(line, pen);

        QLineF wing(line.x1(), line.y1(), line.x1() + 3.0, line.y1());

        wing.setAngle(line.angle() + 45.0);
        scene->addLine(wing, pen);
        wing.setAngle(line.angle() - 45.0);
        scene->addLine(wing, pen);

        wing.setLine(line.x2(), line.y2(), line.x2() + 3.0, line.y2());

        wing.setAngle(line.angle() + 135.0);
        scene->addLine(wing, pen);
        wing.setAngle(line.angle() - 135.0);
        scene->addLine(wing, pen);

}




void GradingWindow::draw_preview()
{
        qreal px = 15.0;
        qreal py = 15.0;

        qreal hx[5] = { 126.0, 139.0, 152.0, 165.0, 178.0 };
        qreal hy[8] = { 64.0, 74, 84, 104, 114, 124, 144, 154 };
        qreal hs = 5;

        qreal ty = 180;


        QPen black_solid(Qt::black);
        QPen blue_solid(Qt::blue);
        QPen black_dash(Qt::black);
        black_dash.setStyle(Qt::DashLine);

        QFont font;
        font.setPixelSize(10);

        layout_scene->clear();

        // Der Rahmen
        layout_scene->addRect(0, 0, 210, 297, black_solid);
        layout_scene->addRect(px, py, 184, 250, black_dash);

        // Ausenmaße
        draw_arrow(layout_scene, QLineF(30, 2, 30, py - 2), blue_solid);
        layout_scene->addText("1", font)->setPos(32, 0);

        draw_arrow(layout_scene, QLineF(2, 30, px - 2, 30), blue_solid);
        layout_scene->addText("2", font)->setPos(2, 13);


        // Multiple choice
        for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 8; j++) {
                        layout_scene->addRect(hx[i], hy[j], hs, hs, Qt::SolidLine, Qt::Dense4Pattern);
                }
        }

        draw_arrow(layout_scene, QLineF(px + 2, hy[0] + 2, hx[0] - 2, hy[0] + 2), blue_solid);
        draw_arrow(layout_scene, QLineF(px + 2, hy[0] - 4, hx[1] - 2, hy[0] - 4), blue_solid);
        draw_arrow(layout_scene, QLineF(px + 2, hy[0] - 10, hx[2] - 2, hy[0] - 10), blue_solid);
        draw_arrow(layout_scene, QLineF(px + 2, hy[0] - 16, hx[3] - 2, hy[0] - 16), blue_solid);
        draw_arrow(layout_scene, QLineF(px + 2, hy[0] - 22, hx[4] - 2, hy[0] - 22), blue_solid);

        layout_scene->addText("3", font)->setPos(60, hy[0] + 2);
        layout_scene->addText("7", font)->setPos(60, hy[0] - 38);

        draw_arrow(layout_scene, QLineF(hx[4] + 3, py + 2, hx[4] + 3, hy[0] - 2), blue_solid);
        layout_scene->addText("8", font)->setPos(hx[4] + 5, 35);

        draw_arrow(layout_scene, QLineF(hx[4] + 3, hy[2] + hs + 2, hx[4] + 3, hy[3] - 2), blue_solid);
        layout_scene->addText("9", font)->setPos(hx[4] + 5, hy[2] + hs - 2);

        draw_arrow(layout_scene, QLineF(hx[4] + 3, hy[5] + hs + 2, hx[4] + 3, hy[6] - 2), blue_solid);
        layout_scene->addText("10", font)->setPos(hx[4] + 3, hy[5] + hs - 2);

        // Text
        layout_scene->addRect(px + 2, ty, 180, 40, Qt::SolidLine, Qt::Dense4Pattern);

        draw_arrow(layout_scene, QLineF(hx[4] + 3, hy[7] + hs + 2, hx[4] + 3, ty - 2), blue_solid);
        layout_scene->addText("11", font)->setPos(hx[4] + 3, hy[7] + hs);



        //layout_preview->scale(2.0, 2.0);
        layout_preview->setScene(layout_scene);
}







void GradingWindow::save_data()
{
        if (save_name.isEmpty()) {
                save_name = save_name_apprentice->text() + " " + QString::number(save_year->value());
        }

        QString filename = QFileDialog::getSaveFileName(this, "Save", save_dir + save_name, "Beurteilung (*.grd)");
        if (filename.isEmpty()) {
                return;
        }

        if (QFileInfo(filename).suffix().isEmpty())
                filename.append(".grd");

        GradingSave *binarySave = new GradingSave( GradingSave::BINARY_FILE );

        binarySave->setFilename(filename);

        binarySave->registerVariable("Apprentice Name", save_name_apprentice->text());
        binarySave->registerVariable("Instructor Name", save_name_instructor->text());
        binarySave->registerVariable("Year", save_year->value());
        binarySave->registerVariable("Date1", save_date1->date());
        binarySave->registerVariable("Date2", save_date2->date());
        binarySave->registerVariable("comment", save_comment->toPlainText());

        binarySave->registerVariable("radioGroupA", radioGroupA->checkedId());
        binarySave->registerVariable("radioGroupB", radioGroupB->checkedId());
        binarySave->registerVariable("radioGroupC", radioGroupC->checkedId());
        binarySave->registerVariable("radioGroupD", radioGroupD->checkedId());
        binarySave->registerVariable("radioGroupE", radioGroupE->checkedId());
        binarySave->registerVariable("radioGroupF", radioGroupF->checkedId());
        binarySave->registerVariable("radioGroupG", radioGroupG->checkedId());
        binarySave->registerVariable("radioGroupH", radioGroupH->checkedId());

        binarySave->registerVariable("MW index", combo_mw->currentIndex());

        binarySave->registerVariable("combaA index", combo_a->currentIndex());
        binarySave->registerVariable("comboB index", combo_b->currentIndex());
        binarySave->registerVariable("comboC index", combo_c->currentIndex());
        binarySave->registerVariable("comboD index", combo_d->currentIndex());
        binarySave->registerVariable("comboE index", combo_e->currentIndex());

        binarySave->registerVariable("edit", edit->toPlainText());
        binarySave->registerVariable("edit", edit->toPlainText());


        if( !binarySave->save() )
        {
                QMessageBox(QMessageBox::Warning, "Fehler", "Fehler beim speichern der Datei!", QMessageBox::Close, this).exec();
        }


        delete binarySave;
}

void GradingWindow::load_data()
{
        QString filename = QFileDialog::getOpenFileName(this, "Open", save_dir, "Beurteilung (*.grd)");
        if (filename.isEmpty()) {
                return;
        }

        GradingSave *binaryLoad = new GradingSave( GradingSave::BINARY_FILE );

        binaryLoad->setFilename(filename);

        if( binaryLoad->load() )
        {
                save_name_apprentice->setText(binaryLoad->getValue("Apprentice Name").toString());
                save_name_instructor->setText(binaryLoad->getValue("Instructor Name").toString());
                save_year->setValue(binaryLoad->getValue("Year").toInt());
                save_date1->setDate(binaryLoad->getValue("Date1").toDate());
                save_date2->setDate(binaryLoad->getValue("Date2").toDate());
                save_comment->setPlainText(binaryLoad->getValue("comment").toString());

                radioGroupA->button(binaryLoad->getValue("radioGroupA").toInt())->setChecked(true);
                radioGroupB->button(binaryLoad->getValue("radioGroupB").toInt())->setChecked(true);
                radioGroupC->button(binaryLoad->getValue("radioGroupC").toInt())->setChecked(true);
                radioGroupD->button(binaryLoad->getValue("radioGroupD").toInt())->setChecked(true);
                radioGroupE->button(binaryLoad->getValue("radioGroupE").toInt())->setChecked(true);
                radioGroupF->button(binaryLoad->getValue("radioGroupF").toInt())->setChecked(true);
                radioGroupG->button(binaryLoad->getValue("radioGroupG").toInt())->setChecked(true);
                radioGroupH->button(binaryLoad->getValue("radioGroupH").toInt())->setChecked(true);

                combo_mw->setCurrentIndex(binaryLoad->getValue("MW index").toInt());

                combo_a->setCurrentIndex(binaryLoad->getValue("combaA index").toInt());
                combo_b->setCurrentIndex(binaryLoad->getValue("comboB index").toInt());
                combo_c->setCurrentIndex(binaryLoad->getValue("comboC index").toInt());
                combo_d->setCurrentIndex(binaryLoad->getValue("comboD index").toInt());
                combo_e->setCurrentIndex(binaryLoad->getValue("comboE index").toInt());

                edit->setPlainText(binaryLoad->getValue("edit").toString());
        }
        else
        {
                QMessageBox(QMessageBox::Warning, "Fehler", "Datei konnte nicht eingelesen werden!", QMessageBox::Close, this).exec();
        }

        delete binaryLoad;
}
