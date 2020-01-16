#include "fan.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <QStringLiteral>
#include <QDebug>
#include <QStyleOption>

#include "global.h"

extern "C" {
	#include <fcntl.h>
	#include <linux/i2c-dev.h>
	#include <sys/ioctl.h>
	#include <i2c/smbus.h>
}
#include <unistd.h>

Fan::Fan(QWidget *parent, const int fanid, const uint8_t address)
    : QWidget(parent) {

	if(fanid == -1) {
		throw "FanId of must be given";
		return;
	}
	this->fanid = fanid;
	if(address > 127) {
		throw "I2C address must be a valid value";
	}
	this->i2c_addres = address;
	this->plotData = std::valarray<int>(PLOTLEN+1);
	this->set_speed = 0;
	this->info = new Info(this);
	connect(this, &Fan::setSpeedChanged, this->info, &Info::setSetSpeed);
	connect(this, &Fan::speedChanged, this->info, &Info::setSpeed);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	QLabel *id = new QLabel(QString::number(fanid+1), this);
	id->setObjectName("id");
	id->setAlignment(Qt::AlignCenter);
	id->setMargin(0);
	layout->addWidget(id);

	set_speed_label = new QLabel(QString::number(set_speed) + " RPM", this);
	set_speed_label->setObjectName("setSpeed");
	set_speed_label->setAlignment(Qt::AlignCenter);
	set_speed_label->setMargin(0);
	layout->addWidget(set_speed_label);

	speed_label = new QLabel(QStringLiteral("0") + " RPM", this);
	speed_label->setObjectName("speed");
	speed_label->setAlignment(Qt::AlignCenter);
	speed_label->setMargin(0);
	layout->addWidget(speed_label);

	this->resize(133, 150);

	connect(this, &Fan::chartDataChanged, info, &Info::updateChart);
}

void Fan::acquireData() {
	if(client < 0) return;

	if(this->i2c_addres < 3) return;

	if(ioctl(client, I2C_SLAVE, i2c_addres)<0) {
		qFatal("Failed to acquire bus access and/or talk to slave");
		return;
	}

	unsigned char data[3];
	read(client, data, 3);

	uint16_t rpm = (data[0] << 8) | data[1];
	qDebug() << "Read speed 0x" << hex << data[0] << data[1] << " as " << dec << rpm << "of maxiumum" << data[2] << "%";
	if(rpm == 0xFFFF) {
		this->lastSpeed = rpm;
		speed_label->setNum(lastSpeed);
		return;
	}

	if(this->lastSpeed != rpm) {
		this->lastSpeed = rpm;
		speed_label->setText(QString::number(lastSpeed) + " RPM");
		emit speedChanged(lastSpeed);
	}

	plotData = plotData.shift(1);
	plotData[PLOTLEN] = rpm;
	emit chartDataChanged(plotData);
}

void Fan::sendData() {
	if(client < 0) return;

	if(this->i2c_addres < 3) return;

	if(ioctl(client, I2C_SLAVE, i2c_addres) < 0) {
		qFatal("Failed to acquire bus access and/or talk to slave");
		return;
	}

	uint8_t buffer[2];

	buffer[0] = static_cast<uint8_t>(set_speed);
	buffer[1] = static_cast<uint8_t>(set_speed >> 8);

	if(i2c_smbus_write_i2c_block_data(client, SEND_RPM, 2, buffer)) {
		qCritical() << "Failed writing to I2C device!";
	} else {
		qDebug() << "Successful send data";
	}
}

void Fan::increaseSpeed(int inc) {
	this->setSpeed(this->set_speed + inc);
	if(this->set_speed < 0) {
		this->setSpeed(0);
	} else if(this->set_speed > MAXSPEED) {
		this->setSpeed(MAXSPEED);
	}
}

void Fan::setSpeed(int value) {
	this->set_speed = value;
	emit setSpeedChanged(this->set_speed);

	set_speed_label->setText(QString::number(this->set_speed) + " RPM");
	sendData();
}

int Fan::getFanId() {
	return fanid;
}

Info * Fan::showInfo() {
	return info;
}

void Fan::paintEvent(QPaintEvent *event) {
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Fan::focusInEvent(QFocusEvent *event) {
	this->setStyleSheet("Fan { background-color: #1c565c; color: #fff; }");
}

void Fan::focusOutEvent(QFocusEvent *event) {
	this->setStyleSheet("Fan { background-color: #257179; }");
}
