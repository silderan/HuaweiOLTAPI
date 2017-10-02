#ifndef QFRONTEND_H
#define QFRONTEND_H

#include <QMainWindow>

namespace Ui {
class QFrontend;
}

class QFrontend : public QMainWindow
{
	Q_OBJECT

public:
	explicit QFrontend(QWidget *parent = 0);
	~QFrontend();

private:
	Ui::QFrontend *ui;
};

#endif // QFRONTEND_H
