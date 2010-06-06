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

#include <QString>

#include "GradingBuild.h"


GradingBuild::GradingBuild()
{
        leftPos = 10;
        topPos = 10;
        topToTickPos = 10;
        tickToTickPos1 = 10;
        tickToTickPos2 = 10;
        tickToTextPos = 10;


        for (int i = 0; i < 8; i++) {
                this->tickPos[i] = 0;
        }
}


bool GradingBuild::build()
{

        QString source;

        source += QString::fromUtf8("\\batchmode\n\\documentclass[11pt]{article}\n\\usepackage{ngerman}\n");
        source += QString::fromUtf8("\\usepackage[utf8]{inputenc}\n\\usepackage{vmargin}\n\\setpapersize{A4}\n");

        source += QString::fromUtf8("\\setmargins{");
        source += QString::number(leftPos) + QString::fromUtf8("mm}{");
        source += QString::number(topPos) + QString::fromUtf8("mm}");
        source += QString::fromUtf8("{184mm}{250mm}{12pt}{25pt}{0pt}{30pt}\n");

        source +=QString::fromUtf8("\\parindent0pt\n\\pagestyle{empty}\n\\begin{document}\n");

        source += QString::fromUtf8("\\vspace*{") + QString::number(topToTickPos) + QString::fromUtf8("mm}\n");

        source += QString::fromUtf8("\\hspace*{") + QString::number(tickPos[0]) + QString::fromUtf8("mm}$\\surd$\\\\\n");
        source += QString::fromUtf8("\\hspace*{") + QString::number(tickPos[1]) + QString::fromUtf8("mm}$\\surd$\\\\\n");
        source += QString::fromUtf8("\\hspace*{") + QString::number(tickPos[2]) + QString::fromUtf8("mm}$\\surd$\\\\[") + QString::number(tickToTickPos1) + QString::fromUtf8("mm]\n");
        source += QString::fromUtf8("\\hspace*{") + QString::number(tickPos[3]) + QString::fromUtf8("mm}$\\surd$\\\\\n");
        source += QString::fromUtf8("\\hspace*{") + QString::number(tickPos[4]) + QString::fromUtf8("mm}$\\surd$\\\\\n");
        source += QString::fromUtf8("\\hspace*{") + QString::number(tickPos[5]) + QString::fromUtf8("mm}$\\surd$\\\\[") + QString::number(tickToTickPos2) + QString::fromUtf8("mm]\n");
        source += QString::fromUtf8("\\hspace*{") + QString::number(tickPos[6]) + QString::fromUtf8("mm}$\\surd$\\\\\n");
        source += QString::fromUtf8("\\hspace*{") + QString::number(tickPos[7]) + QString::fromUtf8("mm}$\\surd$\\\\\n\n");

        source += QString::fromUtf8("\\vspace*{") + QString::number(tickToTextPos) + QString::fromUtf8("mm}\\Large\n");

        source += text;
        source += QString::fromUtf8("\n\\end{document}\n");


        QFile out(QDir::tempPath() + "/grading.tex");
        if (!out.open(QIODevice::WriteOnly | QIODevice::Text)) {
                return false;
        }

        QTextStream out_stream(&out);
        out_stream.setCodec("UTF-8");

        out_stream << source;
        out.close();

        return true;
}




