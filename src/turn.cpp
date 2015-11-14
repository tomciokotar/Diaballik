#include "turn.h"

Turn::Turn()
{
	setUp();

	totalMovementsCount = 0;
	ballThrown = false;
}

Turn::Turn(QString turnState, Board *boardGiven)
{
	QTextStream input(&turnState);

	input >> totalMovementsCount;

	setUp();

	for (int i = 0; i < 3; i++) {
		int currentPiece;
		input >> currentPiece;

		if (currentPiece > 0) {
			if (currentPiece == 8) {
				ball = boardGiven->getWhiteBall();
				movesOrder.push_back(2);
			}
			else {
				pieces[i] = boardGiven->getWhitePiece(currentPiece - 1);
				movesOrder.push_back(donePieceMovementsCount);
				donePieceMovementsCount++;
			}
		}
		else if (currentPiece < 0) {
			if (currentPiece == -8) {
				ball = boardGiven->getBlackBall();
				movesOrder.push_back(2);
			}
			else {
				pieces[i] = boardGiven->getBlackPiece(-currentPiece - 1);
				movesOrder.push_back(donePieceMovementsCount);
				donePieceMovementsCount++;
			}
		}

		for (int j = 0, x, y; j < 2; j++) {
			input >> x >> y;
			coordsArray[i][j] = QPoint(x,y);
		}
	}

	if (ball != NULL)
		ballThrown = true;
	else
		ballThrown = false;
}

Turn::~Turn()
{
}

int Turn::getDoneMovementsCount()
{
	return movesOrder.size();
}

int Turn::getTotalMovementsCount()
{
	return totalMovementsCount;
}

int Turn::getDonePieceMovementsCount()
{
	return donePieceMovementsCount;
}

bool Turn::isBallThrown()
{
	return ballThrown;
}

void Turn::newMovement(Piece *pieceGiven, QPoint endPoint)
{
	int movesOrderSize = movesOrder.size();

	coordsArray[movesOrderSize][0] = pieceGiven->getCoords();
	coordsArray[movesOrderSize][1] = endPoint;

	pieces[movesOrderSize] = pieceGiven;

	Board *board = pieces[movesOrderSize]->getBoard();
	while (!board->nextTurns.empty())
		delete board->nextTurns.pop();

	movesOrder.push_back(donePieceMovementsCount);
	totalMovementsCount = movesOrder.size();
	donePieceMovementsCount++;

	pieceGiven->changePosition(endPoint);
}

void Turn::newMovement(Ball *ballGiven, QPoint endPoint)
{
	int movesOrderSize = movesOrder.size();

	coordsArray[movesOrderSize][0] = ballGiven->getCoords();
	coordsArray[movesOrderSize][1] = endPoint;

	pieces[movesOrderSize] = NULL;
	ball = ballGiven;
	ballThrown = true;

	Board *board = ball->getBoard();
	while (!board->nextTurns.empty())
		delete board->nextTurns.pop();

	movesOrder.push_back(2);
	totalMovementsCount = movesOrder.size();

	ballGiven->changePosition(endPoint);
}

void Turn::undoMovement(bool visibly)
{
	if (getDoneMovementsCount() == 0)
		return;

	int lastMovement = movesOrder.back();
	movesOrder.pop_back();

	int movesOrderSize = movesOrder.size();

	if (lastMovement == 2) {
		ball->changePosition(coordsArray[movesOrderSize][0], visibly);
		ballThrown = false;
	}
	else {
		pieces[movesOrderSize]->changePosition(coordsArray[movesOrderSize][0], visibly);
		donePieceMovementsCount--;
	}
}

void Turn::undoMovement()
{
	undoMovement(true);
}

void Turn::redoMovement(bool visibly)
{
	if (movesOrder.size() == totalMovementsCount)
		return;

	int movesOrderSize = movesOrder.size();

	if (pieces[movesOrderSize] == NULL) {
		ball->changePosition(coordsArray[movesOrderSize][1], visibly);
		ballThrown = true;
		movesOrder.push_back(2);
	}
	else {
		pieces[movesOrderSize]->changePosition(coordsArray[movesOrderSize][1], visibly);
		movesOrder.push_back(donePieceMovementsCount);
		donePieceMovementsCount++;
	}
}

void Turn::redoMovement()
{
	redoMovement(true);
}

QString Turn::saveTurn()
{
	QString turnState = "";
	QTextStream output(&turnState);

	output << totalMovementsCount << "\n";

	for (int i = 0; i < 3; i++) {
		if (pieces[i] != NULL) {
			if (pieces[i]->getPlayer() == 0)
				output << pieces[i]->getNumber() + 1 << " ";
			else
				output << -(pieces[i]->getNumber() + 1) << " ";
		}
		else if (ball != NULL && i < totalMovementsCount) {
			if (ball->getPlayer() == 0)
				output << "8 ";
			else
				output << "-8 ";
		}
		else
			output << "0 ";

		for (int j = 0; j < 2; j++)
			output << coordsArray[i][j].x() << " " << coordsArray[i][j].y() << " ";

		output << "\n";
	}

	return turnState;
}

void Turn::setUp()
{
	pieces[0] = NULL;
	pieces[1] = NULL;
	pieces[2] = NULL;
	ball = NULL;

	donePieceMovementsCount = 0;
}
