#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <pigpiod_if2.h>
#include <cstdlib>
#include <QGridLayout>
#include <QTimer>

#include <fcntl.h>

#include "global.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	qInfo() << "Starting FanCo";

	/*
	 * Initalize the GPIOs and I2C
	 */
	std::string gpioHost = GPIOHOST;
	std::string gpioPort = GPIOPORT;

	if(std::getenv("GPIOHOST")==NULL) {
		qInfo() << "GPIO host not found defaulting to " << GPIOHOST;
	} else {
		gpioHost = std::getenv("GPIOHOST");
		qInfo() << "GPIO Host set to " << QString::fromStdString(gpioHost);
	}
	if(std::getenv("GPIOPORT")==NULL) {
		qInfo() << "GPIO port not found defaulting to port " << GPIOPORT;
	} else {
		gpioPort = std::getenv("GPIOPORT");
		qInfo() << "GPIO Port set to " << QString::fromStdString(gpioPort);
	}

	std::vector<char> _optHost(gpioHost.c_str(), gpioHost.c_str() + gpioHost.size() + 1);
	std::vector<char> _optPort(gpioPort.c_str(), gpioPort.c_str() + gpioPort.size() + 1);

	if((pi = pigpio_start(gpioHost == "NULL" ? nullptr : _optHost.data(), _optPort.data())) < 0) {
		qFatal("Could not initalise GPIOs\npigpiod service might not be running!\nsudo pigpiod to start it");
		throw "IO error: GPIO could not be initialized.";
	}

	if((client = open("/dev/i2c-1", O_RDWR)) < 0) {
		qFatal("Failed to open the i2c bus");
		throw "Failed to open the i2c bus!";
	}

	encoder = new Encoder();

	/*
	 *	Setup UI
	 */
	ui->setupUi(this);
	setWindowTitle("FanCo");

	stack = new QStackedWidget(this);
	mainwindow = new QWidget();
	stack->addWidget(mainwindow);
	setCentralWidget(stack);

	QGridLayout *grid = new QGridLayout(this);
	mainwindow->setLayout(grid);

	// Initalize all fans
	for(int i=0; i<FANS; ++i) {
		fans[i] = new Fan(this, i, addresses[i]);
		grid->addWidget(fans[i], i/6, i%6);
		stack->addWidget(fans[i]->showInfo());
		qDebug() << "Init fan " << i << " with slave address " << hex << addresses[i];
	}

	fans[0]->setFocus(Qt::OtherFocusReason);

	this->position = 0;

	// Connect encoder events to mainwindow
	connect(encoder, &Encoder::turn, this, &MainWindow::encoderTurned);
	connect(encoder, &Encoder::press, this, &MainWindow::buttonPress);

	stack->setCurrentIndex(0);

	popup = new Popup(this);
	popup->raise();
	stack->addWidget(popup);


	QTimer *mainloop = new QTimer(this);
	connect(mainloop, &QTimer::timeout, this, &MainWindow::mainloop);
	mainloop->start(1000);

	QFile qss(":/Resources/styles/default.qss");
	qss.open(QFile::ReadOnly);
	mainwindow->setStyleSheet(qss.readAll());
}

void MainWindow::mainloop() {
	for(int i=0; i<FANS; ++i) {
		fans[i]->acquireData();
	}
}

void MainWindow::encoderTurned(int value) {
	if(stack->currentIndex() == 0) { // Main Menu
		position += value;
		if(position<0) position=FANS-1;
		if(position>=FANS) position=0;

		fans[position]->setFocus(Qt::OtherFocusReason);
	} else {
		fans[position]->increaseSpeed(value);
	}
}

void MainWindow::buttonPress(int type) {
	switch (type) {
		case SHORT_PRESS:
			if(stack->currentIndex() != 0) {
				encoder->setVelocityControl(false);
				stack->setCurrentIndex(0);
			} else {
				encoder->setVelocityControl(true);
				stack->setCurrentIndex(position+1);
			}
			break;
		case LONG_PRESS:
			popup->hide();
			break;
		case HOLD:
			popup->activateWindow();
			popup->show();
			popup->raise();
			break;
		case RELEASE:
			popup->hide();
			break;
		default:
			qWarning() << "[Warning] Invalid button press type!";
			break;
>>>>>>> Stashed changes
	}
}

MainWindow::~MainWindow() {
    delete ui;
}
