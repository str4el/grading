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
#include "ui_mainwindow.h"
#include "presets.h"
#include "build.h"

MainWindow::MainWindow(QWidget *parrent) :
                QMainWindow(parrent),
                ui(new Ui::MainWindow),
                config("config.ini", QSettings::IniFormat, this)
{
        ui->setupUi(this);

        domainNames.insert("skills", "Fertigkeiten");
        domainNames.insert("care", "Sorgfalt");
        domainNames.insert("interest", "Interesse");
        domainNames.insert("teamwork", "Zusammenarbeit");
        domainNames.insert("total", "Gesamtnote");


        saveDir = config.value("path/save", Presets::saveDir()).toString();
        ui->settingsLatexEdit->setText(config.value("path/latex").toString());
        ui->settingsPdfEdit->setText(config.value("path/pdf").toString());
        settingsGradeInit();
        loadPos();

        connect(ui->settingsLatexButton, SIGNAL(clicked()), this, SLOT(settingsLatexFind()));
        connect(ui->settingsPdfButton, SIGNAL(clicked()), this, SLOT(settingsPdfFind()));

        latex = new QProcess(this);
        viewer = new QProcess(this);



        builder = new Build();
        connect(ui->offsetTop, SIGNAL(valueChanged(int)), builder, SLOT(setTopPos(int)));
        connect(ui->offsetLeft, SIGNAL(valueChanged(int)), builder, SLOT(setLeftPos(int)));
        connect(ui->offsetTopToTick, SIGNAL(valueChanged(int)), builder, SLOT(setTopToTickPos(int)));
        connect(ui->offsetTickToTick1, SIGNAL(valueChanged(int)), builder, SLOT(setTickToTickPos1(int)));
        connect(ui->offsetTickToTick2, SIGNAL(valueChanged(int)), builder, SLOT(setTickToTickPos2(int)));
        connect(ui->offsetTickToText, SIGNAL(valueChanged(int)), builder, SLOT(setTickToTextPos(int)));


        connect(ui->savePosButton, SIGNAL(clicked()), this, SLOT(savePos()));


        layoutScene = new QGraphicsScene(ui->layoutPreview);
        drawPreview();

        connect(ui->offsetTop, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->offsetLeft, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->offsetTick1, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->offsetTick2, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->offsetTick3, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->offsetTick4, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->offsetTick5, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->offsetTopToTick, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->offsetTickToTick1, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->offsetTickToTick2, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->offsetTickToText, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));

        // Group radio buttons.
        this->groupRadioButtions();

        connect(ui->stack, SIGNAL(clicked()), this, SLOT(stackText()));
        connect(ui->build, SIGNAL(clicked()), this, SLOT(buildPdf()));
        connect(latex, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(view(int,QProcess::ExitStatus)));

        connect(ui->save, SIGNAL(clicked()), this, SLOT(saveData()));
        connect(ui->load, SIGNAL(clicked()), this, SLOT(loadData()));

        ui->browser->setSource(QUrl("./help.htm"));

        ui->versionLabel->setText(QString("Grading ") + Presets::version());

        show();
}




MainWindow::~MainWindow()
{
        delete latex;
        delete viewer;
        delete layoutScene;
        delete builder;
}



void MainWindow::closeEvent(QCloseEvent *event)
{
        config.setValue("path/save", saveDir);
        config.setValue("path/latex", ui->settingsLatexEdit->text());
        config.setValue("path/pdf", ui->settingsPdfEdit->text());
}




QString MainWindow::getText(QString domain, int grade)
{
        if (grade <= 0 || grade > 5) return QString();
        return config.value(QString("grades/%1%2").arg(domain).arg(grade), QString("%1 %2").arg(domainNames[domain]).arg(grade)).toString() + " ";
}


/* Die Funktion stackText stellt den Beurteilungstext aus Vorgefertigten Sätzen zusammen.
 * Je nachdem welche bewertung in den ComboBoxen festgelegt wurde.
 */
