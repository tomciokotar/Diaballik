#include "square.h"

Square::Square(int x, int y, Board *boardGiven, QGraphicsScene *scene)
	: QGraphicsRectItem(x * 50, y * 50, 50, 50, 0, scene)
{
	if ((x + y)%2 == 0)
		color = QColor("#aaccee");
	else
		color = QColor("#aabbee");

	setBrush(QBrush(color, Qt::SolidPattern));

	board = boardGiven;
	coords = QPoint(x,y);
	highlighted = false;
	piece = NULL;
}

Square::~Square()
{
}

void Square::mousePressEvent(QGraphicsSceneMouseEvent *)
{
	if (isHighlighted())
		if (board->getHighlightedPiece() != NULL) {
			if (board->isBeingEdited()) {
				board->previousModifications.push(
					Movement(board->getHighlightedPiece()->getCoords(),
							 board->getHighlightedPiece(), getCoords()) );
							 
				board->nextModifications.clear();

				board->getHighlightedPiece()->changePosition(getCoords());
			}
			else
				board->getCurrentTurn()->newMovement(board->getHighlightedPiece(), getCoords());

			if (!board->isBeingEdited()) {
				if (board->getCurrentPlayer() == 0)
					board->getStatus()->setText("Current player: white.");
				else
					board->getStatus()->setText("Current player: red.");
			}
		}
}

void Square::highlight(bool visibly)
{
	highlighted = true;

	if (visibly)
		setBrush(QBrush(QColor("#aa00ee"), Qt::SolidPattern));
}

void Square::highlight()
{
	highlight(true);
}

void Square::turnOffHighlight()
{
	highlighted = false;
	setBrush(QBrush(color, Qt::SolidPattern));
}

bool Square::isHighlighted()
{
	return highlighted;
}

QPoint Square::getCoords()
{
	return coords;
}

void Square::setCoords(QPoint point)
{
	coords = point;
}

Piece * Square::getPiece()
{
	return piece;
}

void Square::putPiece(Piece *pieceGiven)
{
	piece = pieceGiven;
}
