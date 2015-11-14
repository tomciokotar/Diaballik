#ifndef SQUARE_H
#define SQUARE_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPoint>

class Board;

#include "board.h"

class Square : public QGraphicsRectItem
{
	Board *board;
	QColor color;
	QPoint coords;
	Piece *piece;

	bool highlighted;

public:
	Square(int, int, Board *, QGraphicsScene *);
	~Square();

	void mousePressEvent(QGraphicsSceneMouseEvent *);
	void highlight(bool);
	void highlight();
	void turnOffHighlight();
	bool isHighlighted();
	QPoint getCoords();
	void setCoords(QPoint);
	Piece * getPiece();
	void putPiece(Piece *);
};

#endif // SQUARE_H
