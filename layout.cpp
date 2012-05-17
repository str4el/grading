#include "layout.h"

#include "presets.h"

#include <QSettings>

Layout::Layout(QObject *parent) :
        QObject(parent)
{
        mAssessmentTextRect = Presets2::instance().assessmentTextRect();
}




void Layout::load(QSettings &settings)
{
        settings.beginGroup("layout");
        mGradeSelectionXPos = settings.value("grade_selection_x_position").toHash();
        mGradeSelectionYPos = settings.value("grade_selection_y_position").toHash();
        mAssessmentTextRect = settings.value("assessment_text_rect").toRect();
        settings.endGroup();
}





void Layout::save(QSettings &settings)
{
        settings.beginGroup("layout");
        settings.setValue("grade_selection_x_position", mGradeSelectionXPos);
        settings.setValue("grade_selection_y_position", mGradeSelectionYPos);
        settings.setValue("assessment_text_rect", mAssessmentTextRect);
        settings.endGroup();
}





int Layout::gradeSelectionXPos(const QString &name) const
{
        if (mGradeSelectionXPos.contains(name)) {
                return mGradeSelectionXPos[name].toInt();
        }

        return Presets2::instance().gradeSelectionXPos(name);
}




int Layout::gradeSelectionYPos(const QString &name) const
{
        if (mGradeSelectionYPos.contains(name)) {
                return mGradeSelectionYPos[name].toInt();
        }

        return Presets2::instance().gradeSelectionYPos(name);
}




QPoint Layout::gradeSelectionPos(const QString &xName, const QString &yName) const
{
        QPoint ret;
        ret.setX(gradeSelectionXPos(xName));
        ret.setY(gradeSelectionYPos(yName));
        return ret;
}




void Layout::setGradeSelectionXPos(const QString &name, const int pos)
{
        mGradeSelectionXPos[name].setValue(pos);
        emit changed();
}




void Layout::setGradeSelectionYPos(const QString &name, const int pos)
{
        mGradeSelectionYPos[name].setValue(pos);
        emit changed();
}




QList <QPoint> Layout::gradeSelectionPoints() const
{
        QList <QPoint> points;

        foreach (QString grade, mGradeSelection.keys()) {
                points.append(gradeSelectionPos(mGradeSelection.value(grade), grade));
        }

        return points;
}