void MainWindow::stackText()
{
        QString text;

        text += getText("skills", ui->comboA->currentIndex());
        text += getText("care", ui->comboB->currentIndex());
        text += getText("interest", ui->comboC->currentIndex());
        text += getText("teamwork", ui->comboD->currentIndex());
        text += getText("total", ui->comboE->currentIndex());

        if (text.isEmpty()) {
                QMessageBox(QMessageBox::Warning,
                            QString::fromUtf8("Fehler"),
                            QString::fromUtf8("Es konnte kein Text zusammengestellt werden. Möglicherweise ist die config Datei fehlerhaft."),
                            QMessageBox::Close, this).exec();
                return;
        }

        if (ui->comboMw->currentIndex() == 0) {
                text.remove(QRegExp("/[\\w\\s]*\\]")).remove('[');
        } else {
                text.remove(QRegExp("\\[[\\w\\s]*/")).remove(']');
        }

        ui->edit->setPlainText(text);
        ui->tab->setCurrentWidget(ui->tabEdit);
}





void MainWindow::buildPdf()
{
        builder->setText(ui->edit->toPlainText());

        int tickPos[5];
        tickPos[0] = ui->offsetTick1->value();
        tickPos[1] = ui->offsetTick2->value();
        tickPos[2] = ui->offsetTick3->value();
        tickPos[3] = ui->offsetTick4->value();
        tickPos[4] = ui->offsetTick5->value();

        builder->setTickPos(0, tickPos[radioGroupA->checkedId()]);
        builder->setTickPos(1, tickPos[radioGroupB->checkedId()]);
        builder->setTickPos(2, tickPos[radioGroupC->checkedId()]);
        builder->setTickPos(3, tickPos[radioGroupD->checkedId()]);
        builder->setTickPos(4, tickPos[radioGroupE->checkedId()]);
        builder->setTickPos(5, tickPos[radioGroupF->checkedId()]);
        builder->setTickPos(6, tickPos[radioGroupG->checkedId()]);
        builder->setTickPos(7, tickPos[radioGroupH->checkedId()]);


        builder->build();

        latex->setWorkingDirectory(QDir::tempPath());
        latex->start(ui->settingsLatexEdit->text(), QStringList("grading.tex"));
        if (!latex->waitForStarted(3000)) {
                QMessageBox(QMessageBox::Warning,
                            QString::fromUtf8("Fehler"),
                            QString::fromUtf8("LaTeX konnte nicht gestartet werden! Einstellungen überprüfen."),
                            QMessageBox::Close, this).exec();
                viewer->close();
                return;
        }
}




