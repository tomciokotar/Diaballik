#ifndef PIECE_H
#define PIECE_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QPoint>

class Board;

#include "board.h"

class Piece : public QGraphicsRectItem
{
	Board *board;
	QGraphicsItemAnimation *animation;
	QPoint coords;
	QColor color;

	int pieceColor, number;
	bool holdingBall;
	bool purpleHighlighted;
	bool greenHighlighted;

public:
	QTimeLine *timeline;

	Piece(int, int, int, int, Board *, QGraphicsScene *);
	~Piece();

	void mousePressEvent(QGraphicsSceneMouseEvent *);
	int getPlayer();
	void moveTo(QPoint);
	QPoint getCoords();
	void setCoords(QPoint);
	bool isHoldingBall();
	void isHoldingBall(bool);
	bool isPurpleHighlighted();
	void highlightInPurple(bool);
	void highlightInPurple();
	void turnOffPurpleHighlight();
	void changePosition(QPoint, bool);
	void changePosition(QPoint);
	bool isGreenHighlighted();
	void isGreenHighlighted(bool);
	Board * getBoard();
	void shake();
	int getNumber();
};

#endif // PIECE_H
