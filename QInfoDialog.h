#ifndef QINFODIALOG_H
#define QINFODIALOG_H

#include <QDialog>

namespace Ui {
class QInfoDialog;
}

class QInfoDialog : public QDialog
{
	Q_OBJECT

public:
	explicit QInfoDialog(const QString &title, const QString &txt, QWidget *parent = 0);
	~QInfoDialog();

private:
	Ui::QInfoDialog *ui;
};

#endif // QINFODIALOG_H
