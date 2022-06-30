#include "horn.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	horn2 w;
	w.show();
	return a.exec();
}
