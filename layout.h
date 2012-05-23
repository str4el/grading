/*
 *
 *  Copyright (C) 2012 Stephan Reinhard <Stephan-Reinhard@gmx.de>
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


#ifndef LAYOUT_H
#define LAYOUT_H

#include <QObject>
#include <QHash>
#include <QMap>
#include <QRectF>
#include <QVariant>
#include <QFont>

class QString;
class QSettings;

class Layout : public QObject
{
        Q_OBJECT

private:
        int mXOffset;
        int mYOffset;
        QHash<QString, int> mGradeSelectionXPos;
        QHash<QString, int> mGradeSelectionYPos;
        QMap<QString, QString> mGradeSelection;

        QFont mFont;
        QRectF mAssessmentTextRect;
        QString mAssessmentText;
        QRectF mMeasuresTextRect;
        QString mMeasuresText;



public:
        explicit Layout(QObject *parent = 0);
        void save (QSettings & settings);
        void load (QSettings & settings);

        inline int xOffset (void) const { return mXOffset; }
        inline int yOffset (void) const { return mYOffset; }
        int gradeSelectionXPos(const QString & name) const;
        int gradeSelectionYPos(const QString & name) const;
        QPoint gradeSelectionPos(const QString & xName, const QString & yName) const;

        void setGradeSelection (const QString & grade, const QString & select) { mGradeSelection[grade] = select; emit changed(); }
        QList <QPoint> gradeSelectionPoints (void) const;

        inline QFont font (void) const { return mFont; }
        inline QString assessmentText (void) const { return mAssessmentText; }
        inline QRectF assessmentTextRect(void) const { return mAssessmentTextRect.translated(mXOffset, mYOffset); }
        inline QString measuresText (void) const { return mMeasuresText; }
        inline QRectF measuresTextRect(void) const { return mMeasuresTextRect.translated(mXOffset, mYOffset); }

public slots:
        void setXOffset (const int offset) { mXOffset = offset; emit changed(); }
        void setYOffest (const int offset) { mYOffset = offset; emit changed(); }
        void setGradeSelectionXPos(const QString & name, const int pos);
        void setGradeSelectionYPos(const QString & name, const int pos);

        void setFont (const QFont & font) { mFont = font; emit changed(); }
        void setAssessmentText (const QString & text) { mAssessmentText = text; emit changed(); }
        void setAssessmentTextRect(const QRectF & rect) { mAssessmentTextRect = rect; emit changed(); }
        void setMeasuresText (const QString & text) { mMeasuresText = text; emit changed(); }
        void setMeasuresTextRect(const QRectF & rect) { mMeasuresTextRect = rect; emit changed(); }

        void setDefaults (void);

signals:
        void changed (void);


};

#endif // LAYOUT_H
