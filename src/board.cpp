#include "board.h"

int directions[8][2] = {{1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};

Board::Board(bool isPlayerAHuman, bool isPlayerBHuman, QGraphicsScene *sceneGiven, QLabel *statusGiven)
{
	human[0] = isPlayerAHuman;
	human[1] = isPlayerBHuman;
	currentPlayer = 0;

	scene = sceneGiven;
	status = statusGiven;

	setUpBoard();

	for (int i = 0; i < 7; i++) {
		whitePieces[i] = new Piece(i, 0, 0, i, this, scene);
		pieces[i][0] = whitePieces[i];
		getSquare(i,0)->putPiece(whitePieces[i]);
	}

	for (int i = 0; i < 7; i++) {
		blackPieces[i] = new Piece(i, 6, 1, i, this, scene);
		pieces[i][6] = blackPieces[i];
		getSquare(i,6)->putPiece(blackPieces[i]);
	}
	
	whiteBall = new Ball(3, 0, 0, this, scene);
	getPiece(3,0)->isHoldingBall(true);
	blackBall = new Ball(3, 6, 1, this, scene);
	getPiece(3,6)->isHoldingBall(true);
}

Board::Board(QString boardState, QGraphicsScene *sceneGiven, QLabel *statusGiven)
{
	QTextStream input(&boardState);

	int playerType[2];
	input >> currentPlayer >> playerType[0] >> playerType[1];
	human[0] = (bool) playerType[0];
	human[1] = (bool) playerType[1];

	scene = sceneGiven;
	status = statusGiven;

	setUpBoard();

	for (int i = 0, x, y; i < 7; i++) {
		input >> x >> y;
		whitePieces[i] = new Piece(x, y, 0, i, this, scene);
		pieces[x][y] = whitePieces[i];
		getSquare(x,y)->putPiece(whitePieces[i]);
	}

	for (int i = 0, x, y; i < 7; i++) {
		input >> x >> y;
		blackPieces[i] = new Piece(x, y, 1, i, this, scene);
		pieces[x][y] = blackPieces[i];
		getSquare(x,y)->putPiece(blackPieces[i]);
	}

	int x,y;

	input >> x >> y;
	whiteBall = new Ball(x, y, 0, this, scene);
	getPiece(x,y)->isHoldingBall(true);

	input >> x >> y;
	blackBall = new Ball(x, y, 1, this, scene);
	getPiece(x,y)->isHoldingBall(true);

	if (!isBoardOK())
		emit corruptedBoard();

	int n;
	input >> n;

	for (int i = 0; i < n; i++) {
		QString turnState = "";
		QTextStream turnInput(&turnState);

		for (int j = 0, currentObject; j < 16; j++) {
			input >> currentObject;
			turnInput << currentObject << " ";
		}
		
		previousTurns.push(new Turn(turnState, this));
	}
}

Board::~Board()
{
	stopDfs();

	for (int i = 0; i < 7; i++) {
		 if (whitePieces[i] != NULL)
			 whitePieces[i]->timeline->setCurrentTime(300);
		 if (blackPieces[i] != NULL)
			 blackPieces[i]->timeline->setCurrentTime(300);
	 }

	 if (whiteBall != NULL)
		 whiteBall->timeline->setCurrentTime(100);
	 if (blackBall != NULL)
		 blackBall->timeline->setCurrentTime(100);

	 qDeleteAll(scene->items());

	 delete currentTurn;
	 while (!previousTurns.empty())
		 delete previousTurns.pop();

	 while (!previousTurns.empty())
		 delete previousTurns.pop();
}

Square * Board::getSquare(int i, int j)
{
	return squares[i][j];
}

Piece * Board::getPiece(int i, int j)
{
	return pieces[i][j];
}

Piece * Board::getWhitePiece(int i)
{
	return whitePieces[i];
}

Piece * Board::getBlackPiece(int i)
{
	return blackPieces[i];
}

Ball * Board::getWhiteBall()
{
	return whiteBall;
}

Ball * Board::getBlackBall()
{
	return blackBall;
}

void Board::turnOffHighlighting()
{
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++) {
			getSquare(i,j)->turnOffHighlight();

			if (getSquare(i,j)->getPiece() != NULL)
				getSquare(i,j)->getPiece()->turnOffPurpleHighlight();
		}
	
	whiteBall->isHighlighted(false);
	blackBall->isHighlighted(false);
}

