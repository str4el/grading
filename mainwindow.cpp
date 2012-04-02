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

        ui->saveBeginDateEdit->setDate(QDate(QDate::currentDate().year(), 1, 1));
        ui->saveEndDateEdit->setDate(QDate(QDate::currentDate().year(), 1, 1));

        domainNames.insert("skills", "Fertigkeiten");
        domainNames.insert("care", "Sorgfalt");
        domainNames.insert("interest", "Interesse");
        domainNames.insert("teamwork", "Zusammenarbeit");
        domainNames.insert("total", "Gesamtnote");


        loadSettings();

        settingsGradeInit();

        connect(ui->settingsLatexButton, SIGNAL(clicked()), this, SLOT(settingsLatexFind()));
        connect(ui->settingsPdfButton, SIGNAL(clicked()), this, SLOT(settingsPdfFind()));

        latex = new QProcess(this);
        viewer = new QProcess(this);



        builder = new Build();
        connect(ui->layoutTopOffsetSpinBox, SIGNAL(valueChanged(int)), builder, SLOT(setTopPos(int)));
        connect(ui->layoutLeftOffsetSpinBox, SIGNAL(valueChanged(int)), builder, SLOT(setLeftPos(int)));
        connect(ui->layoutWidthSpinBox, SIGNAL(valueChanged(int)), builder, SLOT(setWidth(int)));
        connect(ui->layoutTopToTickSpinBox, SIGNAL(valueChanged(int)), builder, SLOT(setTopToTickPos(int)));
        connect(ui->layoutTickToTick1SpinBox, SIGNAL(valueChanged(int)), builder, SLOT(setTickToTickPos1(int)));
        connect(ui->layoutTickToTick2SpinBox, SIGNAL(valueChanged(int)), builder, SLOT(setTickToTickPos2(int)));
        connect(ui->layoutTickToTextSpinBox, SIGNAL(valueChanged(int)), builder, SLOT(setTickToTextPos(int)));
        loadPos();


        connect(ui->layoutDefaultsButton, SIGNAL(clicked()), this, SLOT(setDefaultPos()));
        connect(ui->layoutSaveButton, SIGNAL(clicked()), this, SLOT(savePos()));

        layoutScene = new QGraphicsScene(ui->layoutPreView);
        drawPreview();

        connect(ui->layoutTopOffsetSpinBox, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->layoutLeftOffsetSpinBox, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->layoutWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->layoutTick1PosSpinBox, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->layoutTick2PosSpinBox, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->layoutTick3PosSpinBox, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->layoutTick4PosSpinBox, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->layoutTick5PosSpinBox, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->layoutTopToTickSpinBox, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->layoutTickToTick1SpinBox, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->layoutTickToTick2SpinBox, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));
        connect(ui->layoutTickToTextSpinBox, SIGNAL(valueChanged(int)), this, SLOT(drawPreview()));

        // Group radio buttons.
        this->groupRadioButtions();

        connect(ui->stackButton, SIGNAL(clicked()), this, SLOT(stackText()));
        connect(ui->buildButton, SIGNAL(clicked()), this, SLOT(buildPdf()));
        connect(latex, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(viewPdf(int,QProcess::ExitStatus)));

        connect(ui->saveSaveButton, SIGNAL(clicked()), this, SLOT(saveData()));
        connect(ui->saveLoadButton, SIGNAL(clicked()), this, SLOT(loadData()));

        ui->infoBrowser->setSource(QUrl("./help.htm"));

        ui->infoVersionLabel->setText(QString("Grading ") + Presets::version());

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
        saveSettings();
        event->accept();
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

        text += getText("skills", ui->assessmentSkillsCombo->currentIndex());
        text += getText("care", ui->assessmentCareCombo->currentIndex());
        text += getText("interest", ui->assessmentInterestCombo->currentIndex());
        text += getText("teamwork", ui->assessmentTeamworkCombo->currentIndex());
        text += getText("total", ui->assessmentTotalCombo->currentIndex());

        if (text.isEmpty()) {
                QMessageBox(QMessageBox::Warning,
                            QString::fromUtf8("Fehler"),
                            QString::fromUtf8("Es konnte kein Text zusammengestellt werden. Möglicherweise ist die config Datei fehlerhaft."),
                            QMessageBox::Close, this).exec();
                return;
        }

        if (ui->assessmentSexCombo->currentIndex() == 0) {
                text.remove(QRegExp("/[\\w\\s]*\\]")).remove('[');
        } else {
                text.remove(QRegExp("\\[[\\w\\s]*/")).remove(']');
        }

        ui->editTextEdit->setPlainText(text);
        ui->tab->setCurrentWidget(ui->editTab);
}





