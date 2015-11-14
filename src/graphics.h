#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QGraphicsView>
#include <QMouseEvent>

class Graphics : public QGraphicsView
{
protected:
	void mousePressEvent(QMouseEvent *);

public:
	Graphics(QWidget *parent = 0);
};

#endif // GRAPHICS_H
