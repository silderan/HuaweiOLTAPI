#include "QInfoDialog.h"
#include "ui_QInfoDialog.h"

QInfoDialog::QInfoDialog(const QString &title, const QString &txt, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::QInfoDialog)
{
	ui->setupUi(this);
	this->setWindowTitle(title);
	ui->plainTextEdit->insertPlainText(txt);
}

QInfoDialog::~QInfoDialog()
{
	delete ui;
}
