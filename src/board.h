#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QStack>
#include <QGraphicsScene>
#include <QTextStream>
#include <QLabel>
#include <QTimer>

class Piece;
class Ball;
class Square;
class Turn;
class Movement;

#include "piece.h"
#include "ball.h"
#include "square.h"
#include "turn.h"
#include "movement.h"

class Board : public QObject
{
	Q_OBJECT

	Square *squares[8][8];   // WTF?!
	Piece *pieces[7][7];
	Piece *whitePieces[7], *blackPieces[7];
	Ball *whiteBall, *blackBall;
	Piece *highlightedPiece;
	Ball *highlightedBall;
	Turn *currentTurn;

	QGraphicsScene *scene;
	QLabel *status;

	int currentPlayer, whoIsGenerating;
	bool human[2];
	int winner;
	bool beingEdited;
	bool generatingStopped;
	int alpha, beta;
	bool computerAutomatic, generatingInProgress;

public:
	QStack<Turn *> previousTurns, nextTurns;
	QStack<Movement> previousModifications, nextModifications;
	QStack<Movement> dfsMovesStack, bestMovesStack;
	QTimer timer;

	Board(bool, bool, QGraphicsScene *, QLabel *);
	Board(QString, QGraphicsScene *, QLabel *);
	~Board();

	Square * getSquare(int, int);
	Piece * getPiece(int, int);
	Piece * getWhitePiece(int);
	Piece * getBlackPiece(int);
	Ball * getWhiteBall();
	Ball * getBlackBall();
	Piece * getHighlightedPiece();
	Ball * getHighlightedBall();
	Turn * getCurrentTurn();

	void turnOffHighlighting();
	bool highlightSquares(Piece *);
	bool highlightPieces(Ball *);
	void setHighlightedPiece(Piece *);
	void setHighlightedBall(Ball *);
	void highlightAllSquares();
	void highlightWhitePieces();
	void highlightBlackPieces();
	bool highlightSquare(int, int);
	
	bool isThrowPossible(Square *, Ball *);
	int getCurrentPlayer();
	void putPiece(QPoint, Piece *);
	void movementFinished();
	
	void newTurn();
	void confirmTurn(bool);
	void confirmTurn();
	
	int getWinner();
	int checkWhoWon(int);
	
	bool isBlocking(int);
	Piece * isSomethingNextTo(Piece *);
	int howManyStick(int);
	
	void undoWholeTurn();
	void redoWholeTurn();
	
	bool isBeingEdited();
	void turnOnEditor();
	void undoModification();
	void redoModification();
	void cancelEditing();
	void confirmEditing(int);
	
	bool isBoardOK();
	QString saveBoard();
	
	int countPoints(int);
	void generateTurn(int);
	void giveHint();
	void moveAsComputer();
	void dfs(int, int, int, int);
	void pieceDfs(int, int, int, int, int, int, int, int, int);
	void ballDfs(int, int, int, int, int, int, QPoint);
	void stopDfs();
	void correctBestMoves();
	
	Piece * getPieceById(int, int);
	Ball * getBallById(int);
	
	bool isComputerAutomatic();
	void isComputerAutomatic(bool);
	bool isHuman(int);
	bool isGeneratingInProgress();
	
	QLabel * getStatus();
	void setUpBoard();

signals:
	void theEnd();
	void corruptedBoard();
	void runEvents();

private slots:
	void automaticTurn();
};

#endif // BOARD_H
