#ifndef TURN_H
#define TURN_H

#include <QObject>
#include <QPoint>
#include <QVector>
#include <QTextStream>

class Piece;
class Ball;
class Board;

#include "piece.h"
#include "ball.h"
#include "board.h"

class Turn : public QObject
{
	Q_OBJECT

	QPoint coordsArray[3][2];

	Piece *pieces[3];
	Ball *ball;

	int donePieceMovementsCount, totalMovementsCount;
	bool ballThrown;

	QVector<int> movesOrder;
	/*
	  0 - first movement
	  1 - second movement
	  2 - ball throw
	*/

public:
	Turn();
	Turn(QString, Board *);
	~Turn();

	int getDoneMovementsCount();
	int getTotalMovementsCount();
	int getDonePieceMovementsCount();
	bool isBallThrown();
	void newMovement(Piece *, QPoint);
	void newMovement(Ball *, QPoint);
	void undoMovement(bool);
	void undoMovement();
	void redoMovement(bool);
	void redoMovement();
	QString saveTurn();
	void setUp();
};

#endif // TURN_H
