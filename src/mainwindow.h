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
#include "grafika.h"
#include "plansza.h"
#include "pole.h"
#include "pionek.h"
#include "pilka.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	Grafika *gview;
	QGraphicsScene *scene;
	QMenu *nowagra, *rozgrywka, *opcje, *edytor, *pomoc;
	QAction *czlczl, *czlkomp, *kompczl, *kompkomp, *zapisz, *zapiszjako, *wczytaj, *zakoncz;
	QAction *zatwierdzruch, *cofnij, *powtorz;
	QAction *ruchkompa, *dajhinta, *edytuj, *automat, *anulujgen;
	QAction *zatwierdzedytb, *zatwierdzedytc, *anulujedyt;
	QAction *info;
	QLabel *status;
	QTimer *timer;

	Plansza *plansza;
	QString nazwapliku;
	QApplication *app;

	bool poinformowano;

public:
	MainWindow(QApplication *);
	~MainWindow();

	 void resizeEvent(QResizeEvent *);
	 void closeEvent(QCloseEvent *);
	 void zatwierdzEdycje(int);
	 bool parsuj(QString);
	 void generujPlansze(bool, bool);
	 void sconnectuj();
	 void trybGry();
	 void trybEdycji();

private slots:
	 void informacje();
	 void koniecGry();
	 void zatwierdz();
	 void cofnijruch();
	 void powtorzruch();
	 void odpalEdytor();
	 void anulujEdycje();
	 void zatwierdzEdycjeB();
	 void zatwierdzEdycjeC();
	 void zapiszPlansze();
	 void zapiszJako();
	 void wczytajPlansze();
	 void zlaPlansza();
	 void dajHinta();
	 void puscEventy();
	 void przerwij();
	 void generujCzlCzl();
	 void generujCzlKomp();
	 void generujKompCzl();
	 void generujKompKomp();
	 void poruszKompem();
	 void ustawAutomat();
};

#endif // MAINWINDOW_H
