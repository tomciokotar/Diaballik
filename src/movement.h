#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <QPoint>

class Piece;
class Ball;

#include "piece.h"
#include "ball.h"

class Movement
{
	QPoint coords[2];
	Piece *piece;
	Ball *ball;

public:
	Movement();
	Movement(QPoint, Piece*, QPoint);
	Movement(QPoint, Ball*, QPoint);

	QPoint startPoint();
	QPoint endPoint();
	Piece * getPiece();
	Ball * getBall();
};

#endif // MOVEMENT_H
