#include <QtGui/QApplication>
#include "mainWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow window(&app);

	window.setWindowTitle("Diaballik");

	window.show();

	return app.exec();
}
