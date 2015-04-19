#include "grafika.h"

Grafika::Grafika(QWidget *parent)
	: QGraphicsView(parent)
{
}

void Grafika::mousePressEvent(QMouseEvent *event)
{
	QGraphicsView::mousePressEvent(event);
}
