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

#include <QtGui>

#include "mainwindow.h"
#include "option.h"
#include "presets.h"
#include "build.h"

MainWindow::MainWindow(QWidget *parrent) : QMainWindow(parrent), config("config.ini", QSettings::IniFormat, this)
{
        setupUi(this);

        load_set();

        latex = new QProcess(this);
        viewer = new QProcess(this);


        domainNames.insert("skills", "Fertigkeiten");
        domainNames.insert("care", "Sorgfalt");
        domainNames.insert("interest", "Interesse");
        domainNames.insert("teamwork", "Zusammenarbeit");
        domainNames.insert("total", "Gesamtnote");

        settingsGradeInit();

        builder = new Build();


        connect(offset_top, SIGNAL(valueChanged(int)), builder, SLOT(setTopPos(int)));
        connect(offset_left, SIGNAL(valueChanged(int)), builder, SLOT(setLeftPos(int)));
        connect(offset_top_to_tick, SIGNAL(valueChanged(int)), builder, SLOT(setTopToTickPos(int)));
        connect(offset_tick_to_tick_1, SIGNAL(valueChanged(int)), builder, SLOT(setTickToTickPos1(int)));
        connect(offset_tick_to_tick_2, SIGNAL(valueChanged(int)), builder, SLOT(setTickToTickPos2(int)));
        connect(offset_tick_to_text, SIGNAL(valueChanged(int)), builder, SLOT(setTickToTextPos(int)));

        load_pos();

        connect(savePosButton, SIGNAL(clicked()), this, SLOT(save_pos()));


        layout_scene = new QGraphicsScene(layout_preview);
        draw_preview();

        connect(offset_top, SIGNAL(valueChanged(int)), this, SLOT(draw_preview()));
        connect(offset_left, SIGNAL(valueChanged(int)), this, SLOT(draw_preview()));
        connect(offset_tick_1, SIGNAL(valueChanged(int)), this, SLOT(draw_preview()));
        connect(offset_tick_2, SIGNAL(valueChanged(int)), this, SLOT(draw_preview()));
        connect(offset_tick_3, SIGNAL(valueChanged(int)), this, SLOT(draw_preview()));
        connect(offset_tick_4, SIGNAL(valueChanged(int)), this, SLOT(draw_preview()));
        connect(offset_tick_5, SIGNAL(valueChanged(int)), this, SLOT(draw_preview()));
        connect(offset_top_to_tick, SIGNAL(valueChanged(int)), this, SLOT(draw_preview()));
        connect(offset_tick_to_tick_1, SIGNAL(valueChanged(int)), this, SLOT(draw_preview()));
        connect(offset_tick_to_tick_2, SIGNAL(valueChanged(int)), this, SLOT(draw_preview()));
        connect(offset_tick_to_text, SIGNAL(valueChanged(int)), this, SLOT(draw_preview()));

        // Group radio buttons.
        this->groupRadioButtions();

        connect(stack, SIGNAL(clicked()), this, SLOT(stack_text()));
        connect(build, SIGNAL(clicked()), this, SLOT(build_pdf()));
        connect(latex, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(view(int,QProcess::ExitStatus)));

        connect(save, SIGNAL(clicked()), this, SLOT(save_data()));
        connect(load, SIGNAL(clicked()), this, SLOT(load_data()));

        browser->setSource(QUrl("./help.htm"));

        version_label->setText(QString("Grading ") + Presets::version());

        show();
}




MainWindow::~MainWindow()
{
        delete latex;
        delete viewer;
        delete layout_scene;
        delete builder;
}



void MainWindow::closeEvent(QCloseEvent *event)
{
        save_set();
}




QString MainWindow::getText(QString domain, int grade)
{
        if (grade <= 0 || grade > 5) return QString();
        return config.value(QString("grades/%1%2").arg(domain).arg(grade), QString("%1 %2").arg(domainNames[domain]).arg(grade)).toString() + " ";
}


