#include "QFrontend.h"
#include "ui_QFrontend.h"

QFrontend::QFrontend(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::QFrontend)
{
	ui->setupUi(this);
}

QFrontend::~QFrontend()
{
	delete ui;
}
