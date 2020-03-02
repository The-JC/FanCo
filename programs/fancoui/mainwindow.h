#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "config.h"

#include "fan.h"
#include "encoder.h"
#include "info.h"
#include "popup.h"

#define FANS 18

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	QWidget *mainwindow;
	QStackedWidget *stack;
	Encoder *encoder;
	Fan *fans[FANS];
	int position;
	Popup *popup;

	void showGlobalChange();
public slots:
	void mainloop(void);
	void encoderTurned(int turns);
	void buttonPress(int type);
signals:
	void increase(int inc);
};
#endif // MAINWINDOW_H