/* Die Funktion stack_text stellt den Beurteilungstext aus Vorgefertigten Sätzen zusammen.
 * Je nachdem welche bewertung in den ComboBoxen festgelegt wurde.
 */
void MainWindow::stack_text()
{
        QString text;

        text += getText("skills", combo_a->currentIndex());
        text += getText("care", combo_b->currentIndex());
        text += getText("interest", combo_c->currentIndex());
        text += getText("teamwork", combo_d->currentIndex());
        text += getText("total", combo_e->currentIndex());

        if (text.isEmpty()) {
                QMessageBox(QMessageBox::Warning,
                            QString::fromUtf8("Fehler"),
                            QString::fromUtf8("Es konnte kein Text zusammengestellt werden. Möglicherweise ist die config Datei fehlerhaft."),
                            QMessageBox::Close, this).exec();
                return;
        }

        if (combo_mw->currentIndex() == 0) {
                text.remove(QRegExp("/[\\w\\s]*\\]")).remove('[');
        } else {
                text.remove(QRegExp("\\[[\\w\\s]*/")).remove(']');
        }

        edit->setPlainText(text);
        tab->setCurrentWidget(tab_edit);
}





void MainWindow::build_pdf()
{
        builder->setText(edit->toPlainText());

        int tick_pos[5];
        tick_pos[0] = offset_tick_1->value();
        tick_pos[1] = offset_tick_2->value();
        tick_pos[2] = offset_tick_3->value();
        tick_pos[3] = offset_tick_4->value();
        tick_pos[4] = offset_tick_5->value();

        builder->setTickPos(0, tick_pos[radioGroupA->checkedId()]);
        builder->setTickPos(1, tick_pos[radioGroupB->checkedId()]);
        builder->setTickPos(2, tick_pos[radioGroupC->checkedId()]);
        builder->setTickPos(3, tick_pos[radioGroupD->checkedId()]);
        builder->setTickPos(4, tick_pos[radioGroupE->checkedId()]);
        builder->setTickPos(5, tick_pos[radioGroupF->checkedId()]);
        builder->setTickPos(6, tick_pos[radioGroupG->checkedId()]);
        builder->setTickPos(7, tick_pos[radioGroupH->checkedId()]);


        builder->build();

        Option config("config", this);
        latex->setWorkingDirectory(QDir::tempPath());
        latex->start(config.getOption("bin_pdflatex"), QStringList("grading.tex"));
        if (!latex->waitForStarted(3000)) {
                QMessageBox(QMessageBox::Warning,
                            QString::fromUtf8("Fehler"),
                            QString::fromUtf8("LaTeX konnte nicht gestartet werden!"),
                            QMessageBox::Close, this).exec();
                viewer->close();
                return;
        }
}




void MainWindow::view(int exitCode, QProcess::ExitStatus exitStatus )
{
        Option config("config", this);

        if ((exitStatus == QProcess::NormalExit) && (exitCode == 0)) {

                if (viewer->state() == QProcess::Running) viewer->close();

                viewer->start(config.getOption("bin_pdfview"), QStringList(QDir::tempPath() + "/grading.pdf"));
                if (!viewer->waitForStarted(3000)) {
                        QMessageBox(QMessageBox::Warning,
                                    QString::fromUtf8("Fehler"),
                                    QString::fromUtf8("Der PDF Betrachter konnte nicht gestartet werden!"),
                                    QMessageBox::Close, this).exec();
                        viewer->close();
                        return;
                }
        } else {
                QMessageBox(QMessageBox::Warning,
                            QString::fromUtf8("Fehler"),
                            QString::fromUtf8("LaTeX wurde nicht ordnugsgemäß beendet!"),
                            QMessageBox::Close, this).exec();
        }
}