bool Board::highlightSquares(Piece *pieceGiven)
{
	int i = pieceGiven->getCoords().x(), j = pieceGiven->getCoords().y();

		bool isAnythingHighlighted = false;

		for (int k = 0; k < 4; k++)
			if (highlightSquare(i + directions[k][0], j + directions[k][1]))
				isAnythingHighlighted = true;

		return isAnythingHighlighted;
}

bool Board::highlightPieces(Ball *ballGiven)
{
	QPoint coords = ballGiven->getCoords();

	for (int k = 0; k < 8; k++)
		for (int i = coords.x() + directions[k][0], j = coords.y() + directions[k][1];
			 i >= 0 && i < 7 && j >= 0 && j < 7; i += directions[k][0], j += directions[k][1])
			if (!isThrowPossible(getSquare(i,j), ballGiven))
				break;

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			if (getSquare(i,j)->getPiece() != NULL)
				if (getSquare(i,j)->getPiece()->isPurpleHighlighted())
					return true;

	return false;
}

bool Board::isThrowPossible(Square *squareGiven, Ball *ballGiven)
{
	if (squareGiven->getPiece() != NULL) {
		if (squareGiven->getPiece()->getPlayer() == ballGiven->getPlayer()) {
			squareGiven->getPiece()->highlightInPurple();
			return true;
		}
		else
			return false;
	}

	return true;
}

int Board::getCurrentPlayer()
{
	return currentPlayer;
}

Piece * Board::getHighlightedPiece()
{
	return highlightedPiece;
}

Ball * Board::getHighlightedBall()
{
	return highlightedBall;
}

void Board::setHighlightedPiece(Piece *pieceGiven)
{
	if (highlightedPiece != NULL)
		highlightedPiece->isGreenHighlighted(false);

	if (pieceGiven != NULL) {
		pieceGiven->isGreenHighlighted(true);
		setHighlightedBall(NULL);
	}

	highlightedPiece = pieceGiven;
}

void Board::setHighlightedBall(Ball *ballGiven)
{
	if (highlightedBall != NULL)
		highlightedBall->isHighlighted(false);

	if (ballGiven != NULL) {
		ballGiven->isHighlighted(true);
		setHighlightedPiece(NULL);
	}

	highlightedBall = ballGiven;
}

void Board::movementFinished()
{
	turnOffHighlighting();
	setHighlightedPiece(NULL);
	setHighlightedBall(NULL);
}

void Board::putPiece(QPoint point, Piece *pieceGiven)
{
	pieces[point.x()][point.y()] = pieceGiven;
}

Turn * Board::getCurrentTurn()
{
	return currentTurn;
}

void Board::newTurn()
{
	currentPlayer ^= 1;
	currentTurn = new Turn();

	if (getCurrentPlayer() == 0)
		status->setText("Current player: white.");
	else
		status->setText("Current player: red.");

	if (!human[currentPlayer] && isComputerAutomatic())
		moveAsComputer();
}

void Board::confirmTurn(bool doneAutomatically)
{
	if (getCurrentTurn()->getDoneMovementsCount() == 0 || (!human[currentPlayer] && !doneAutomatically)) {
		status->setText("The turn can't be confirmed right now!");
		return;
	}

	movementFinished();

	winner = checkWhoWon(currentPlayer);

	if (winner != -1)
		emit theEnd();
	else {
		previousTurns.push(currentTurn);
		newTurn();
	}
}

void Board::confirmTurn()
{
	confirmTurn(false);
}

int Board::getWinner()
{
	return winner;
}

int Board::checkWhoWon(int playerGiven)
{
	if (playerGiven == 0 && whiteBall->getCoords().y() == 6)
		return playerGiven;
	else if (playerGiven == 1 && blackBall->getCoords().y() == 0)
		return playerGiven;

	if (isBlocking(playerGiven^1) && howManyStick(playerGiven) >= 3)
		return playerGiven;
	else if (isBlocking(playerGiven) && howManyStick(playerGiven^1) >= 3)
		return playerGiven^1;

	return -1;
}

bool Board::isBlocking(int playerGiven)
{
	int currentColumn = 0;

	Piece *currentPiece = NULL;
	for (int i = 0; i < 7; i++)
		if (getPiece(0, i) != NULL && getPiece(0, i)->getPlayer() == playerGiven) {
			currentPiece = getPiece(0, i);
			break;
		}

	while (currentPiece != NULL) {
		currentPiece = isSomethingNextTo(currentPiece);
		currentColumn++;
	}

	if (currentColumn == 7)
		return true;
	else
		return false;

}

