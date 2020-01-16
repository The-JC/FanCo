#include "info.h"

#include <QGridLayout>
#include <QLabel>
#include <QtCharts/QValueAxis>
#include <QString>
#include <QDebug>

#include "fan.h"

#include <random>

Info::Info(QWidget *parent) : QWidget(parent) {
	this->parent = parent;

	Fan *fan = (Fan *) parent;

	QGridLayout *layout = new QGridLayout(this);
	layout->setMargin(0);

	QLabel *id = new QLabel(QString::number(fan->getFanId()+1));
	id->setMargin(10);
	id->setAlignment(Qt::AlignTop);
	layout->addWidget(id, 0, 0, 1, 2);

	speed = new QLineSeries();

	for(int i=0; i<=PLOTLEN; i++) {
		speed->append(i, 0);
	}

	chart = new QChart();
	chart->addSeries(speed);
	chart->legend()->hide();
	chart->setTitle("RPM vs dt");

	QValueAxis *axisX = new QValueAxis;
	axisX->setLabelFormat("%i");
	axisX->setTitleText("T-");
	axisX->setReverse(true);
	chart->addAxis(axisX, Qt::AlignBottom);
	speed->attachAxis(axisX);

	QValueAxis *axisY = new QValueAxis;
	axisY->setLabelFormat("%i");
	axisY->setTitleText("RPM");
	axisY->setRange(0, 12000);
	chart->addAxis(axisY, Qt::AlignLeft);
	speed->attachAxis(axisY);

	chartViewer = new QChartView(chart);
	chartViewer->setRenderHint(QPainter::Antialiasing);

	layout->addWidget(chartViewer, 1, 0);

	fan->set_speed = 0;

	set_speed_label = new QLabel("Set Speed: " + QString::number(fan->set_speed));
	set_speed_label->setMargin(0);
	layout->addWidget(set_speed_label, 2, 0);

	speed_label = new QLabel("Speed: " + QString::number(0));
	layout->addWidget(speed_label, 2, 1);
}

void Info::setSetSpeed(int newValue) {
	set_speed_label->setText("Set Speed: " + QString::number(newValue));
}

void Info::setSpeed(int newValue) {
	speed_label->setText("Speed: " + QString::number(newValue));
}

void Info::updateChart(std::valarray<int> &data) {
	speed->clear();
	for(unsigned int i=0; i<data.size(); ++i) {
		speed->append(PLOTLEN-i, data[i]);
	}
	chart->update();
	chartViewer->repaint();
}