void MainWindow::groupRadioButtions()
{
        this->radioGroupA = new QButtonGroup;
        this->radioGroupA->addButton(check_a1, MainWindow::VERY_GOOD);
        this->radioGroupA->addButton(check_a2, MainWindow::GOOD);
        this->radioGroupA->addButton(check_a3, MainWindow::NORMAL);
        this->radioGroupA->addButton(check_a4, MainWindow::BAD);
        this->radioGroupA->addButton(check_a5, MainWindow::VERY_BAD);

        this->radioGroupB = new QButtonGroup;
        this->radioGroupB->addButton(check_b1, MainWindow::VERY_GOOD);
        this->radioGroupB->addButton(check_b2, MainWindow::GOOD);
        this->radioGroupB->addButton(check_b3, MainWindow::NORMAL);
        this->radioGroupB->addButton(check_b4, MainWindow::BAD);
        this->radioGroupB->addButton(check_b5, MainWindow::VERY_BAD);

        this->radioGroupC = new QButtonGroup;
        this->radioGroupC->addButton(check_c1, MainWindow::VERY_GOOD);
        this->radioGroupC->addButton(check_c2, MainWindow::GOOD);
        this->radioGroupC->addButton(check_c3, MainWindow::NORMAL);
        this->radioGroupC->addButton(check_c4, MainWindow::BAD);
        this->radioGroupC->addButton(check_c5, MainWindow::VERY_BAD);

        this->radioGroupD = new QButtonGroup;
        this->radioGroupD->addButton(check_d1, MainWindow::VERY_GOOD);
        this->radioGroupD->addButton(check_d2, MainWindow::GOOD);
        this->radioGroupD->addButton(check_d3, MainWindow::NORMAL);
        this->radioGroupD->addButton(check_d4, MainWindow::BAD);
        this->radioGroupD->addButton(check_d5, MainWindow::VERY_BAD);

        this->radioGroupE = new QButtonGroup;
        this->radioGroupE->addButton(check_e1, MainWindow::VERY_GOOD);
        this->radioGroupE->addButton(check_e2, MainWindow::GOOD);
        this->radioGroupE->addButton(check_e3, MainWindow::NORMAL);
        this->radioGroupE->addButton(check_e4, MainWindow::BAD);
        this->radioGroupE->addButton(check_e5, MainWindow::VERY_BAD);

        this->radioGroupF = new QButtonGroup;
        this->radioGroupF->addButton(check_f1, MainWindow::VERY_GOOD);
        this->radioGroupF->addButton(check_f2, MainWindow::GOOD);
        this->radioGroupF->addButton(check_f3, MainWindow::NORMAL);
        this->radioGroupF->addButton(check_f4, MainWindow::BAD);
        this->radioGroupF->addButton(check_f5, MainWindow::VERY_BAD);

        this->radioGroupG = new QButtonGroup;
        this->radioGroupG->addButton(check_g1, MainWindow::VERY_GOOD);
        this->radioGroupG->addButton(check_g2, MainWindow::GOOD);
        this->radioGroupG->addButton(check_g3, MainWindow::NORMAL);
        this->radioGroupG->addButton(check_g4, MainWindow::BAD);
        this->radioGroupG->addButton(check_g5, MainWindow::VERY_BAD);

        this->radioGroupH = new QButtonGroup;
        this->radioGroupH->addButton(check_h1, MainWindow::VERY_GOOD);
        this->radioGroupH->addButton(check_h2, MainWindow::GOOD);
        this->radioGroupH->addButton(check_h3, MainWindow::NORMAL);
        this->radioGroupH->addButton(check_h4, MainWindow::BAD);
        this->radioGroupH->addButton(check_h5, MainWindow::VERY_BAD);
}



void MainWindow::settingsGradeInit()
{
        QString domain;
        foreach (domain, domainNames.keys()) {
                settingsGradeDomainComboBox->addItem(domainNames[domain], domain);
        }

        connect(this->settingsGradeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsGradeRead()));
        connect(this->settingsGradeDomainComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsGradeRead()));
        connect(this->settingsGradeTextSaveButton, SIGNAL(clicked()), this, SLOT(settingsGradeWrite()));

        settingsGradeRead();
}