void MainWindow::view(int exitCode, QProcess::ExitStatus exitStatus )
{
        if ((exitStatus == QProcess::NormalExit) && (exitCode == 0)) {

                if (viewer->state() == QProcess::Running) viewer->close();

                viewer->start(ui->settingsPdfEdit->text(), QStringList(QDir::tempPath() + "/grading.pdf"));
                if (!viewer->waitForStarted(3000)) {
                        QMessageBox(QMessageBox::Warning,
                                    QString::fromUtf8("Fehler"),
                                    QString::fromUtf8("Der PDF Betrachter konnte nicht gestartet werden! Einstellungen überprüfen"),
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
        this->radioGroupA->addButton(ui->checkA1, MainWindow::VERY_GOOD);
        this->radioGroupA->addButton(ui->checkA2, MainWindow::GOOD);
        this->radioGroupA->addButton(ui->checkA3, MainWindow::NORMAL);
        this->radioGroupA->addButton(ui->checkA4, MainWindow::BAD);
        this->radioGroupA->addButton(ui->checkA5, MainWindow::VERY_BAD);

        this->radioGroupB = new QButtonGroup;
        this->radioGroupB->addButton(ui->checkB1, MainWindow::VERY_GOOD);
        this->radioGroupB->addButton(ui->checkB2, MainWindow::GOOD);
        this->radioGroupB->addButton(ui->checkB3, MainWindow::NORMAL);
        this->radioGroupB->addButton(ui->checkB4, MainWindow::BAD);
        this->radioGroupB->addButton(ui->checkB5, MainWindow::VERY_BAD);

        this->radioGroupC = new QButtonGroup;
        this->radioGroupC->addButton(ui->checkC1, MainWindow::VERY_GOOD);
        this->radioGroupC->addButton(ui->checkC2, MainWindow::GOOD);
        this->radioGroupC->addButton(ui->checkC3, MainWindow::NORMAL);
        this->radioGroupC->addButton(ui->checkC4, MainWindow::BAD);
        this->radioGroupC->addButton(ui->checkC5, MainWindow::VERY_BAD);

        this->radioGroupD = new QButtonGroup;
        this->radioGroupD->addButton(ui->checkD1, MainWindow::VERY_GOOD);
        this->radioGroupD->addButton(ui->checkD2, MainWindow::GOOD);
        this->radioGroupD->addButton(ui->checkD3, MainWindow::NORMAL);
        this->radioGroupD->addButton(ui->checkD4, MainWindow::BAD);
        this->radioGroupD->addButton(ui->checkD5, MainWindow::VERY_BAD);

        this->radioGroupE = new QButtonGroup;
        this->radioGroupE->addButton(ui->checkE1, MainWindow::VERY_GOOD);
        this->radioGroupE->addButton(ui->checkE2, MainWindow::GOOD);
        this->radioGroupE->addButton(ui->checkE3, MainWindow::NORMAL);
        this->radioGroupE->addButton(ui->checkE4, MainWindow::BAD);
        this->radioGroupE->addButton(ui->checkE5, MainWindow::VERY_BAD);

        this->radioGroupF = new QButtonGroup;
        this->radioGroupF->addButton(ui->checkF1, MainWindow::VERY_GOOD);
        this->radioGroupF->addButton(ui->checkF2, MainWindow::GOOD);
        this->radioGroupF->addButton(ui->checkF3, MainWindow::NORMAL);
        this->radioGroupF->addButton(ui->checkF4, MainWindow::BAD);
        this->radioGroupF->addButton(ui->checkF5, MainWindow::VERY_BAD);

        this->radioGroupG = new QButtonGroup;
        this->radioGroupG->addButton(ui->checkG1, MainWindow::VERY_GOOD);
        this->radioGroupG->addButton(ui->checkG2, MainWindow::GOOD);
        this->radioGroupG->addButton(ui->checkG3, MainWindow::NORMAL);
        this->radioGroupG->addButton(ui->checkG4, MainWindow::BAD);
        this->radioGroupG->addButton(ui->checkG5, MainWindow::VERY_BAD);

        this->radioGroupH = new QButtonGroup;
        this->radioGroupH->addButton(ui->checkH1, MainWindow::VERY_GOOD);
        this->radioGroupH->addButton(ui->checkH2, MainWindow::GOOD);
        this->radioGroupH->addButton(ui->checkH3, MainWindow::NORMAL);
        this->radioGroupH->addButton(ui->checkH4, MainWindow::BAD);
        this->radioGroupH->addButton(ui->checkH5, MainWindow::VERY_BAD);
}




void MainWindow::settingsLatexFind()
{
        QString filename = QFileDialog::getOpenFileName(this, "Suche nach Latex", ui->settingsLatexEdit->text());
        if (filename.isEmpty()) {
                return;
        }
        ui->settingsLatexEdit->setText(filename);
}





void MainWindow::settingsPdfFind()
{
        QString filename = QFileDialog::getOpenFileName(this, "Suche nach PDF Betrachter", ui->settingsPdfEdit->text());
        if (filename.isEmpty()) {
                return;
        }
        ui->settingsPdfEdit->setText(filename);
}




void MainWindow::settingsGradeInit()
{
        QString domain;
        foreach (domain, domainNames.keys()) {
                ui->settingsGradeDomainComboBox->addItem(domainNames[domain], domain);
        }

        connect(ui->settingsGradeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsGradeRead()));
        connect(ui->settingsGradeDomainComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(settingsGradeRead()));
        connect(ui->settingsGradeTextSaveButton, SIGNAL(clicked()), this, SLOT(settingsGradeWrite()));

        settingsGradeRead();
}





void MainWindow::settingsGradeRead()
{
        QString domain = ui->settingsGradeDomainComboBox->itemData(ui->settingsGradeDomainComboBox->currentIndex()).toString();
        int grade = ui->settingsGradeComboBox->currentIndex() + 1;
        ui->settingsGradeEdit->setPlainText(getText(domain, grade));
}




void MainWindow::settingsGradeWrite()
{
        QString domain = ui->settingsGradeDomainComboBox->itemData(ui->settingsGradeDomainComboBox->currentIndex()).toString();
        int grade = ui->settingsGradeComboBox->currentIndex() + 1;
        config.setValue(QString("grades/%1%2").arg(domain).arg(grade), ui->settingsGradeEdit->toPlainText());
}




void MainWindow::drawArrow(QGraphicsScene *scene, const QLineF line, const QPen pen)
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




void MainWindow::drawPreview()
{
        qreal px = ui->offsetLeft->value();
        qreal py = ui->offsetTop->value();

        qreal hs = 4;

        qreal hx[5];
        hx[0] = px + ui->offsetTick1->value();
        hx[1] = px + ui->offsetTick2->value();
        hx[2] = px + ui->offsetTick3->value();
        hx[3] = px + ui->offsetTick4->value();
        hx[4] = px + ui->offsetTick5->value();

        qreal hy[8] = { 64.0, 74, 84, 104, 114, 124, 144, 154 };
        hy[0] = py + ui->offsetTopToTick->value();
        hy[1] = hy[0] + hs + 3;
        hy[2] = hy[1] + hs + 3;
        hy[3] = hy[2] + hs + 3 + ui->offsetTickToTick1->value();
        hy[4] = hy[3] + hs + 3;
        hy[5] = hy[4] + hs + 3;
        hy[6] = hy[5] + hs + 3 + ui->offsetTickToTick2->value();
        hy[7] = hy[6] + hs + 3;

        qreal ty = hy[7] + hs + 3 + ui->offsetTickToText->value();

        QPen blackSolid(Qt::black);
        QPen blueSolid(Qt::blue);
        QPen blackDash(Qt::black);
        blackDash.setStyle(Qt::DashLine);

        QFont font;
        font.setPixelSize(10);

        layoutScene->clear();

        // Der Rahmen
        layoutScene->addRect(0, 0, 210, 297, blackSolid);
        layoutScene->addRect(px, py, 184, 250, blackDash);

        // Ausenmaße
        drawArrow(layoutScene, QLineF(30, 2, 30, py - 2), blueSolid);
        layoutScene->addText("1", font)->setPos(32, 0);

        drawArrow(layoutScene, QLineF(2, 30, px - 2, 30), blueSolid);
        layoutScene->addText("2", font)->setPos(2, 13);


        // Multiple choice
        for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 8; j++) {
                        layoutScene->addRect(hx[i], hy[j], hs, hs, Qt::SolidLine, Qt::Dense4Pattern);
                }
        }

        drawArrow(layoutScene, QLineF(px + 2, hy[0] + 2, hx[0] - 2, hy[0] + 2), blueSolid);
        drawArrow(layoutScene, QLineF(px + 2, hy[0] - 4, hx[1] - 2, hy[0] - 4), blueSolid);
        drawArrow(layoutScene, QLineF(px + 2, hy[0] - 10, hx[2] - 2, hy[0] - 10), blueSolid);
        drawArrow(layoutScene, QLineF(px + 2, hy[0] - 16, hx[3] - 2, hy[0] - 16), blueSolid);
        drawArrow(layoutScene, QLineF(px + 2, hy[0] - 22, hx[4] - 2, hy[0] - 22), blueSolid);

        layoutScene->addText("3", font)->setPos(60, hy[0] + 2);
        layoutScene->addText("7", font)->setPos(60, hy[0] - 38);

        drawArrow(layoutScene, QLineF(hx[4] + 3, py + 2, hx[4] + 3, hy[0] - 2), blueSolid);
        layoutScene->addText("8", font)->setPos(hx[4] + 5, 35);

        drawArrow(layoutScene, QLineF(hx[4] + 3, hy[2] + hs + 2, hx[4] + 3, hy[3] - 2), blueSolid);
        layoutScene->addText("9", font)->setPos(hx[4] + 5, hy[2] + hs - 2);

        drawArrow(layoutScene, QLineF(hx[4] + 3, hy[5] + hs + 2, hx[4] + 3, hy[6] - 2), blueSolid);
        layoutScene->addText("10", font)->setPos(hx[4] + 3, hy[5] + hs - 2);

        // Text
        layoutScene->addRect(px + 2, ty, 180, 40, Qt::SolidLine, Qt::Dense4Pattern);

        drawArrow(layoutScene, QLineF(hx[4] + 3, hy[7] + hs + 2, hx[4] + 3, ty - 2), blueSolid);
        layoutScene->addText("11", font)->setPos(hx[4] + 3, hy[7] + hs);

        ui->layoutPreview->setScene(layoutScene);
}




