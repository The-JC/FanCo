#ifndef INFO_H
#define INFO_H

#include <valarray>

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

using namespace QtCharts;

class Info : public QWidget
{
	Q_OBJECT
public:
	Info(QWidget *parent = nullptr);
private:
	QWidget *parent;
	QLabel *set_speed_label;
	QLabel *speed_label;
	QLineSeries *speed;
	QChart *chart;
	QChartView *chartViewer;
	QTimer *chartUpdateTimer;
public slots:
	void setSetSpeed(int value);
	void setSpeed(int value);
	void updateChart(std::valarray<int> &data);
signals:
};

#endif // INFO_H