Piece * Board::isSomethingNextTo(Piece *pieceGiven)
{
	if (pieceGiven->getCoords().x() == 6)
		return NULL;

	int x = pieceGiven->getCoords().x(), y = pieceGiven->getCoords().y();

	for (int i = -1; i <= 1; i++)
		if (y+i >= 0 && y+i < 7 && getSquare(x+1, y+i)->getPiece() != NULL
		   && getSquare(x+1, y+i)->getPiece()->getPlayer() == pieceGiven->getPlayer())
			return getSquare(x+1, y+i)->getPiece();

	return NULL;
}

int Board::howManyStick(int playerGiven)
{
	int result = 0;
	int x,y;

	for (int i = 0; i < 7; i++) {
		if (playerGiven == 0) {
			x = whitePieces[i]->getCoords().x(), y = whitePieces[i]->getCoords().y();

			if (y < 6 && getPiece(x, y+1) != NULL && getPiece(x, y+1)->getPlayer() != playerGiven)
				result++;
		}
		else {
			x = blackPieces[i]->getCoords().x(), y = blackPieces[i]->getCoords().y();

			if (y > 0 && getPiece(x, y-1) != NULL && getPiece(x, y-1)->getPlayer() != playerGiven)
				result++;
		}
	}

	return result;
}

void Board::undoWholeTurn()
{
	if (previousTurns.empty()) {
		status->setText("Can't undo the turn.");
		return;
	}

	if (currentTurn->getTotalMovementsCount() > 0)
		nextTurns.push(currentTurn);
	else
		delete currentTurn;

	currentTurn = previousTurns.pop();
	currentPlayer ^= 1;
}

void Board::redoWholeTurn()
{
	if (nextTurns.empty()) {
		status->setText("Can't redo the turn.");
		return;
	}

	previousTurns.push(currentTurn);
	currentTurn = nextTurns.pop();
	currentPlayer ^= 1;
}

void Board::highlightAllSquares()
{
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			if (getSquare(i,j)->getPiece() == NULL)
				getSquare(i,j)->highlight();
}

void Board::highlightWhitePieces()
{
	for (int i = 0; i < 7; i++)
		if (!whitePieces[i]->isHoldingBall())
			whitePieces[i]->highlightInPurple();
}

void Board::highlightBlackPieces()
{
	for (int i = 0; i < 7; i++)
		if (!blackPieces[i]->isHoldingBall())
			blackPieces[i]->highlightInPurple();
}

bool Board::isBeingEdited()
{
	return beingEdited;
}

void Board::turnOnEditor()
{
	movementFinished();
	beingEdited = true;
}

void Board::undoModification()
{
	if (previousModifications.empty()) {
		status->setText("Can't undo the modification.");
		return;
	}

	Movement lastMovement = previousModifications.pop();

	if (lastMovement.getPiece() != NULL)
		lastMovement.getPiece()->changePosition(lastMovement.startPoint());
	else
		lastMovement.getBall()->changePosition(lastMovement.startPoint());

	nextModifications.push(lastMovement);
}

void Board::redoModification()
{
	if (nextModifications.empty()) {
		status->setText("Can't redo the modification.");
		return;
	}

	Movement nextMovement = nextModifications.pop();

	if (nextMovement.getPiece() != NULL)
		nextMovement.getPiece()->changePosition(nextMovement.endPoint());
	else
		nextMovement.getBall()->changePosition(nextMovement.endPoint());

	previousModifications.push(nextMovement);
}

void Board::cancelEditing()
{
	while (!previousModifications.empty())
		undoModification();

	nextModifications.clear();
	beingEdited = false;

	movementFinished();
}

void Board::confirmEditing(int nextPlayer)
{
	previousModifications.clear();
	nextModifications.clear();
	beingEdited = false;

	delete currentTurn;
	currentTurn = new Turn();

	while (!previousTurns.empty())
		delete previousTurns.pop();

	while (!nextTurns.empty())
		delete nextTurns.pop();

	movementFinished();
	currentPlayer = nextPlayer;
	
	winner = checkWhoWon(currentPlayer);

	if (winner != -1)
		emit theEnd();
	else {
		winner = checkWhoWon(currentPlayer^1);

		if (winner != -1)
			emit theEnd();
	}
}

bool Board::isBoardOK()
{
	int playerAWon = checkWhoWon(0);
	int playerBWon = checkWhoWon(1);

	if (playerAWon == -1 || playerBWon == -1)
		return true;
	else if (playerAWon != playerBWon)
		return false;

	return true;
}

