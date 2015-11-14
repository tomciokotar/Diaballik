#include "piece.h"

Piece::Piece(int x, int y, int colorGiven, int numberGiven, Board *boardGiven, QGraphicsScene *scene)
	: QGraphicsRectItem(5 + x*50, 5 + y*50, 40, 40, 0, scene)
{
	if (colorGiven == 0)
		color = QColor(Qt::white);
	else
		color = QColor(Qt::red);

	setBrush(QBrush(color, Qt::SolidPattern));

	board = boardGiven;
	pieceColor = colorGiven;
	number = numberGiven;
	holdingBall = false;
	purpleHighlighted = false;
	greenHighlighted = false;
	coords = QPoint(x,y);

	timeline = new QTimeLine(300);
	timeline->setCurrentTime(300);
	timeline->setFrameRange(0, 20);

	animation = new QGraphicsItemAnimation();
	animation->setItem(this);
	animation->setTimeLine(timeline);
}

Piece::~Piece()
{
	delete timeline;
	delete animation;
}

void Piece::mousePressEvent(QGraphicsSceneMouseEvent *)
{
	if (isPurpleHighlighted()) {
		if (board->getHighlightedBall() != NULL) {
			if (board->isBeingEdited()) {
				
				board->previousModifications.push(
					Movement(board->getHighlightedBall()->getCoords(),
							 board->getHighlightedBall(), getCoords()) );
				
				board->nextModifications.clear();

				board->getHighlightedBall()->changePosition(getCoords());
			}
			else
				board->getCurrentTurn()->newMovement(board->getHighlightedBall(), getCoords());
		}
	}
	else if (!isGreenHighlighted() && !isHoldingBall()) {
		board->turnOffHighlighting();

		if (board->isBeingEdited()) {
			board->highlightAllSquares();
			board->setHighlightedPiece(this);
		}
		else if (board->getCurrentPlayer() == getPlayer() && board->isHuman(getPlayer())
				&& board->getCurrentTurn()->getDonePieceMovementsCount() < 2) {
			
			if (board->highlightSquares(this))
				board->setHighlightedPiece(this);
			else
				shake();
		}
		else
			shake();
	}
	else if (isGreenHighlighted()) {
		board->turnOffHighlighting();
		board->setHighlightedPiece(NULL);
	}
	else {
		board->turnOffHighlighting();
		shake();
	}

	if (!board->isBeingEdited()) {
		if (board->getCurrentPlayer() == 0)
			board->getStatus()->setText("Current player: white.");
		else
			board->getStatus()->setText("Current player: red.");
	}
}

int Piece::getPlayer()
{
	return pieceColor;
}

void Piece::moveTo(QPoint endPoint)
{
	timeline->setCurrentTime(300);

	QPoint direction = QPoint(endPoint.x() - getCoords().x(),
							  endPoint.y() - getCoords().y());
	
	animation->setItem(this);
	animation->clear();
	animation->setPosAt(1, QPoint(x() + 50 * direction.x() * scale(),
								  y() + 50 * direction.y() * scale()));
	
	timeline->start();
}

QPoint Piece::getCoords()
{
	return coords;
}

void Piece::setCoords(QPoint point)
{
	coords = point;
}

bool Piece::isHoldingBall()
{
	return holdingBall;
}

void Piece::isHoldingBall(bool value)
{
	holdingBall = value;
}

bool Piece::isPurpleHighlighted()
{
	return purpleHighlighted;
}

void Piece::highlightInPurple(bool visibly)
{
	purpleHighlighted = true;

	if (visibly)
		setBrush(QBrush(QColor("#aa00ee"), Qt::SolidPattern));
}

void Piece::highlightInPurple()
{
	highlightInPurple(true);
}

void Piece::turnOffPurpleHighlight()
{
	purpleHighlighted = false;
	setBrush(QBrush(color, Qt::SolidPattern));
}

void Piece::changePosition(QPoint endPoint, bool visibly)
{
	if (visibly)
		moveTo(endPoint);

	board->putPiece(coords, NULL);
	board->putPiece(endPoint, this);
	board->getSquare(coords.x(), coords.y())->putPiece(NULL);
	board->getSquare(endPoint.x(), endPoint.y())->putPiece(this);

	coords = endPoint;
	board->movementFinished();
}

void Piece::changePosition(QPoint endPoint)
{
	changePosition(endPoint, true);
}

bool Piece::isGreenHighlighted()
{
	return greenHighlighted;
}

void Piece::isGreenHighlighted(bool value)
{
	greenHighlighted = value;

	if (value == true)
		setBrush(QBrush(QColor(Qt::green), Qt::SolidPattern));
	else
		setBrush(QBrush(color, Qt::SolidPattern));
}

Board * Piece::getBoard()
{
	return board;
}

void Piece::shake()
{
	timeline->setCurrentTime(300);

	animation->setItem(this);
	animation->clear();
	animation->setPosAt(0.25, QPoint(x() + 5 * scale(), y()));
	animation->setPosAt(0.75, QPoint(x() - 5 * scale(), y()));
	animation->setPosAt(1, QPoint(x(), y()));

	timeline->start();
}

int Piece::getNumber()
{
	return number;
}