void MainWindow::buildPdf()
{
        builder->setText(ui->editTextEdit->toPlainText());

        int tickPos[5];
        tickPos[0] = ui->layoutTick1PosSpinBox->value();
        tickPos[1] = ui->layoutTick2PosSpinBox->value();
        tickPos[2] = ui->layoutTick3PosSpinBox->value();
        tickPos[3] = ui->layoutTick4PosSpinBox->value();
        tickPos[4] = ui->layoutTick5PosSpinBox->value();

        builder->setTickPos(0, tickPos[assessmentKnowledgeRadioGroup->checkedId()]);
        builder->setTickPos(1, tickPos[assessmentSkillsRadioGroup->checkedId()]);
        builder->setTickPos(2, tickPos[assessmentSavetyRadioGroup->checkedId()]);
        builder->setTickPos(3, tickPos[assessmentReliabilityRadioGroup->checkedId()]);
        builder->setTickPos(4, tickPos[assessmentActivityRadioGroup->checkedId()]);
        builder->setTickPos(5, tickPos[assessmentProperHandlingRadioGroup->checkedId()]);
        builder->setTickPos(6, tickPos[assessmentTeamworkRadioGroup->checkedId()]);
        builder->setTickPos(7, tickPos[assessmentResponsibilityRadioGroup->checkedId()]);


        ui->statusLabel->setText(QString::fromUtf8("Wird erstellt ..."));
        builder->build();

        latex->setWorkingDirectory(QDir::tempPath());
        latex->start(ui->settingsLatexEdit->text(), QStringList("grading.tex"));
        if (!latex->waitForStarted(3000)) {
                ui->statusLabel->setText(QString::fromUtf8("nicht erfolgreich"));
                QMessageBox(QMessageBox::Warning,
                            QString::fromUtf8("Fehler"),
                            QString::fromUtf8("LaTeX konnte nicht gestartet werden! Einstellungen überprüfen."),
                            QMessageBox::Close, this).exec();
                viewer->close();
                return;
        }
}




