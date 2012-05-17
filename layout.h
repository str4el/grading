#ifndef LAYOUT_H
#define LAYOUT_H

#include <QObject>
#include <QHash>
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


public:
        explicit Layout(QObject *parent = 0);
        void save (QSettings & settings);
        void load (QSettings & settings);

        int gradeSelectionXPos(const QString & name, const int pos);
        int gradeSelectionYPos(const QString & name, const int pos);
        QRect assessmentTextRect(void) const { return mAssessmentTextRect; }

public slots:
        void setGradeSelectionXPos(const QString & name, const int pos);
        void setGradeSelectionYPos(const QString & name, const int pos);
        void setAssessmentTextRect(const QRect & rect) { mAssessmentTextRect = rect; emit changed(); }

signals:
        void changed (void);


};

#endif // LAYOUT_H
