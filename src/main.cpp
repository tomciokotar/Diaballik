#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w(&a);

	w.setWindowTitle("Diaballik");

	w.show();

	return a.exec();
}