void MainWindow::settingsGradeRead()
{
        QString domain = settingsGradeDomainComboBox->itemData(settingsGradeDomainComboBox->currentIndex()).toString();
        int grade = settingsGradeComboBox->currentIndex() + 1;
        this->settingsGradeEdit->setPlainText(getText(domain, grade));

        qDebug() << domain << grade << getText(domain, grade);
}


void MainWindow::settingsGradeWrite()
{
        QString domain = settingsGradeDomainComboBox->itemData(settingsGradeDomainComboBox->currentIndex()).toString();
        int grade = settingsGradeComboBox->currentIndex() + 1;
        config.setValue(QString("grades/%1%2").arg(domain).arg(grade), settingsGradeEdit->toPlainText());
}



void MainWindow::draw_arrow(QGraphicsScene *scene, const QLineF line, const QPen pen)
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




void MainWindow::draw_preview()
{
        qreal px = offset_left->value();
        qreal py = offset_top->value();

        qreal hs = 4;

        qreal hx[5];
        hx[0] = px + offset_tick_1->value();
        hx[1] = px + offset_tick_2->value();
        hx[2] = px + offset_tick_3->value();
        hx[3] = px + offset_tick_4->value();
        hx[4] = px + offset_tick_5->value();

        qreal hy[8] = { 64.0, 74, 84, 104, 114, 124, 144, 154 };
        hy[0] = py + offset_top_to_tick->value();
        hy[1] = hy[0] + hs + 3;
        hy[2] = hy[1] + hs + 3;
        hy[3] = hy[2] + hs + 3 + offset_tick_to_tick_1->value();
        hy[4] = hy[3] + hs + 3;
        hy[5] = hy[4] + hs + 3;
        hy[6] = hy[5] + hs + 3 + offset_tick_to_tick_2->value();
        hy[7] = hy[6] + hs + 3;

        qreal ty = hy[7] + hs + 3 + offset_tick_to_text->value();

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

        layout_preview->setScene(layout_scene);
}




void MainWindow::save_data()
{
        if (save_name.isEmpty()) {
                save_name = save_name_apprentice->text() + " " + QString::number(save_year->value());
        }

        QString filename = QFileDialog::getSaveFileName(this, "Save", save_dir + save_name, "Beurteilung (*.grd)");
        if (filename.isEmpty()) {
                return;
        }

        QFileInfo fn(filename);

        save_dir = fn.dir().path() + "/";

        if (fn.suffix().isEmpty()) {
                filename.append(".grd");
        }

        QSettings save(filename, QSettings::IniFormat, this);

        save.setValue("info/apprentice", save_name_apprentice->text());
        save.setValue("info/instructor", save_name_instructor->text());
        save.setValue("info/year", save_year->value());
        save.setValue("info/begin", save_date1->date().toString());
        save.setValue("info/end", save_date2->date().toString());
        save.setValue("info/comment", save_comment->toPlainText());

        save.setValue("grades/a", radioGroupA->checkedId());
        save.setValue("grades/b", radioGroupB->checkedId());
        save.setValue("grades/c", radioGroupC->checkedId());
        save.setValue("grades/d", radioGroupD->checkedId());
        save.setValue("grades/e", radioGroupE->checkedId());
        save.setValue("grades/f", radioGroupF->checkedId());
        save.setValue("grades/g", radioGroupG->checkedId());
        save.setValue("grades/h", radioGroupH->checkedId());

        save.setValue("assessment/sex", combo_mw->currentIndex());
        save.setValue("assessment/a", combo_a->currentIndex());
        save.setValue("assessment/b", combo_b->currentIndex());
        save.setValue("assessment/c", combo_c->currentIndex());
        save.setValue("assessment/d", combo_d->currentIndex());
        save.setValue("assessment/e", combo_e->currentIndex());
        save.setValue("assessment/text", edit->toPlainText());
}




