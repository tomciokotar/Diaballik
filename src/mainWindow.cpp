#include "mainWindow.h"

MainWindow::MainWindow(QApplication *appGiven)
	: QMainWindow(0)
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	setMinimumSize(360, 407);


	gview = new Graphics(this);
	setCentralWidget(gview);

	scene = new QGraphicsScene(this);
	gview->setScene(scene);

	filename = "";
	informed = false;
	app = appGiven;

	newGameMenu = menuBar()->addMenu("New game");
	gameplayMenu = menuBar()->addMenu("Gameplay");
	optionsMenu = menuBar()->addMenu("Options");
	editorMenu = menuBar()->addMenu("Editor");
	helpMenu = menuBar()->addMenu("Help");

	humanHumanAct = new QAction("Human-Human", this);
	connect(humanHumanAct, SIGNAL(triggered()), this, SLOT(generateHumanHuman()));
	newGameMenu->addAction(humanHumanAct);

	humanCompAct = new QAction("Human-Computer", this);
	connect(humanCompAct, SIGNAL(triggered()), this, SLOT(generateHumanComp()));
	newGameMenu->addAction(humanCompAct);
	
	compHumanAct = new QAction("Computer-Human", this);
	connect(compHumanAct, SIGNAL(triggered()), this, SLOT(generateCompHuman()));
	newGameMenu->addAction(compHumanAct);

	compCompAct = new QAction("Computer-Computer", this);
	connect(compCompAct, SIGNAL(triggered()), this, SLOT(generateCompComp()));
	newGameMenu->addAction(compCompAct);

	/*saveAct = new QAction("Save", this);
	saveAct->setShortcut(Qt::CTRL + Qt::Key_S);
	connect(saveAct, SIGNAL(triggered()), this, SLOT(saveBoard()));
	newGameMenu->addAction(saveAct);

	saveAsAct = new QAction("Save as...", this);
	saveAsAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveBoardAs()));
	newGameMenu->addAction(saveAsAct);

	loadAct = new QAction("Load", this);
	connect(loadAct, SIGNAL(triggered()), this, SLOT(loadBoard()));
	newGameMenu->addAction(loadAct);*/

	closeAct = new QAction("Close", this);
	connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));
	newGameMenu->addAction(closeAct);


	confirmTurnAct = new QAction("Confirm turn", this);
	confirmTurnAct->setShortcut(Qt::CTRL + Qt::Key_F);
	connect(confirmTurnAct, SIGNAL(triggered()), this, SLOT(confirmTurn()));
	gameplayMenu->addAction(confirmTurnAct);

	undoMovementAct = new QAction("Undo move", this);
	undoMovementAct->setShortcut(Qt::CTRL + Qt::Key_Z);
	connect(undoMovementAct, SIGNAL(triggered()), this, SLOT(undoMovement()));
	gameplayMenu->addAction(undoMovementAct);

	redoMovementAct = new QAction("Redo move", this);
	redoMovementAct->setShortcut(Qt::CTRL + Qt::Key_Y);
	connect(redoMovementAct, SIGNAL(triggered()), this, SLOT(redoMovement()));
	gameplayMenu->addAction(redoMovementAct);


	moveAsComputerAct = new QAction("Make computer's move", this);
	connect(moveAsComputerAct, SIGNAL(triggered()), this, SLOT(moveAsComputer()));
	optionsMenu->addAction(moveAsComputerAct);

	giveHintAct = new QAction("Give hint", this);
	giveHintAct->setShortcut(Qt::CTRL + Qt::Key_H);
	connect(giveHintAct, SIGNAL(triggered()), this, SLOT(giveHint()));
	optionsMenu->addAction(giveHintAct);

	turnOnEditorAct = new QAction("Edit game", this);
	connect(turnOnEditorAct, SIGNAL(triggered()), this, SLOT(turnOnEditor()));
	optionsMenu->addAction(turnOnEditorAct);

	computerAutomaticAct = new QAction("Computer plays automatically", this);
	computerAutomaticAct->setCheckable(true);
	connect(computerAutomaticAct, SIGNAL(triggered()), this, SLOT(setComputerAsAutomatic()));
	optionsMenu->addAction(computerAutomaticAct);

	stopGeneratingAct = new QAction("Stop generating move", this);
	stopGeneratingAct->setShortcut(Qt::CTRL + Qt::Key_A);
	connect(stopGeneratingAct, SIGNAL(triggered()), this, SLOT(stopGenerating()));
	optionsMenu->addAction(stopGeneratingAct);


	confirmEditingWhitePlaysAct = new QAction("Confirm - white plays", this);
	connect(confirmEditingWhitePlaysAct, SIGNAL(triggered()), this, SLOT(confirmEditingWhitePlays()));
	editorMenu->addAction(confirmEditingWhitePlaysAct);

	confirmEditingBlackPlaysAct = new QAction("Confirm - red plays", this);
	connect(confirmEditingBlackPlaysAct, SIGNAL(triggered()), this, SLOT(confirmEditingBlackPlays()));
	editorMenu->addAction(confirmEditingBlackPlaysAct);

	cancelEditingAct = new QAction("Cancel", this);
	connect(cancelEditingAct, SIGNAL(triggered()), this, SLOT(cancelEditing()));
	editorMenu->addAction(cancelEditingAct);


	showInfoAct = new QAction("Instructions", this);
	showInfoAct->setShortcut(Qt::Key_F1);
	connect(showInfoAct, SIGNAL(triggered()), this, SLOT(showInfo()));
	helpMenu->addAction(showInfoAct);


	status = new QLabel(this);
	statusBar()->addWidget(status);


	board = NULL;
	generateBoard(true, true);
}

