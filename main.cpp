#include <QApplication>
#include "ui.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setStyle("cleanlooks");
	UI demo;
	demo.resize(500, 300);
	demo.show();

	return app.exec();
}