void MainWindow::saveData()
{
        if (saveName.isEmpty()) {
                saveName = ui->saveNameApprentice->text() + " " + QString::number(ui->saveYear->value());
        }

        QString filename = QFileDialog::getSaveFileName(this, "Save", saveDir + saveName, "Beurteilung (*.grd)");
        if (filename.isEmpty()) {
                return;
        }

        QFileInfo fn(filename);

        saveDir = fn.dir().path() + "/";

        if (fn.suffix().isEmpty()) {
                filename.append(".grd");
        }

        QSettings save(filename, QSettings::IniFormat, this);

        save.setValue("info/apprentice", ui->saveNameApprentice->text());
        save.setValue("info/instructor", ui->saveNameInstructor->text());
        save.setValue("info/year", ui->saveYear->value());
        save.setValue("info/begin", ui->saveDate1->date().toString());
        save.setValue("info/end", ui->saveDate2->date().toString());
        save.setValue("info/comment", ui->saveComment->toPlainText());

        save.setValue("grades/knowledge", radioGroupA->checkedId());
        save.setValue("grades/skills", radioGroupB->checkedId());
        save.setValue("grades/safety", radioGroupC->checkedId());
        save.setValue("grades/reliability", radioGroupD->checkedId());
        save.setValue("grades/activity", radioGroupE->checkedId());
        save.setValue("grades/proper_handling", radioGroupF->checkedId());
        save.setValue("grades/teamwork", radioGroupG->checkedId());
        save.setValue("grades/responsibility", radioGroupH->checkedId());

        save.setValue("assessment/sex", ui->comboMw->currentIndex());
        save.setValue("assessment/skills", ui->comboA->currentIndex());
        save.setValue("assessment/care", ui->comboB->currentIndex());
        save.setValue("assessment/interest", ui->comboC->currentIndex());
        save.setValue("assessment/teamwork", ui->comboD->currentIndex());
        save.setValue("assessment/total", ui->comboE->currentIndex());
        save.setValue("assessment/text", ui->edit->toPlainText());
}




