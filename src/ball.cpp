#include "ball.h"

Ball::Ball(int x, int y, int colorGiven, Board *boardGiven, QGraphicsScene *scene)
	: QGraphicsEllipseItem(10 + x*50, 10 + y*50, 30, 30, 0 ,scene)
{
	if (colorGiven == 0)
		color = QColor(Qt::blue);
	else
		color = QColor(Qt::black);

	setBrush(QBrush(color, Qt::SolidPattern));

	board = boardGiven;
	ballColor = colorGiven;
	highlighted = false;
	coords = QPoint(x,y);

	timeline = new QTimeLine(200);
	timeline->setCurrentTime(200);
	timeline->setFrameRange(0, 20);

	animation = new QGraphicsItemAnimation();
	animation->setItem(this);
	animation->setTimeLine(timeline);
}

Ball::~Ball()
{
	delete timeline;
	delete animation;
}

void Ball::mousePressEvent(QGraphicsSceneMouseEvent *)
{
	if (!isHighlighted()) {
		board->turnOffHighlighting();

		if (board->isBeingEdited()) {
			if (getPlayer() == 0)
				board->highlightWhitePieces();
			else
				board->highlightBlackPieces();

			board->setHighlightedBall(this);
		}
		else if (board->getCurrentPlayer() == getPlayer()
				&& board->isHuman(getPlayer()) && !board->getCurrentTurn()->isBallThrown()) {
			
			if (board->highlightPieces(this))
				board->setHighlightedBall(this);
			else
				shake();
		}
		else
			shake();
	}
	else {
		board->turnOffHighlighting();
		board->setHighlightedBall(NULL);
	}

	if (!board->isBeingEdited()) {
		if (board->getCurrentPlayer() == 0)
			board->getStatus()->setText("Current player: white.");
		else
			board->getStatus()->setText("Current player: red.");
	}
}

int Ball::getPlayer()
{
	return ballColor;
}

void Ball::moveTo(QPoint endPoint)
{
	timeline->setCurrentTime(200);

	QPoint direction = QPoint(endPoint.x() - getCoords().x(),
							  endPoint.y() - getCoords().y());
	animation->setItem(this);
	animation->clear();
	animation->setPosAt(1, QPoint(x() + 50 * direction.x() * scale(),
								  y() + 50 * direction.y() * scale()));

	timeline->start();
}

QPoint Ball::getCoords()
{
	return coords;
}

void Ball::setCoords(QPoint point)
{
	coords = point;
}

void Ball::changePosition(QPoint endPoint, bool visibly)
{
	if (visibly)
		moveTo(endPoint);

	board->getPiece(coords.x(), coords.y())->isHoldingBall(false);
	board->getPiece(endPoint.x(), endPoint.y())->isHoldingBall(true);

	coords = endPoint;
	board->movementFinished();
}

void Ball::changePosition(QPoint endPoint)
{
	changePosition(endPoint, true);
}

bool Ball::isHighlighted()
{
	return highlighted;
}

void Ball::isHighlighted(bool value)
{
	highlighted = value;

	if (value == true)
		setBrush(QBrush(QColor(Qt::green), Qt::SolidPattern));
	else
		setBrush(QBrush(color, Qt::SolidPattern));

}

Board * Ball::getBoard()
{
	return board;
}

void Ball::shake()
{
	timeline->setCurrentTime(300);

	animation->setItem(this);
	animation->clear();
	animation->setPosAt(0.25, QPoint(x() + 5 * scale(), y()));
	animation->setPosAt(0.75, QPoint(x() - 5 * scale(), y()));
	animation->setPosAt(1, QPoint(x(), y()));

	timeline->start();
}
