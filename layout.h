#ifndef LAYOUT_H
#define LAYOUT_H

#include <QObject>
#include <QHash>
#include <QMap>
#include <QRect>
#include <QVariant>

class QString;
class QSettings;

class Layout : public QObject
{
        Q_OBJECT

private:
        QRect mAssessmentTextRect;
        QHash<QString, QVariant> mGradeSelectionXPos;
        QHash<QString, QVariant> mGradeSelectionYPos;

        QMap<QString, QString> mGradeSelection;
        QString mAssessmentText;



public:
        explicit Layout(QObject *parent = 0);
        void save (QSettings & settings);
        void load (QSettings & settings);

        QRect assessmentTextRect(void) const { return mAssessmentTextRect; }
        int gradeSelectionXPos(const QString & name) const;
        int gradeSelectionYPos(const QString & name) const;
        QPoint gradeSelectionPos(const QString & xName, const QString & yName) const;

        void setGradeSelection (const QString & grade, const QString & select) { mGradeSelection[grade] = select; emit changed(); }
        QList <QPoint> gradeSelectionPoints (void) const;

public slots:
        void setAssessmentTextRect(const QRect & rect) { mAssessmentTextRect = rect; emit changed(); }
        void setGradeSelectionXPos(const QString & name, const int pos);
        void setGradeSelectionYPos(const QString & name, const int pos);

        void setAssessmentText (const QString & text) { mAssessmentText = text; emit changed(); }

signals:
        void changed (void);


};

#endif // LAYOUT_H
