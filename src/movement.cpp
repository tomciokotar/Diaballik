#include "movement.h"

Movement::Movement()
{
}

Movement::Movement(QPoint start, Piece *pieceGiven, QPoint end)
{
	coords[0] = start;
	coords[1] = end;
	piece = pieceGiven;
	ball = NULL;
}

Movement::Movement(QPoint start, Ball *ballGiven, QPoint end)
{
	coords[0] = start;
	coords[1] = end;
	piece = NULL;
	ball = ballGiven;
}

QPoint Movement::startPoint()
{
	return coords[0];
}

QPoint Movement::endPoint()
{
	return coords[1];
}

Piece * Movement::getPiece()
{
	return piece;
}

Ball * Movement::getBall()
{
	return ball;
}