void MainWindow::viewPdf(int exitCode, QProcess::ExitStatus exitStatus )
{
        ui->statusLabel->setText(QString::fromUtf8("erfolgreich"));

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
        this->assessmentKnowledgeRadioGroup = new QButtonGroup;
        this->assessmentKnowledgeRadioGroup->addButton(ui->assessmentKnowledge1RadioButton, MainWindow::VERY_GOOD);
        this->assessmentKnowledgeRadioGroup->addButton(ui->assessmentKnowledge2RadioButton, MainWindow::GOOD);
        this->assessmentKnowledgeRadioGroup->addButton(ui->assessmentKnowledge3RadioButton, MainWindow::NORMAL);
        this->assessmentKnowledgeRadioGroup->addButton(ui->assessmentKnowledge4RadioButton, MainWindow::BAD);
        this->assessmentKnowledgeRadioGroup->addButton(ui->assessmentKnowledge5RadioButton, MainWindow::VERY_BAD);

        this->assessmentSkillsRadioGroup = new QButtonGroup;
        this->assessmentSkillsRadioGroup->addButton(ui->assessmentSkills1RadioButton, MainWindow::VERY_GOOD);
        this->assessmentSkillsRadioGroup->addButton(ui->assessmentSkills2RadioButton, MainWindow::GOOD);
        this->assessmentSkillsRadioGroup->addButton(ui->assessmentSkills3RadioButton, MainWindow::NORMAL);
        this->assessmentSkillsRadioGroup->addButton(ui->assessmentSkills4RadioButton, MainWindow::BAD);
        this->assessmentSkillsRadioGroup->addButton(ui->assessmentSkills5RadioButton, MainWindow::VERY_BAD);

        this->assessmentSavetyRadioGroup = new QButtonGroup;
        this->assessmentSavetyRadioGroup->addButton(ui->assessmentSavety1RadioButton, MainWindow::VERY_GOOD);
        this->assessmentSavetyRadioGroup->addButton(ui->assessmentSavety2RadioButton, MainWindow::GOOD);
        this->assessmentSavetyRadioGroup->addButton(ui->assessmentSavety3RadioButton, MainWindow::NORMAL);
        this->assessmentSavetyRadioGroup->addButton(ui->assessmentSavety4RadioButton, MainWindow::BAD);
        this->assessmentSavetyRadioGroup->addButton(ui->assessmentSavety5RadioButton, MainWindow::VERY_BAD);

        this->assessmentReliabilityRadioGroup = new QButtonGroup;
        this->assessmentReliabilityRadioGroup->addButton(ui->assessmentReliability1RadioButton, MainWindow::VERY_GOOD);
        this->assessmentReliabilityRadioGroup->addButton(ui->assessmentReliability2RadioButton, MainWindow::GOOD);
        this->assessmentReliabilityRadioGroup->addButton(ui->assessmentReliability3RadioButton, MainWindow::NORMAL);
        this->assessmentReliabilityRadioGroup->addButton(ui->assessmentReliability4RadioButton, MainWindow::BAD);
        this->assessmentReliabilityRadioGroup->addButton(ui->assessmentReliability5RadioButton, MainWindow::VERY_BAD);

        this->assessmentActivityRadioGroup = new QButtonGroup;
        this->assessmentActivityRadioGroup->addButton(ui->assessmentActivity1RadioButton, MainWindow::VERY_GOOD);
        this->assessmentActivityRadioGroup->addButton(ui->assessmentActivity2RadioButton, MainWindow::GOOD);
        this->assessmentActivityRadioGroup->addButton(ui->assessmentActivity3RadioButton, MainWindow::NORMAL);
        this->assessmentActivityRadioGroup->addButton(ui->assessmentActivity4RadioButton, MainWindow::BAD);
        this->assessmentActivityRadioGroup->addButton(ui->assessmentActivity5RadioButton, MainWindow::VERY_BAD);

        this->assessmentProperHandlingRadioGroup = new QButtonGroup;
        this->assessmentProperHandlingRadioGroup->addButton(ui->assessmentProperHandling1RadioButton, MainWindow::VERY_GOOD);
        this->assessmentProperHandlingRadioGroup->addButton(ui->assessmentProperHandling2RadioButton, MainWindow::GOOD);
        this->assessmentProperHandlingRadioGroup->addButton(ui->assessmentProperHandling3RadioButton, MainWindow::NORMAL);
        this->assessmentProperHandlingRadioGroup->addButton(ui->assessmentProperHandling4RadioButton, MainWindow::BAD);
        this->assessmentProperHandlingRadioGroup->addButton(ui->assessmentProperHandling5RadioButton, MainWindow::VERY_BAD);

        this->assessmentTeamworkRadioGroup = new QButtonGroup;
        this->assessmentTeamworkRadioGroup->addButton(ui->assessmentTeamwork1RadioButton, MainWindow::VERY_GOOD);
        this->assessmentTeamworkRadioGroup->addButton(ui->assessmentTeamwork2RadioButton, MainWindow::GOOD);
        this->assessmentTeamworkRadioGroup->addButton(ui->assessmentTeamwork3RadioButton, MainWindow::NORMAL);
        this->assessmentTeamworkRadioGroup->addButton(ui->assessmentTeamwork4RadioButton, MainWindow::BAD);
        this->assessmentTeamworkRadioGroup->addButton(ui->assessmentTeamwork5RadioButton, MainWindow::VERY_BAD);

        this->assessmentResponsibilityRadioGroup = new QButtonGroup;
        this->assessmentResponsibilityRadioGroup->addButton(ui->assessmentResponsibility1RadioButton, MainWindow::VERY_GOOD);
        this->assessmentResponsibilityRadioGroup->addButton(ui->assessmentResponsibility2RadioButton, MainWindow::GOOD);
        this->assessmentResponsibilityRadioGroup->addButton(ui->assessmentResponsibility3RadioButton, MainWindow::NORMAL);
        this->assessmentResponsibilityRadioGroup->addButton(ui->assessmentResponsibility4RadioButton, MainWindow::BAD);
        this->assessmentResponsibilityRadioGroup->addButton(ui->assessmentResponsibility5RadioButton, MainWindow::VERY_BAD);
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
        qreal px = ui->layoutLeftOffsetSpinBox->value();
        qreal py = ui->layoutTopOffsetSpinBox->value();
        qreal width = ui->layoutWidthSpinBox->value();

        qreal hs = 4;

        qreal hx[5];
        hx[0] = px + ui->layoutTick1PosSpinBox->value();
        hx[1] = px + ui->layoutTick2PosSpinBox->value();
        hx[2] = px + ui->layoutTick3PosSpinBox->value();
        hx[3] = px + ui->layoutTick4PosSpinBox->value();
        hx[4] = px + ui->layoutTick5PosSpinBox->value();

        qreal hy[8] = { 64.0, 74, 84, 104, 114, 124, 144, 154 };
        hy[0] = py + ui->layoutTopToTickSpinBox->value();
        hy[1] = hy[0] + hs + 3;
        hy[2] = hy[1] + hs + 3;
        hy[3] = hy[2] + hs + 3 + ui->layoutTickToTick1SpinBox->value();
        hy[4] = hy[3] + hs + 3;
        hy[5] = hy[4] + hs + 3;
        hy[6] = hy[5] + hs + 3 + ui->layoutTickToTick2SpinBox->value();
        hy[7] = hy[6] + hs + 3;

        qreal ty = hy[7] + hs + 3 + ui->layoutTickToTextSpinBox->value();

        QPen blackSolid(Qt::black);
        QPen blueSolid(Qt::blue);
        QPen blackDash(Qt::black);
        blackDash.setStyle(Qt::DashLine);

        QFont font;
        font.setPixelSize(10);

        layoutScene->clear();

        // Der Rahmen
        layoutScene->addRect(0, 0, 210, 297, blackSolid);
        layoutScene->addRect(px, py, ui->layoutWidthSpinBox->value(), 250, blackDash);

        // Ausenmaße
        drawArrow(layoutScene, QLineF(30, 2, 30, py - 2), blueSolid);
        layoutScene->addText("1", font)->setPos(32, 0);

        drawArrow(layoutScene, QLineF(2, 30, px - 2, 30), blueSolid);
        layoutScene->addText("2", font)->setPos(2, 13);

        drawArrow(layoutScene, QLineF(px, py + 255, px + width, py + 255), blueSolid);
        layoutScene->addText("12", font)->setPos(60, py + 255);


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
        layoutScene->addRect(px, ty, ui->layoutWidthSpinBox->value(), 40, Qt::SolidLine, Qt::Dense4Pattern);

        drawArrow(layoutScene, QLineF(hx[4] + 3, hy[7] + hs + 2, hx[4] + 3, ty - 2), blueSolid);
        layoutScene->addText("11", font)->setPos(hx[4] + 3, hy[7] + hs);

        ui->layoutPreView->setScene(layoutScene);
}