void MainWindow::loadData()
{
        QString filename = QFileDialog::getOpenFileName(this, "Open", saveDir, "Beurteilung (*.grd)");
        if (filename.isEmpty()) {
                return;
        }

        saveDir = QFileInfo(filename).dir().path() + "/";

        QSettings load(filename, QSettings::IniFormat, this);

        ui->saveNameApprentice->setText(load.value("info/apprentice").toString());
        ui->saveNameInstructor->setText(load.value("info/instructor").toString());
        ui->saveYear->setValue(load.value("info/year").toInt());
        ui->saveDate1->setDate(QDate::fromString(load.value("info/begin").toString()));
        ui->saveDate2->setDate(QDate::fromString(load.value("info/end").toString()));
        ui->saveComment->setPlainText(load.value("info/comment").toString());

        radioGroupA->button(load.value("grades/knowledge").toInt())->setChecked(true);
        radioGroupB->button(load.value("grades/skills").toInt())->setChecked(true);
        radioGroupC->button(load.value("grades/safety").toInt())->setChecked(true);
        radioGroupD->button(load.value("grades/reliability").toInt())->setChecked(true);
        radioGroupE->button(load.value("grades/activity").toInt())->setChecked(true);
        radioGroupF->button(load.value("grades/proper_handling").toInt())->setChecked(true);
        radioGroupG->button(load.value("grades/teamwork").toInt())->setChecked(true);
        radioGroupH->button(load.value("grades/responsibility").toInt())->setChecked(true);

        ui->comboMw->setCurrentIndex(load.value("assessment/sex").toInt());
        ui->comboA->setCurrentIndex(load.value("assessment/skills").toInt());
        ui->comboB->setCurrentIndex(load.value("assessment/care").toInt());
        ui->comboC->setCurrentIndex(load.value("assessment/interest").toInt());
        ui->comboD->setCurrentIndex(load.value("assessment/teamwork").toInt());
        ui->comboE->setCurrentIndex(load.value("assessment/total").toInt());
        ui->edit->setPlainText(load.value("assessment/text").toString());
}