MainWindow::~MainWindow()
{
	delete board;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
	if (event->oldSize().width() != -1)
		gview->fitInView(scene->sceneRect());
}

void MainWindow::closeEvent(QCloseEvent *)
{
	stopGenerating();
}

void MainWindow::showInfo()
{
	QMessageBox::information(this, "Instructions",
		"Your goal is to bring your ball to your opponent's line. Good luck!");
}

void MainWindow::generateBoard(bool playerAHuman, bool playerBHuman)
{
	if (board != NULL) {
		if (board->isGeneratingInProgress()) {
			status->setText("You can't start a new game while a move is being generated.");
			return;
		}

		stopGenerating();

		delete board;
	}

	board = new Board(playerAHuman, playerBHuman, scene, status);
	runConnects();
}

void MainWindow::endGame()
{
	QString winner;

	if (board->getWinner() == 0)
		winner = "white";
	else
		winner = "red";

	QMessageBox::information(this,
		"Game over!", "Game over! Congratulations to the " + winner + " player!");
	
	generateBoard(true, true);
}

void MainWindow::confirmTurn()
{
	board->confirmTurn();
}

void MainWindow::undoMovement()
{
	stopGenerating();

	if (board->isBeingEdited())
		board->undoModification();
	else if (board->getCurrentTurn()->getDoneMovementsCount() > 0)
		board->getCurrentTurn()->undoMovement();
	else {
		board->undoWholeTurn();
		board->getCurrentTurn()->undoMovement();
	}
}

void MainWindow::redoMovement()
{
	stopGenerating();

	if (board->isBeingEdited())
		board->redoModification();
	else if (board->getCurrentTurn()->getDoneMovementsCount() < board->
									getCurrentTurn()->getTotalMovementsCount())
		board->getCurrentTurn()->redoMovement();
	else {
		board->redoWholeTurn();
		board->getCurrentTurn()->redoMovement();
	}
}

void MainWindow::turnOnEditor()
{
	stopGenerating();
	board->turnOnEditor();
	runEditMode();
}

void MainWindow::cancelEditing()
{
	board->cancelEditing();
	runGameMode();
}

void MainWindow::confirmEditing(int whoPlays)
{
		if (!board->isBoardOK()) {
			QMessageBox::information(this, "Invalid configuration",
			 "The configuration is invalid - currently both players are the winners. Just cancel or change it.");
		}
		else {
			board->confirmEditing(whoPlays);
			runGameMode();
		}
}

void MainWindow::confirmEditingWhitePlays()
{
	confirmEditing(0);
}

void MainWindow::confirmEditingBlackPlays()
{
	confirmEditing(1);
}

void MainWindow::saveBoard()
{
	stopGenerating();

	if (filename != "") {
		QTextDocumentWriter writer(filename);
		QTextDocument doc(board->saveBoard(), this);

		bool ok = writer.write(&doc);

		if (ok) {
			if (!informed && board->getCurrentTurn()->getDoneMovementsCount() > 0) {
				QMessageBox::information(this, "Game saved",
					"The game has been saved. Note: unconfirmed moves haven't been saved.");
				
				informed = true;
				status->setText("Game saved!");
			}
		}
		else
			QMessageBox::information(this, "Error", "Can't save the game to the file.");
	}
	else
		saveBoardAs();
}

void MainWindow::loadBoard()
{
	QString name = QFileDialog::getOpenFileName(this, "Load game...",
							QString(), ".txt files (*.txt);;All files (*)");

	QFile loadedFile(name);

	if (loadedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream input(&loadedFile);
		QString boardState = input.readAll();

		if (!parseState(boardState)) {
			QMessageBox::information(this, "Error", "The file seems to be corrupted.");

			loadedFile.close();
			return;
		}

		if (board != NULL) {
			if (board->isGeneratingInProgress()) {
				status->setText("Can't load a game while generating move.");
				return;
			}

			stopGenerating();

			delete board;
		}

		board = new Board(boardState, scene, status);
		runConnects();

		loadedFile.close();
	}
	else
		QMessageBox::information(this, "Error", "Can't load the game.");

}

