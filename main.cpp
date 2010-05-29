#include <QApplication>
#include "GradingWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	GradingWindow *window = new GradingWindow;
	return app.exec();
}

