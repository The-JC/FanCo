#ifndef POPUP_H
#define POPUP_H

#include <QWidget>

class Popup : public QWidget
{
	Q_OBJECT
public:
	explicit Popup(QWidget *parent = nullptr);
protected:
	void paintEvent(QPaintEvent *event) override;

signals:

};

#endif // POPUP_H