bool MainWindow::parseState(QString boardState)
{
	QTextStream input(&boardState);

	for (int i = 0, a; i < 3; i++) {
		input >> a;

		if (a != 0 && a != 1)
			return false;
	}

	int squares[7][7];

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			squares[i][j] = 0;

	for (int i = 0, x, y; i < 14; i++) {
		input >> x >> y;

		if (qMin(x,y) < 0 || qMax(x,y) >= 7)
			return false;

		if (squares[x][y] != 0)
			return false;
		else
			squares[x][y] = i + 1;
	}

	int x,y;

	input >> x >> y;
	if (squares[x][y] < 1 || squares[x][y] > 7)
		return false;

	input >> x >> y;
	if (squares[x][y] < 8 || squares[x][y] > 14)
		return false;

	int n;
	input >> n;

	for (int i = 0, howMany; i < n; i++) {
		input >> howMany;

		if (howMany < 1 || howMany > 3)
			return false;

		for (int j = 0, currentPiece, start[2], end[2]; j < 3; j++) {
			input >> currentPiece >> start[0] >> end[0] >> start[1] >> end[1];

			if (currentPiece < -8 || currentPiece > 8)
				return false;

			if (currentPiece == 0)
				if (start[0] != 0 || start[1] != 0 || end[0] != 0 || end[1] != 0)
					return false;

			for (int k = 0; k < 2; k++)
				if (start[k] < 0 || start[k] >= 7 || end[k] < 0 || end[k] >= 7)
					return false;

			if (currentPiece != 0 && start[0] == start[1] && end[0] == end[1])
				return false;
		}
	}

	return true;
}

void MainWindow::saveBoardAs()
{
	stopGenerating();

	filename = QFileDialog::getSaveFileName(this, "Save game as...",
						 QString(), ".txt files (*.txt);;All files (*)");

	if (filename == "")
		QMessageBox::information(this, "Error", "Enter a filename to save.");
	else
		saveBoard();
}

void MainWindow::corruptedBoard()
{
	QMessageBox::information(this, "Error",
		 "The game loaded is invalid (both players are winning). New game will be generated.");

	generateBoard(true, true);
}

void MainWindow::giveHint()
{
	board->giveHint();
}

void MainWindow::runEvents()
{
	app->processEvents();
}

void MainWindow::stopGenerating()
{
	board->isComputerAutomatic(false);
	computerAutomaticAct->setChecked(false);

	board->stopDfs();
}

void MainWindow::generateHumanHuman()
{
	generateBoard(true, true);
}

void MainWindow::generateHumanComp()
{
	generateBoard(true, false);
}

void MainWindow::generateCompHuman()
{
	generateBoard(false, true);
}

void MainWindow::generateCompComp()
{
	generateBoard(false, false);
}

void MainWindow::moveAsComputer()
{
	if (board->isComputerAutomatic()) {
		status->setText("You can't do it, computer is playing automatically right now.");
		return;
	}

	board->moveAsComputer();
}

void MainWindow::setComputerAsAutomatic()
{
	if (board->isComputerAutomatic())
		stopGenerating();
	else {
		board->isComputerAutomatic(true);
		computerAutomaticAct->setChecked(true);

		board->moveAsComputer();
	}
}

void MainWindow::runConnects()
{
	connect(board, SIGNAL(theEnd()), this, SLOT(endGame()));
	connect(board, SIGNAL(corruptedBoard()), this, SLOT(corruptedBoard()));
	connect(board, SIGNAL(runEvents()), this, SLOT(runEvents()));

	runGameMode();
}

void MainWindow::runGameMode()
{
	//saveAct->setEnabled(true);
	//saveAsAct->setEnabled(true);
	confirmTurnAct->setEnabled(true);
	moveAsComputerAct->setEnabled(true);
	giveHintAct->setEnabled(true);
	turnOnEditorAct->setEnabled(true);
	computerAutomaticAct->setEnabled(true);
	stopGeneratingAct->setEnabled(true);
	confirmEditingWhitePlaysAct->setDisabled(true);
	confirmEditingBlackPlaysAct->setDisabled(true);
	cancelEditingAct->setDisabled(true);

	if (board->getCurrentPlayer() == 0)
		status->setText("Current player: white.");
	else
		status->setText("Current player: red.");
}

void MainWindow::runEditMode()
{
	//saveAct->setDisabled(true);
	//saveAsAct->setDisabled(true);
	confirmTurnAct->setDisabled(true);
	moveAsComputerAct->setDisabled(true);
	giveHintAct->setDisabled(true);
	turnOnEditorAct->setDisabled(true);
	computerAutomaticAct->setDisabled(true);
	stopGeneratingAct->setDisabled(true);
	confirmEditingWhitePlaysAct->setEnabled(true);
	confirmEditingBlackPlaysAct->setEnabled(true);
	cancelEditingAct->setEnabled(true);

	status->setText("Edit mode.");
}
