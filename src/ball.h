#ifndef BALL_H
#define BALL_H

#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QPoint>

class Board;

#include "board.h"

class Ball : public QGraphicsEllipseItem
{
	Board *board;
	QGraphicsItemAnimation *animation;
	QPoint coords;
	QColor color;

	int ballColor;
	bool highlighted;

public:
	QTimeLine *timeline;

	Ball(int, int, int, Board *, QGraphicsScene *);
	~Ball();

	void mousePressEvent(QGraphicsSceneMouseEvent *);
	int getPlayer();
	void moveTo(QPoint);
	QPoint getCoords();
	void setCoords(QPoint);
	void changePosition(QPoint, bool);
	void changePosition(QPoint);
	bool isHighlighted();
	void isHighlighted(bool);
	Board * getBoard();
	void shake();

};

#endif // BALL_H
