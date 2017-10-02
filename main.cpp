#include "QFrontend.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QFrontend w;
	w.show();

	return a.exec();
}
