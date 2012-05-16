#include "preview.h"

#include <QPainter>


Preview::Preview(QWidget *parent) :
        QWidget(parent),
        mWindow(0,0, 2279, 3308)
{
}




void Preview::paintEvent(QPaintEvent *)
{
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setWindow(mWindow);
        p.setViewport(mWindow);

        if (mBuilder) {
                mBuilder->paint(p);
        }

}
