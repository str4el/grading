/*
 *
 *  Copyright (C) 2010 Stephan Reinhard <Stephan-Reinhard@gmx.de>
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

#ifndef BUILD_H
#define BUILD_H

#include <QtCore>

class Build : public QObject
{

        Q_OBJECT

public:
    Build();
    bool build();

public slots:
    void setLeftPos(int pos) { leftPos = pos; }
    void setTopPos(int pos) { topPos = pos; }
    void setTopToTickPos(int pos) { topToTickPos = pos; }
    void setTickToTickPos1(int pos) { tickToTickPos1 = pos; }
    void setTickToTickPos2(int pos) { tickToTickPos2 = pos; }
    void setTickToTextPos(int pos) { tickToTextPos = pos; }

    void setText(QString myText) { text = myText; }
    void setTickPos(int n, int pos)
    {
            if (n > 7 || n < 0) return;
            tickPos[n] = pos;
    }

private:
    int leftPos;
    int topPos;
    int topToTickPos;
    int tickToTickPos1;
    int tickToTickPos2;
    int tickToTextPos;

    QString text;
    int tickPos[8];

};

#endif // BUILD_H
