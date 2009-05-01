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
#include "Option.h"



/* Constructor versucht die angegebene Datei zu öffenen.
 * Die Datei sollte optionen nach folgendem Format enthalten:
 * option_name="options_inhalt"
 * Die Optionen dürfen nicht über mehrere zeilen hinweg gehen.
 */
Option::Option(const QString filename, QWidget *parrent) : QObject(parrent)
{

	file.setFileName(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox(QMessageBox::Warning, "Fehler", QString("Konnte Datei \"%1\" nicht Öffnen!").arg(filename), QMessageBox::Close, parrent).exec();
		return;
	}
	in.setDevice(&file);

}


Option::~Option()
{
	file.close();
}



/* Durchsucht den stream nach einer Option mit dem angegebenen Namen und gibt dessen Inhalt zurück.
 */
QString Option::getOption(const QString name)
{
	QString line;

	in.seek(0);
	do {
		line = in.readLine();
		int refer = line.indexOf(QChar('='));
		if (line.left(refer).contains(name)) {
			int begin = line.indexOf(QChar('"'), refer);
			int end = line.lastIndexOf(QChar('"'));
			return line.mid(begin + 1, end - begin - 1);
		}

	} while (!line.isNull());

	return QString();

}


