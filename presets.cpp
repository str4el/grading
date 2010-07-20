/*
 *
 *  Copyright (C) 2010 Wolfgang Forstmeier <wolfgang.forstmeier@gmail.com>
 *  Copyright (C) 2010 Stephan Reinhard <stephan-reinhard@gmx.de>
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

#include "presets.h"
#include <QProcessEnvironment>
#include <QSettings>


QString Presets::version = "0.3.1";


/* Default positions
 */
int Presets::topOffset = 15;
int Presets::leftOffset = 15;
int Presets::tickOffset[5] = { 126, 136, 152, 165, 178 };
int Presets::topToTick = 65;
int Presets::tickToTick1 = 14;
int Presets::tickToTick2 = 6;
int Presets::tickToText = 10;


QString Presets::saveDir()
{
        QString path;
#ifdef Q_OS_LINUX
        path = QProcessEnvironment::systemEnvironment().value("HOME");
#endif

#ifdef Q_OS_WIN32
        QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", QSettings::NativeFormat);
        path = settings.value("Personal").toString();
#endif
        if (path.isEmpty()) {
                path = ".";
        }
        path += "/";

        return path;
}


