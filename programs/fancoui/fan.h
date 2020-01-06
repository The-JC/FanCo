#ifndef FAN_H
#define FAN_H

#include <valarray>

#include <qwidget.h>
#include "info.h"
#include "config.h"

class Fan : public QWidget {
    Q_OBJECT
private:
	int fanid;
	uint8_t i2c_addres;
	uint16_t lastSpeed;

	Info *info;
	std::valarray<int> plotData;
	QLabel *set_speed_label;
	QLabel *speed_label;

public:
	Fan(QWidget *parent = 0, const int id = -1, const uint8_t address = 0xFF);
    //~Fan();

	int set_speed;

	Info* showInfo();
	int getFanId();
	void acquireData();
	uint16_t getLastSpeed();

protected:
	void paintEvent(QPaintEvent *event) override;
	void focusInEvent(QFocusEvent* e) override;
	void focusOutEvent(QFocusEvent* e) override;

public slots:
	void increaseSpeed(int inc);
	void setSpeed(int value);
signals:
	void setSpeedChanged(int newValue);
	void speedChanged(int newValue);
	void chartDataChanged(std::valarray<int> &newPlotData);
};

#endif // FAN_H
