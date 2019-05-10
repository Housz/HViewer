#include "MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setWindowIcon(QIcon("./Resources/logo.ico"));
	MainWindow w;
	w.show();
	return a.exec();
}