void MainWindow::load_data()
{
        QString filename = QFileDialog::getOpenFileName(this, "Open", save_dir, "Beurteilung (*.grd)");
        if (filename.isEmpty()) {
                return;
        }

        save_dir = QFileInfo(filename).dir().path() + "/";

        QSettings load(filename, QSettings::IniFormat, this);

        save_name_apprentice->setText(load.value("info/apprentice").toString());
        save_name_instructor->setText(load.value("info/instructor").toString());
        save_year->setValue(load.value("info/year").toInt());
        save_date1->setDate(QDate::fromString(load.value("info/begin").toString()));
        save_date2->setDate(QDate::fromString(load.value("info/end").toString()));
        save_comment->setPlainText(load.value("info/comment").toString());

        radioGroupA->button(load.value("grades/a").toInt())->setChecked(true);
        radioGroupB->button(load.value("grades/b").toInt())->setChecked(true);
        radioGroupC->button(load.value("grades/c").toInt())->setChecked(true);
        radioGroupD->button(load.value("grades/d").toInt())->setChecked(true);
        radioGroupE->button(load.value("grades/e").toInt())->setChecked(true);
        radioGroupF->button(load.value("grades/f").toInt())->setChecked(true);
        radioGroupG->button(load.value("grades/g").toInt())->setChecked(true);
        radioGroupH->button(load.value("grades/h").toInt())->setChecked(true);

        combo_mw->setCurrentIndex(load.value("assessment/sex").toInt());
        combo_a->setCurrentIndex(load.value("assessment/a").toInt());
        combo_b->setCurrentIndex(load.value("assessment/b").toInt());
        combo_c->setCurrentIndex(load.value("assessment/c").toInt());
        combo_d->setCurrentIndex(load.value("assessment/d").toInt());
        combo_e->setCurrentIndex(load.value("assessment/e").toInt());
        edit->setPlainText(load.value("assessment/text").toString());
}





void MainWindow::save_pos()
{
        config.setValue("offset/top", offset_top->value());
        config.setValue("offset/left", offset_left->value());
        config.setValue("offset/tick1", offset_tick_1->value());
        config.setValue("offset/tick2", offset_tick_2->value());
        config.setValue("offset/tick3", offset_tick_3->value());
        config.setValue("offset/tick4", offset_tick_4->value());
        config.setValue("offset/tick5", offset_tick_5->value());
        config.setValue("offset/top_to_tick", offset_top_to_tick->value());
        config.setValue("offset/tick_to_tick1", offset_tick_to_tick_1->value());
        config.setValue("offset/tick_to_tick2", offset_tick_to_tick_2->value());
        config.setValue("offset/tick_to_text", offset_tick_to_text->value());
}




void MainWindow::load_pos()
{
        offset_top->setValue(config.value("offset/top", Presets::topOffset()).toInt());
        offset_left->setValue(config.value("offset/left", Presets::leftOffset()).toInt());
        offset_tick_1->setValue(config.value("offset/tick1", Presets::tickOffset(1)).toInt());
        offset_tick_2->setValue(config.value("offset/tick2", Presets::tickOffset(2)).toInt());
        offset_tick_3->setValue(config.value("offset/tick3", Presets::tickOffset(3)).toInt());
        offset_tick_4->setValue(config.value("offset/tick4", Presets::tickOffset(4)).toInt());
        offset_tick_5->setValue(config.value("offset/tick5", Presets::tickOffset(5)).toInt());
        offset_top_to_tick->setValue(config.value("offset/top_to_tick", Presets::topToTick()).toInt());
        offset_tick_to_tick_1->setValue(config.value("offset/tick_to_tick1", Presets::tickToTick(1)).toInt());
        offset_tick_to_tick_2->setValue(config.value("offset/tick_to_tick2", Presets::tickToTick(2)).toInt());
        offset_tick_to_text->setValue(config.value("offset/tick_to_text", Presets::tickToText()).toInt());
}




void MainWindow::save_set()
{
        config.setValue("dir/save", save_dir);
}




void MainWindow::load_set()
{
        save_dir = config.value("dir/save", Presets::saveDir()).toString();
}


