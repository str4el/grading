#ifndef PREVIEW_H
#define PREVIEW_H

#include <QWidget>
#include <QPointer>
#include "builder.h"



class QPainter;
class QRect;


class Preview : public QWidget
{
        Q_OBJECT

private:
        QPointer <Builder> mBuilder;
        QRect mWindow;

protected:
        void paintEvent(QPaintEvent *);


public:
        explicit Preview(QWidget *parent = 0);

        void setBuilder (Builder *b) { mBuilder = b; }


};

#endif // PREVIEW_H
