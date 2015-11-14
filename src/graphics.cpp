#include "graphics.h"

Graphics::Graphics(QWidget *parent)
	: QGraphicsView(parent)
{
}

void Graphics::mousePressEvent(QMouseEvent *event)
{
	QGraphicsView::mousePressEvent(event);
}
