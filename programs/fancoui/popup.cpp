#include "popup.h"

#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>

#define WIDTH 500
#define HEIGHT 280

Popup::Popup(QWidget *parent) : QWidget(parent) {
	resize(WIDTH, HEIGHT);
	setGeometry((800-WIDTH)/2, (480-HEIGHT)/2, WIDTH, HEIGHT);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setMargin(0);
	layout->setAlignment(Qt::AlignHCenter);
	setLayout(layout);

	QLabel *l = new QLabel("Change Fan speed");
	l->setAlignment(Qt::AlignTop);
	l->setMargin(10);
	layout->addWidget(l);


}

void Popup::paintEvent(QPaintEvent *event) {
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	p.fillRect(rect(), {244, 244, 244, 220});
}
