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


QString Presets::mVersion = "0.3.1";

/* Default positions
 */
int Presets::mTopOffset = 15;
int Presets::mLeftOffset = 15;
int Presets::mTickOffset[5] = { 126, 139, 152, 165, 178 };
int Presets::mTopToTick = 65;
int Presets::mTickToTick[2] = { 14, 6 };
int Presets::mTickToText = 10;


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

