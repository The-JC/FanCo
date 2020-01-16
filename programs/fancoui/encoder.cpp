#include "encoder.h"

#include <QDebug>
#include <pigpiod_if2.h>
#include "global.h"
#include "config.h"

#define CLK 21 // GPIO A
#define DATA 20 // GPIO B
#define SWITCH 16
#define PWR 12

static int transits[16] = {
	/* 0000 0001 0010 0011 0100 0101 0110 0111 */
		  0,  -1,   1,   0,   1,   0,   0,  -1,
	/* 1000 1001 1010 1011 1100 1101 1110 1111 */
		 -1,   0,   0,   1,   0,   1,  -1,   0
};

Encoder::Encoder(QObject *parent) : QObject(parent) {
    qDebug() << "Loading encoder";

	lastTurn = new QElapsedTimer;
	holdTime = new QElapsedTimer;

	releaseCheckTimer = new QTimer(this);
	releaseCheckTimer->setSingleShot(true);
	connect(releaseCheckTimer, &QTimer::timeout, this, &Encoder::releaseCheckTimerOverflow);
	connect(this, &Encoder::realeaseCheckTimerStart, releaseCheckTimer, QOverload<int>::of(&QTimer::start));
	connect(this, &Encoder::realeaseCheckTimerStop, releaseCheckTimer, &QTimer::stop);

	levA=0; levB=0;
	step = 0;
	encoderTurned = false;

	// Activate PWR pin to power encoder
	set_mode(pi, PWR, PI_OUTPUT);
	gpio_write(pi, PWR, 1);

	// Configure pins as input
	set_mode(pi, CLK, PI_INPUT);
	set_mode(pi, DATA, PI_INPUT);
	set_mode(pi, SWITCH, PI_INPUT);

	// Configure pull ups as common is ground
	set_pull_up_down(pi, CLK, PI_PUD_UP);
	set_pull_up_down(pi, DATA, PI_PUD_UP);

	this->glitch = 1000;

	set_glitch_filter(pi, CLK, this->glitch);
	set_glitch_filter(pi, DATA, this->glitch);

	this->lastState = (gpio_read(pi, CLK) << 1) | gpio_read(pi, DATA);

	// Monitor edges
	this->cb_id_a = callback_ex(pi, CLK, EITHER_EDGE, _pulseEx, this);
	this->cb_id_b = callback_ex(pi, DATA, EITHER_EDGE, _pulseEx, this);
	this->cb_id_sw = callback_ex(pi, SWITCH, EITHER_EDGE, _pulseEx, this);

	qDebug() << "Loading encoder finished!";
}

void Encoder::_pulseEx(int pi, unsigned gpio, unsigned level, uint32_t tick, void *user) {
	Encoder *encoder = (Encoder *) user;
	if(gpio==SWITCH)
		encoder->_switch(gpio, level, tick);
	else
		encoder->_pulse(gpio, level, tick);
}

void Encoder::releaseCheckTimerOverflow() {
	qDebug() << "HOLD";
	emit press(HOLD);
}

void Encoder::_switch(unsigned int gpio, unsigned int level, uint32_t tick) {
	if(level == 1) {
		if(holdTime->nsecsElapsed() < SHORT_PRESS_TIME * 1000000) {
			qDebug() << "Short press";
			emit press(SHORT_PRESS);
			emit realeaseCheckTimerStop();
		} else if(holdTime->nsecsElapsed() > LONG_PRESS_TIME * 1000000 && !encoderTurned) {
			qDebug() << "Long press";
			emit press(LONG_PRESS);
		} else {
			emit press(RELEASE);
			qDebug() << "Rising Edge with " << holdTime->nsecsElapsed();
		}
	}
	if(level == 0) {
		qDebug() << "Timer started";
		holdTime->start();
		emit realeaseCheckTimerStart(SHORT_PRESS_TIME);
		encoderTurned = false;
	}
}

void Encoder::_pulse(unsigned gpio, unsigned level, uint32_t tick) {
	int newState, inc, detent;

	if(level != PI_TIMEOUT) {
		if(gpio == CLK) levA = level;
		if(gpio == DATA) levB = level;

		newState = levA << 1 | levB;
		inc = transits[lastState << 2 | newState];
		if(inc) {
			lastState = newState;
			detent = step / 4;
			step += inc;
			if(detent != (step / 4)) {
				encoderTurned = true;
				emit turn(velocityControl(step / 4));
				step = 0;
			}
		}
	}
}

int Encoder::velocityControl(int dir) {
	if(velocityControlEnable) {
		if(lastTurn->isValid()) {
			if(dir == lastTurn_dir) {
				if(lastTurn->nsecsElapsed() < 100 * 1000000) {
					dir *= 10;
				}
			}
		}
		lastTurn_dir = dir;
		qDebug() << "Vel Control" << dir;
	}
	lastTurn->start();
	return dir;
}

void Encoder::setGlitchFilter(int glitch) {
	if(glitch < 0) {
		throw "Glitch must be higher than zero!";
		return;
	}
	if(this->glitch == glitch) return;

	this->glitch = glitch;
	set_glitch_filter(pi, CLK, this->glitch);
	set_glitch_filter(pi, DATA, this->glitch);
}

void Encoder::setVelocityControl(bool set) {
	velocityControlEnable = set;
}

Encoder::~Encoder() {
	if(this->cb_id_a >= 0) {
		callback_cancel(this->cb_id_a);
		this->cb_id_a = -1;
	}
	if(this->cb_id_b >= 0) {
		callback_cancel(this->cb_id_b);
		this->cb_id_b = -1;
	}

	set_glitch_filter(pi, CLK, 0);
	set_glitch_filter(pi, DATA, 0);
}