QString Board::saveBoard()
{
	QString boardState = "";
	QTextStream output(&boardState);
	
	int currentTurnMovesCount = getCurrentTurn()->getDoneMovementsCount();
	
	while (getCurrentTurn()->getDoneMovementsCount() > 0)
		getCurrentTurn()->undoMovement(false);

	output << currentPlayer <<  " " << (int) human[0] << " "
		   << (int) human[1] << "\n";

	for (int i = 0; i < 7; i++)
		output << getWhitePiece(i)->getCoords().x() << " "
			   << getWhitePiece(i)->getCoords().y() << "\n";

	for (int i = 0; i < 7; i++)
		output << getBlackPiece(i)->getCoords().x() << " "
			   << getBlackPiece(i)->getCoords().y() << "\n";

	output << getWhiteBall()->getCoords().x() << " "
		   << getWhiteBall()->getCoords().y() << "\n";
		   
	output << getBlackBall()->getCoords().x() << " "
		   << getBlackBall()->getCoords().y() << "\n";

	output <<  previousTurns.size() << "\n";

	for (int i = 0; i < previousTurns.size(); i++)
		output << previousTurns.at(i)->saveTurn();

	while (getCurrentTurn()->getDoneMovementsCount() < currentTurnMovesCount)
		getCurrentTurn()->redoMovement(false);

	return boardState;
}

int Board::countPoints(int playerGiven)
{
	if (playerGiven == 0) {
		if (getBlackBall()->getCoords().y() == 0)
			return -1000;
		else if (getWhiteBall()->getCoords().y() == 6)
			return 1000;
	}
	else {
		if (getWhiteBall()->getCoords().y() == 6)
			return -1000;
		else if (getBlackBall()->getCoords().y() == 0)
			return 1000;
	}

	int result = 0, factor;

	if (playerGiven == 0)
		factor = 1;
	else
		factor = -1;

	for (int i = 0; i < 7; i++) {
			result += getWhitePiece(i)->getCoords().y() * factor;
			result -= (6 - getBlackPiece(i)->getCoords().y()) * factor;
	}

	result += getWhiteBall()->getCoords().y() * factor;
	result -= (6 - getBlackBall()->getCoords().y()) * factor;

	return result - 2*howManyStick(playerGiven);
}

void Board::generateTurn(int playerGiven)
{
	if (generatingInProgress) {
		status->setText("The turn is currently being generated.");
		return;
	}
	
	whoIsGenerating = playerGiven;
	bestMovesStack.clear();
	dfsMovesStack.clear();
	generatingStopped = false;
	generatingInProgress = true;

	status->setText("A wise AI thinks... (Ctrl + A to stop thinking).");
	
	alpha = -1001;

	dfs(getCurrentTurn()->getDonePieceMovementsCount(), getCurrentTurn()->isBallThrown(), playerGiven, 0);

	if (generatingStopped) {
		status->setText("A wise AI doesn't think anymore :(");

		generatingInProgress = false;
		return;
	}

	timer.start();
	generatingInProgress = false;

	status->setText("A wise AI has figured out :)");
}

void Board::giveHint()
{
	if (!human[currentPlayer]) {
		status->setText("Hints aren't available for the computer.");
		return;
	}

	generateTurn(currentPlayer);
}

void Board::moveAsComputer()
{
	if (human[currentPlayer]) {
		status->setText("It's human's turn - computer can't move.");
		return;
	}

	generateTurn(currentPlayer);
}

void Board::dfs(int movesDone, int throwsDone, int playerGiven, int level)
{
	if (generatingStopped)
		return;

	if (level == 0 && alpha == 1000)
		return;
	if (level == 1 && alpha >= beta)
		return;

	if (movesDone < 2) {
		for (int i = 0; i < 7; i++)
			if (!getPieceById(playerGiven, i)->isHoldingBall()) {
				int x = getPieceById(playerGiven, i)->getCoords().x();
				int y = getPieceById(playerGiven, i)->getCoords().y();

				for (int j = 0; j < 4; j++)
					pieceDfs(i, x, y, directions[j][0], directions[j][1],
							 movesDone, throwsDone, playerGiven, level);
			}
	}

	if (throwsDone < 1) {
		QPoint ballCoords = getBallById(playerGiven)->getCoords();

		for (int k = 0; k < 8; k++)
			for (int i = ballCoords.x() + directions[k][0], j = ballCoords.y() + directions[k][1];
				 i >= 0 && i < 7 && j >= 0 && j < 7; i += directions[k][0], j += directions[k][1])
				 
				if (getSquare(i,j)->getPiece() != NULL) {
					if (getSquare(i,j)->getPiece()->getPlayer() == playerGiven)
						ballDfs(i, j, movesDone, throwsDone, playerGiven, level, ballCoords);
					else
						break;
				}
	}

	if (movesDone == 0 && throwsDone == 0)
		return;

	if (level == 0 && countPoints(playerGiven) == 1000) {
		alpha = 1000;

		correctBestMoves();
	}
	else if (level == 1 && countPoints(playerGiven^1) == -1000)
		beta = -1000;

	if (level == 0) {
		emit runEvents();

		beta = 1000;

		dfs(0, 0, playerGiven^1, 1);

		if (alpha < beta) {
			alpha = beta;

			correctBestMoves();
		}
	}
	else if (level == 1) {
		int points = countPoints(playerGiven^1);

		if (beta > points)
			beta = points;
	}
}

