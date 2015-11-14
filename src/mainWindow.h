#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QApplication>
#include <QGraphicsScene>
#include <QTextCodec>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QLabel>
#include <QStatusBar>
#include <QMessageBox>
#include <QTextDocument>
#include <QTextDocumentWriter>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QPoint>
#include "graphics.h"
#include "board.h"
#include "square.h"
#include "piece.h"
#include "ball.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	Graphics *gview;
	QGraphicsScene *scene;
	QMenu *newGameMenu, *gameplayMenu, *optionsMenu, *editorMenu, *helpMenu;
	QAction *humanHumanAct, *humanCompAct, *compHumanAct, *compCompAct, *closeAct;
	//QAction *saveAct, *saveAsAct, *loadAct;
	QAction *confirmTurnAct, *undoMovementAct, *redoMovementAct;
	QAction *moveAsComputerAct, *giveHintAct, *turnOnEditorAct;
	QAction *computerAutomaticAct, *stopGeneratingAct;
	QAction *confirmEditingWhitePlaysAct, *confirmEditingBlackPlaysAct, *cancelEditingAct;
	QAction *showInfoAct;
	QLabel *status;
	QTimer *timer;

	Board *board;
	QString filename;
	QApplication *app;

	bool informed;

public:
	MainWindow(QApplication *);
	~MainWindow();

	 void resizeEvent(QResizeEvent *);
	 void closeEvent(QCloseEvent *);
	 void confirmEditing(int);
	 bool parseState(QString);
	 void generateBoard(bool, bool);
	 void runConnects();
	 void runGameMode();
	 void runEditMode();

private slots:
	 void showInfo();
	 void endGame();
	 void confirmTurn();
	 void undoMovement();
	 void redoMovement();
	 void turnOnEditor();
	 void cancelEditing();
	 void confirmEditingWhitePlays();
	 void confirmEditingBlackPlays();
	 void saveBoard();
	 void saveBoardAs();
	 void loadBoard();
	 void corruptedBoard();
	 void giveHint();
	 void runEvents();
	 void stopGenerating();
	 void generateHumanHuman();
	 void generateHumanComp();
	 void generateCompHuman();
	 void generateCompComp();
	 void moveAsComputer();
	 void setComputerAsAutomatic();
};

#endif // MAINWINDOW_H