void MainWindow::savePos()
{
        config.setValue("position/top", ui->offsetTop->value());
        config.setValue("position/left", ui->offsetLeft->value());
        config.setValue("position/tick1", ui->offsetTick1->value());
        config.setValue("position/tick2", ui->offsetTick2->value());
        config.setValue("position/tick3", ui->offsetTick3->value());
        config.setValue("position/tick4", ui->offsetTick4->value());
        config.setValue("position/tick5", ui->offsetTick5->value());
        config.setValue("position/top_to_tick", ui->offsetTopToTick->value());
        config.setValue("position/tick_to_tick1", ui->offsetTickToTick1->value());
        config.setValue("position/tick_to_tick2", ui->offsetTickToTick2->value());
        config.setValue("position/tick_to_text", ui->offsetTickToText->value());
}




void MainWindow::loadPos()
{
        ui->offsetTop->setValue(config.value("position/top", Presets::topOffset()).toInt());
        ui->offsetLeft->setValue(config.value("position/left", Presets::leftOffset()).toInt());
        ui->offsetTick1->setValue(config.value("position/tick1", Presets::tickOffset(1)).toInt());
        ui->offsetTick2->setValue(config.value("position/tick2", Presets::tickOffset(2)).toInt());
        ui->offsetTick3->setValue(config.value("position/tick3", Presets::tickOffset(3)).toInt());
        ui->offsetTick4->setValue(config.value("position/tick4", Presets::tickOffset(4)).toInt());
        ui->offsetTick5->setValue(config.value("position/tick5", Presets::tickOffset(5)).toInt());
        ui->offsetTopToTick->setValue(config.value("position/top_to_tick", Presets::topToTick()).toInt());
        ui->offsetTickToTick1->setValue(config.value("position/tick_to_tick1", Presets::tickToTick(1)).toInt());
        ui->offsetTickToTick2->setValue(config.value("position/tick_to_tick2", Presets::tickToTick(2)).toInt());
        ui->offsetTickToText->setValue(config.value("position/tick_to_text", Presets::tickToText()).toInt());
}


