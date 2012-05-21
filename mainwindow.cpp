/*
 *
 *  Copyright (C) 2009 - 2012 Stephan Reinhard <Stephan-Reinhard@gmx.de>
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
#include <QPrintDialog>
#include <QFontDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "presets.h"
#include "layout.h"
#include "painter.h"
#include "fileconverter.h"

MainWindow::MainWindow(QWidget *parrent) :
        QMainWindow(parrent),
        ui(new Ui::MainWindow),
        config("grading.ini", QSettings::IniFormat, this),
        mAssessmentTextLayoutActive(false)
{
        ui->setupUi(this);

        mLayout = new Layout(this);
        ui->previewWidget->setLayout(mLayout);
        connect (ui->tab, SIGNAL(currentChanged(int)), this, SLOT(updateLayout()));

        ui->saveBeginDateEdit->setDate(QDate(QDate::currentDate().year(), 1, 1));
        ui->saveEndDateEdit->setDate(QDate(QDate::currentDate().year(), 1, 1));

        domainNames.insert("skills", "Fertigkeiten");
        domainNames.insert("care", "Sorgfalt");
        domainNames.insert("interest", "Interesse");
        domainNames.insert("teamwork", "Zusammenarbeit");
        domainNames.insert("total", "Gesamtnote");

        loadSettings();
        loadLayout();
        settingsGradeInit();

        connect(ui->stackButton, SIGNAL(clicked()), this, SLOT(stackText()));
        connect(ui->printButton, SIGNAL(clicked()), this, SLOT(print()));

        connect(ui->saveSaveButton, SIGNAL(clicked()), this, SLOT(saveData()));
        connect(ui->saveLoadButton, SIGNAL(clicked()), this, SLOT(loadData()));

        connect(ui->layoutSaveButton, SIGNAL(clicked()), this, SLOT(saveLayout()));
        connect(ui->layoutLoadButton, SIGNAL(clicked()), this, SLOT(loadLayout()));
        connect(ui->layoutDefaultsButton, SIGNAL(clicked()), this, SLOT(setDefaultLayout()));


        // Verbindungen für die Layoutmainpulation
        connect(ui->layoutXOffsetSpinBox, SIGNAL(valueChanged(int)), mLayout, SLOT(setXOffset(int)));
        connect(ui->layoutYOffsetSpinBox, SIGNAL(valueChanged(int)), mLayout, SLOT(setYOffest(int)));
        connect(ui->layoutPosXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateLayoutXPos(int)));
        connect(ui->layoutPosYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateLayoutYPos(int)));
        connect(ui->layoutWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateLayoutTextRect()));
        connect(ui->layoutHeightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateLayoutTextRect()));
        connect(ui->previewWidget, SIGNAL(deactivated()), this, SLOT(deactivateLayout()));
        connect(ui->previewWidget, SIGNAL(assessmentTextActivated()), this, SLOT(activateAssessmentTextLayout()));
        connect(ui->previewWidget, SIGNAL(gradeActivated(QString,QString)), this, SLOT(activateLayoutGrade(QString,QString)));
        connect(mLayout, SIGNAL(changed()), ui->previewWidget, SLOT(update()));
        connect(ui->layoutFontButton, SIGNAL(clicked()), this, SLOT(getFont()));


        ui->infoBrowser->setSource(QUrl("./help.htm"));
        ui->infoVersionLabel->setText(Presets::instance().programName() + " " + Presets::instance().programVersion());


        // Gruppierung und Verbindung der Checks herstellen
        foreach (QString grade, QString("Knowledge Skills Safety Reliability Activity ProperHandling Teamwork Responsibility").split(' ')) {
                QRegExp rx(QString("assessment%1*CheckBox").arg(grade));
                rx.setPatternSyntax(QRegExp::Wildcard);


                QButtonGroup * group = new QButtonGroup(this);
                foreach (QCheckBox *checkBox, ui->gradeSelectionGroupBox->findChildren<QCheckBox *>(rx)) {
                        group->addButton(checkBox);
                }
        }

        QFile help(":/doc/help.htm");
        if(help.open(QFile::ReadOnly)) {
                ui->infoBrowser->setText(help.readAll());
                help.close();
        }

        show();

}




void MainWindow::closeEvent(QCloseEvent *event)
{
        saveSettings();
        event->accept();
}




void MainWindow::updateLayout()
{
        foreach (QString yName, Presets::instance().gradeSelectionYNames()) {
                foreach (QString xName, Presets::instance().gradeSelectionXNames()) {
                        QCheckBox *checkBox = ui->gradeSelectionGroupBox->findChild<QCheckBox *>(QString("assessment%1%2CheckBox").arg(yName).arg(xName));
                        if (checkBox) {
                                if (checkBox->isChecked()) {
                                        mLayout->setGradeSelection(yName, xName);
                                }
                        }
                }
        }

        mLayout->setAssessmentText(ui->editTextEdit->toPlainText());
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





void MainWindow::updateLayoutTextRect()
{
        QRectF rect;
        rect.setLeft(ui->layoutPosXSpinBox->value());
        rect.setTop(ui->layoutPosYSpinBox->value());
        rect.setWidth(ui->layoutWidthSpinBox->value());
        rect.setHeight(ui->layoutHeightSpinBox->value());

        if (mAssessmentTextLayoutActive) {
                mLayout->setAssessmentTextRect(rect);
        }
}





void MainWindow::updateLayoutXPos(int pos)
{
        if (mAssessmentTextLayoutActive) {
                updateLayoutTextRect();
        } else if (!mActiveLayoutX.isEmpty()) {
                mLayout->setGradeSelectionXPos(mActiveLayoutX, pos);
        }
}





void MainWindow::updateLayoutYPos(int pos)
{
        if (mAssessmentTextLayoutActive) {
                updateLayoutTextRect();
        } else if (!mActiveLayoutY.isEmpty()) {
                mLayout->setGradeSelectionYPos(mActiveLayoutY, pos);
        }
}




void MainWindow::getFont()
{
        mLayout->setFont(QFontDialog::getFont(0, mLayout->font(), this));
        ui->previewWidget->update();
}




void MainWindow::deactivateLayout()
{
        mAssessmentTextLayoutActive = false;
        mActiveLayoutX.clear();
        mActiveLayoutY.clear();

        ui->layoutPosXSpinBox->setEnabled(false);
        ui->layoutPosYSpinBox->setEnabled(false);
        ui->layoutWidthSpinBox->setEnabled(false);
        ui->layoutHeightSpinBox->setEnabled(false);
}




void MainWindow::activateAssessmentTextLayout()
{
        mAssessmentTextLayoutActive = false;
        mActiveLayoutX.clear();
        mActiveLayoutY.clear();

        QRectF rect = mLayout->assessmentTextRect().translated(- ui->layoutXOffsetSpinBox->value(), - ui->layoutYOffsetSpinBox->value());
        ui->layoutPosXSpinBox->setValue(rect.left());
        ui->layoutPosYSpinBox->setValue(rect.top());
        ui->layoutWidthSpinBox->setValue(rect.width());
        ui->layoutHeightSpinBox->setValue(rect.height());
        ui->layoutPosXSpinBox->setEnabled(true);
        ui->layoutPosYSpinBox->setEnabled(true);
        ui->layoutWidthSpinBox->setEnabled(true);
        ui->layoutHeightSpinBox->setEnabled(true);

        mAssessmentTextLayoutActive = true;
}




void MainWindow::activateLayoutGrade(QString y, QString x)
{
        mAssessmentTextLayoutActive = false;
        mActiveLayoutX.clear();
        mActiveLayoutY.clear();

        ui->layoutPosXSpinBox->setValue(mLayout->gradeSelectionXPos(x) - ui->layoutXOffsetSpinBox->value());
        ui->layoutPosYSpinBox->setValue(mLayout->gradeSelectionYPos(y) - ui->layoutYOffsetSpinBox->value());
        ui->layoutPosXSpinBox->setEnabled(true);
        ui->layoutPosYSpinBox->setEnabled(true);
        ui->layoutWidthSpinBox->setEnabled(false);
        ui->layoutHeightSpinBox->setEnabled(false);

        mActiveLayoutX = x;
        mActiveLayoutY = y;
}




void MainWindow::print()
{
        updateLayout();

        QPrintDialog printDialog (&mPrinter, this);
        printDialog.setOption(QAbstractPrintDialog::PrintShowPageSize, false);

        if (printDialog.exec()) {

                mPrinter.setFullPage(true);
                mPrinter.setResolution(300);

                QPainter p;
                if (p.begin(&mPrinter)) {
                        Painter ph;
                        ph.setFactor(qreal(p.device()->logicalDpiX()) / 25.4);

                        foreach (QPoint point, mLayout->gradeSelectionPoints()) {
                                ph.drawCheck(p, point, 5);
                        }

                        p.setFont(mLayout->font());
                        ph.drawBlockText(p, mLayout->assessmentText(), mLayout->assessmentTextRect());

                        p.end();
                }
        }


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
        save.setValue("file/type", Presets::instance().programName());
        save.setValue("file/version", Presets::instance().fileVersion());

        save.setValue("info/apprentice", ui->saveApprenticeNameEdit->text());
        save.setValue("info/instructor", ui->saveInstructorNameEdit->text());
        save.setValue("info/year", ui->saveYearEdit->value());
        save.setValue("info/begin", ui->saveBeginDateEdit->date().toString());
        save.setValue("info/end", ui->saveEndDateEdit->date().toString());
        save.setValue("info/comment", ui->saveCommentEdit->toPlainText());



        save.beginGroup("grades");
        foreach (QString yName, Presets::instance().gradeSelectionYNames()) {
                foreach (QString xName, Presets::instance().gradeSelectionXNames()) {
                        QCheckBox *checkBox = ui->gradeSelectionGroupBox->findChild<QCheckBox *>(QString("assessment%1%2CheckBox").arg(yName).arg(xName));
                        if (checkBox) {
                                if (checkBox->isChecked()) {
                                        save.setValue(yName, xName);
                                }
                        }
                }
        }
        save.endGroup();

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

        QMessageBox message(this);
        message.setWindowTitle(Presets::instance().programName());
        switch (FileConverter::isConvertible(load.value("file/version", 2).toInt())) {
        case FileConverter::ToOld:
                message.setIcon(QMessageBox::Warning);
                message.setText(QString::fromUtf8("Diese Datei wurde mit einer zu alten Version gespeichert und kann nicht Konvertiert werden!"));
                message.setStandardButtons(QMessageBox::Ok);
                message.exec();
                return;
                break;

        case FileConverter::Convertible:
                message.setIcon(QMessageBox::Information);
                message.setText(QString::fromUtf8("Diese Datei wurde von einer älteren Version erstllt. Soll sie jetzt konvertiert werden?"));
                message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

                if (message.exec() == QMessageBox::Yes) {
                        if (!FileConverter(filename).convert()) {
                                message.setIcon(QMessageBox::Critical);
                                message.setText(QString::fromUtf8("Konvertierung ist Fehlgeschlagen!"));
                                message.setStandardButtons(QMessageBox::Ok);
                                message.exec();
                                return;
                        }
                } else {
                        return;
                }
                break;

        case FileConverter::Newer:
                message.setIcon(QMessageBox::Warning);
                message.setText(QString::fromUtf8("Diese Datei wurde mit einer neueren Version erstellt. Es ist sehr wahrseinlich, dass die geladenen Daten nicht korrekt sind!"));
                message.setStandardButtons(QMessageBox::Ok);
                message.exec();
                break;

        case FileConverter::ExactMatch:
                break;
        }

        ui->saveApprenticeNameEdit->setText(load.value("info/apprentice").toString());
        ui->saveInstructorNameEdit->setText(load.value("info/instructor").toString());
        ui->saveYearEdit->setValue(load.value("info/year").toInt());
        ui->saveBeginDateEdit->setDate(QDate::fromString(load.value("info/begin").toString()));
        ui->saveEndDateEdit->setDate(QDate::fromString(load.value("info/end").toString()));
        ui->saveCommentEdit->setPlainText(load.value("info/comment").toString());

        load.beginGroup("grades");
        QList<QCheckBox *> checks = ui->gradeSelectionGroupBox->findChildren<QCheckBox *>();
        foreach (QString grade, load.childKeys()) {
                foreach (QCheckBox *check, checks) {
                        if (check->objectName().toLower() == QString("assessment%1%2CheckBox").arg(grade).arg(load.value(grade).toString()).toLower()) {
                                check->setChecked(true);
                        }
                }
        }
        load.endGroup();

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
}





void MainWindow::loadSettings()
{
        QSize guiSize;
        guiSize.setWidth(config.value("gui/width", this->sizeHint().width()).toInt());
        guiSize.setHeight(config.value("gui/height", this->sizeHint().height()).toInt());
        this->resize(guiSize);

        saveDir = config.value("path/save", QDir::homePath()).toString() + "/";
}





void MainWindow::saveLayout()
{
        mLayout->save(config);
}




void MainWindow::loadLayout()
{
        deactivateLayout();
        mLayout->load(config);
        ui->previewWidget->unmark();
}




void MainWindow::setDefaultLayout()
{
        deactivateLayout();
        mLayout->setDefaults();
        ui->previewWidget->unmark();
}





