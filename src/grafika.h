#ifndef GRAFIKA_H
#define GRAFIKA_H

#include <QGraphicsView>
#include <QMouseEvent>

class Grafika : public QGraphicsView
{
protected:
	void mousePressEvent(QMouseEvent *);

public:
	Grafika(QWidget *parent = 0);
};

#endif // GRAFIKA_H