void MainWindow::saveData()
{
        if (saveName.isEmpty()) {
                saveName = ui->saveApprenticeNameEdit->text() + " " + QString::number(ui->saveYearEdit->value());
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

        save.setValue("info/apprentice", ui->saveApprenticeNameEdit->text());
        save.setValue("info/instructor", ui->saveInstructorNameEdit->text());
        save.setValue("info/year", ui->saveYearEdit->value());
        save.setValue("info/begin", ui->saveBeginDateEdit->date().toString());
        save.setValue("info/end", ui->saveEndDateEdit->date().toString());
        save.setValue("info/comment", ui->saveCommentEdit->toPlainText());

        save.setValue("grades/knowledge", assessmentKnowledgeRadioGroup->checkedId());
        save.setValue("grades/skills", assessmentSkillsRadioGroup->checkedId());
        save.setValue("grades/safety", assessmentSavetyRadioGroup->checkedId());
        save.setValue("grades/reliability", assessmentReliabilityRadioGroup->checkedId());
        save.setValue("grades/activity", assessmentActivityRadioGroup->checkedId());
        save.setValue("grades/proper_handling", assessmentProperHandlingRadioGroup->checkedId());
        save.setValue("grades/teamwork", assessmentTeamworkRadioGroup->checkedId());
        save.setValue("grades/responsibility", assessmentResponsibilityRadioGroup->checkedId());

        save.setValue("assessment/sex", ui->assessmentSexCombo->currentIndex());
        save.setValue("assessment/skills", ui->assessmentSkillsCombo->currentIndex());
        save.setValue("assessment/care", ui->assessmentCareCombo->currentIndex());
        save.setValue("assessment/interest", ui->assessmentInterestCombo->currentIndex());
        save.setValue("assessment/teamwork", ui->assessmentTeamworkCombo->currentIndex());
        save.setValue("assessment/total", ui->assessmentTotalCombo->currentIndex());
        save.setValue("assessment/text", ui->editTextEdit->toPlainText());
}




void MainWindow::loadData()
{
        QString filename = QFileDialog::getOpenFileName(this, "Open", saveDir, "Beurteilung (*.grd)");
        if (filename.isEmpty()) {
                return;
        }

        saveDir = QFileInfo(filename).dir().path() + "/";

        QSettings load(filename, QSettings::IniFormat, this);

        ui->saveApprenticeNameEdit->setText(load.value("info/apprentice").toString());
        ui->saveInstructorNameEdit->setText(load.value("info/instructor").toString());
        ui->saveYearEdit->setValue(load.value("info/year").toInt());
        ui->saveBeginDateEdit->setDate(QDate::fromString(load.value("info/begin").toString()));
        ui->saveEndDateEdit->setDate(QDate::fromString(load.value("info/end").toString()));
        ui->saveCommentEdit->setPlainText(load.value("info/comment").toString());

        assessmentKnowledgeRadioGroup->button(load.value("grades/knowledge").toInt())->setChecked(true);
        assessmentSkillsRadioGroup->button(load.value("grades/skills").toInt())->setChecked(true);
        assessmentSavetyRadioGroup->button(load.value("grades/safety").toInt())->setChecked(true);
        assessmentReliabilityRadioGroup->button(load.value("grades/reliability").toInt())->setChecked(true);
        assessmentActivityRadioGroup->button(load.value("grades/activity").toInt())->setChecked(true);
        assessmentProperHandlingRadioGroup->button(load.value("grades/proper_handling").toInt())->setChecked(true);
        assessmentTeamworkRadioGroup->button(load.value("grades/teamwork").toInt())->setChecked(true);
        assessmentResponsibilityRadioGroup->button(load.value("grades/responsibility").toInt())->setChecked(true);

        ui->assessmentSexCombo->setCurrentIndex(load.value("assessment/sex").toInt());
        ui->assessmentSkillsCombo->setCurrentIndex(load.value("assessment/skills").toInt());
        ui->assessmentCareCombo->setCurrentIndex(load.value("assessment/care").toInt());
        ui->assessmentInterestCombo->setCurrentIndex(load.value("assessment/interest").toInt());
        ui->assessmentTeamworkCombo->setCurrentIndex(load.value("assessment/teamwork").toInt());
        ui->assessmentTotalCombo->setCurrentIndex(load.value("assessment/total").toInt());
        ui->editTextEdit->setPlainText(load.value("assessment/text").toString());
}





void MainWindow::saveSettings()
{
        config.setValue("gui/width", this->size().width());
        config.setValue("gui/height", this->size().height());

        config.setValue("path/save", saveDir);
        config.setValue("path/latex", ui->settingsLatexEdit->text());
        config.setValue("path/pdf", ui->settingsPdfEdit->text());
}





void MainWindow::loadSettings()
{
        QSize guiSize;
        guiSize.setWidth(config.value("gui/width", this->sizeHint().width()).toInt());
        guiSize.setHeight(config.value("gui/height", this->sizeHint().height()).toInt());
        this->resize(guiSize);

        saveDir = config.value("path/save", Presets::saveDir()).toString();
        ui->settingsLatexEdit->setText(config.value("path/latex").toString());
        ui->settingsPdfEdit->setText(config.value("path/pdf").toString());
}




void MainWindow::setDefaultPos()
{
        ui->layoutTopOffsetSpinBox->setValue(Presets::topOffset());
        ui->layoutLeftOffsetSpinBox->setValue(Presets::leftOffset());
        ui->layoutWidthSpinBox->setValue(Presets::width());
        ui->layoutTick1PosSpinBox->setValue(Presets::tickOffset(1));
        ui->layoutTick2PosSpinBox->setValue(Presets::tickOffset(2));
        ui->layoutTick3PosSpinBox->setValue(Presets::tickOffset(3));
        ui->layoutTick4PosSpinBox->setValue(Presets::tickOffset(4));
        ui->layoutTick5PosSpinBox->setValue(Presets::tickOffset(5));
        ui->layoutTopToTickSpinBox->setValue(Presets::topToTick());
        ui->layoutTickToTick1SpinBox->setValue(Presets::tickToTick(1));
        ui->layoutTickToTick2SpinBox->setValue(Presets::tickToTick(2));
        ui->layoutTickToTextSpinBox->setValue(Presets::tickToText());
}


void MainWindow::savePos()
{
        config.setValue("position/top", ui->layoutTopOffsetSpinBox->value());
        config.setValue("position/left", ui->layoutLeftOffsetSpinBox->value());
        config.setValue("position/width", ui->layoutWidthSpinBox->value());
        config.setValue("position/tick1", ui->layoutTick1PosSpinBox->value());
        config.setValue("position/tick2", ui->layoutTick2PosSpinBox->value());
        config.setValue("position/tick3", ui->layoutTick3PosSpinBox->value());
        config.setValue("position/tick4", ui->layoutTick4PosSpinBox->value());
        config.setValue("position/tick5", ui->layoutTick5PosSpinBox->value());
        config.setValue("position/top_to_tick", ui->layoutTopToTickSpinBox->value());
        config.setValue("position/tick_to_tick1", ui->layoutTickToTick1SpinBox->value());
        config.setValue("position/tick_to_tick2", ui->layoutTickToTick2SpinBox->value());
        config.setValue("position/tick_to_text", ui->layoutTickToTextSpinBox->value());
}




void MainWindow::loadPos()
{
        ui->layoutTopOffsetSpinBox->setValue(config.value("position/top", Presets::topOffset()).toInt());
        ui->layoutLeftOffsetSpinBox->setValue(config.value("position/left", Presets::leftOffset()).toInt());
        ui->layoutWidthSpinBox->setValue(config.value("position/Width", Presets::width()).toInt());
        ui->layoutTick1PosSpinBox->setValue(config.value("position/tick1", Presets::tickOffset(1)).toInt());
        ui->layoutTick2PosSpinBox->setValue(config.value("position/tick2", Presets::tickOffset(2)).toInt());
        ui->layoutTick3PosSpinBox->setValue(config.value("position/tick3", Presets::tickOffset(3)).toInt());
        ui->layoutTick4PosSpinBox->setValue(config.value("position/tick4", Presets::tickOffset(4)).toInt());
        ui->layoutTick5PosSpinBox->setValue(config.value("position/tick5", Presets::tickOffset(5)).toInt());
        ui->layoutTopToTickSpinBox->setValue(config.value("position/top_to_tick", Presets::topToTick()).toInt());
        ui->layoutTickToTick1SpinBox->setValue(config.value("position/tick_to_tick1", Presets::tickToTick(1)).toInt());
        ui->layoutTickToTick2SpinBox->setValue(config.value("position/tick_to_tick2", Presets::tickToTick(2)).toInt());
        ui->layoutTickToTextSpinBox->setValue(config.value("position/tick_to_text", Presets::tickToText()).toInt());
}