void Board::pieceDfs(int i, int x, int y, int dirX, int dirY,
					 int movesDone, int throwsDone, int playerGiven, int level)
{
	if (qMin(x + dirX, y + dirY) >= 0 && qMax(x + dirX, y + dirY) < 7
		&& getSquare(x + dirX, y + dirY)->getPiece() == NULL) {
			
		dfsMovesStack.push( Movement(QPoint(x,y), getPieceById(playerGiven, i),
									 QPoint(x + dirX, y + dirY)) );
		
		getPieceById(playerGiven, i)->changePosition(QPoint(x + dirX, y + dirY), false);
		dfs(movesDone + 1, throwsDone, playerGiven, level);
		
		getPieceById(playerGiven, i)->changePosition(QPoint(x, y), false);
		dfsMovesStack.pop();
	}
}

void Board::ballDfs(int x, int y, int movesDone, int throwsDone,
					int playerGiven, int level, QPoint ballCoords)
{
	dfsMovesStack.push( Movement(ballCoords, getBallById(playerGiven), QPoint(x,y)) );
	getBallById(playerGiven)->changePosition(QPoint(x,y), false);
	dfs(movesDone, throwsDone + 1, playerGiven, level);
	
	getBallById(playerGiven)->changePosition(ballCoords, false);
	dfsMovesStack.pop();
}

void Board::stopDfs()
{
	generatingStopped = true;
	timer.stop();
}

Piece * Board::getPieceById(int playerId, int number)
{
	if (playerId == 0)
		return whitePieces[number];
	else
		return blackPieces[number];
}

Ball * Board::getBallById(int playerId)
{
	if (playerId == 0)
		return whiteBall;
	else
		return blackBall;
}

bool Board::isComputerAutomatic()
{
	return computerAutomatic;
}

void Board::isComputerAutomatic(bool value)
{
	computerAutomatic = value;
}

bool Board::isHuman(int playerId)
{
	return human[playerId];
}

bool Board::isGeneratingInProgress()
{
	return generatingInProgress;
}

void Board::automaticTurn()
{
	if (!bestMovesStack.empty()) {
		Movement bestMove = bestMovesStack.pop();

		if (bestMove.getPiece() != NULL)
			getCurrentTurn()->newMovement(bestMove.getPiece(), bestMove.endPoint());
		else
			getCurrentTurn()->newMovement(bestMove.getBall(), bestMove.endPoint());
	}

	if (bestMovesStack.empty()) {
		timer.stop();

		if (!human[currentPlayer])
			confirmTurn(true);
	}
}

QLabel * Board::getStatus()
{
	return status;
}

void Board::setUpBoard()
{
	whoIsGenerating = 0;
	highlightedPiece = NULL;
	highlightedBall = NULL;
	winner = -1;
	beingEdited = false;
	generatingStopped = false;
	computerAutomatic = false;
	generatingInProgress = false;

	timer.setInterval(400);
	connect(&timer, SIGNAL(timeout()), this, SLOT(automaticTurn()));

	currentTurn = new Turn();

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++) {
			squares[i][j] = new Square(i, j, this, scene);
			pieces[i][j] = NULL;
		}
}

bool Board::highlightSquare(int x, int y)
{
	if (qMin(x,y) >= 0 && qMax(x,y) < 7 && getSquare(x,y)->getPiece() == NULL) {
		getSquare(x,y)->highlight();
		return true;
	}
	else
		return false;
}

void Board::correctBestMoves()
{
	bestMovesStack.clear();
	for (int i = dfsMovesStack.size() - 1; i >= 0; i--)
		bestMovesStack.push(dfsMovesStack.at(i));
}
