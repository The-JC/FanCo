#ifndef ENCODER_H
#define ENCODER_H

#include <QObject>
#include <QElapsedTimer>

enum ACTION_TYPE {
	SHORT_PRESS = 0,
	LONG_PRESS
};

class Encoder : public QObject
{
	Q_OBJECT
public:
	Encoder(QObject *parent = nullptr);
	void setGlitchFilter(int glitch);
	void setVelocityControl(bool set);
	~Encoder();
private:
	// Callback ids of interupts
	int cb_id_a, cb_id_b, cb_id_sw;
	int step;
	int levA, levB, lastState, glitch;

	bool velocityControlEnable;
	QElapsedTimer *lastTurn;
	int lastTurn_dir;
	int velocityControl(int value);

	void _switch(unsigned gpio, unsigned level, uint32_t tick);
	void _pulse(unsigned gpio, unsigned level, uint32_t tick);
	/* Need a static callback to link with C. */
	static void _pulseEx(int pi, unsigned gpio, unsigned level, uint32_t tick, void *user);

signals:
	/** Reports an encoder turn
	 *  +1 is equal to a right turn
	 *  -1 is equal to a left turn
	 * @param turns
	 */
	void turn(int turns);
	/** Reports a button press
	 *
	 * @param type
	 */
	void press(int type);
};

#endif // ENCODER_H
