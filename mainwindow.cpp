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

        connect(ui->stackButton, SIGNAL(clicked()), this, SLOT(stackText()));

        connect(ui->saveSaveButton, SIGNAL(clicked()), this, SLOT(saveData()));
        connect(ui->saveLoadButton, SIGNAL(clicked()), this, SLOT(loadData()));

        ui->infoBrowser->setSource(QUrl("./help.htm"));

        ui->infoVersionLabel->setText(QString("Grading ") + Presets::instance().programVersion());


        show();
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

        saveDir = config.value("path/save", QDir::homePath()).toString();
        ui->settingsLatexEdit->setText(config.value("path/latex").toString());
        ui->settingsPdfEdit->setText(config.value("path/pdf").toString());
}











