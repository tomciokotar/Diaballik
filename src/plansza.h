#ifndef PLANSZA_H
#define PLANSZA_H

#include <QObject>
#include <QStack>
#include <QGraphicsScene>
#include <QTextStream>
#include <QLabel>
#include <QTimer>

class Pionek;
class Pilka;
class Pole;
class Ruch;
class Przes;

#include "pionek.h"
#include "pilka.h"
#include "pole.h"
#include "ruch.h"
#include "przes.h"

class Plansza : public QObject
{
	Q_OBJECT

	Pole *pola[8][8];   // WTF?!
	Pionek *pionki[7][7];
	Pionek *biale[7], *czarne[7];
	Pilka *bialap, *czarnap;
	Pionek *ruszany;
	Pilka *ruszana;
	Ruch *aktualny;

	QGraphicsScene *scene;
	QLabel *status;

	int ktogra, ktogeneruje;
	bool czlowiek[2];
	int wygrany;
	bool edytowana;
	bool przerwac;
	int alfa, beta;
	bool automatycznie, czywtrakcie;

public:
	QStack<Ruch *> poprzednie, nastepne;
	QStack<Przes> popedyt, nastedyt, popgen, najlgen;
	QTimer zegar;

	Plansza(bool, bool, QGraphicsScene *, QLabel *);
	Plansza(QString, QGraphicsScene *, QLabel *);
	~Plansza();

	Pole * pole(int, int);
	Pionek * pionek(int, int);
	Pionek * bialy(int);
	Pionek * czarny(int);
	Pilka * biala();
	Pilka * czarna();
	Pionek * ruchawy();
	Pilka * ruchawa();
	Ruch * ruch();

	void zgasPola();
	bool podswietlPola(Pionek *);
	bool podswietlPola(Pilka *);
	bool czyMoznaPodawac(Pole *, Pilka *);
	int gracz();
	void setRuchawy(Pionek *);
	void setRuchawa(Pilka *);
	void ustawPionek(QPoint, Pionek *);
	void przesunieto();
	void nowyruch();
	void zatwierdzruch(bool);
	void zatwierdzruch();
	int zwyciezca();
	int ktoWygral(int);
	bool czyBlokuje(int);
	Pionek * czyCosStoi(Pionek *);
	int ileStyka(int);
	void cofnijCalyRuch();
	void powtorzCalyRuch();
	void zapalWszystko();
	void zapalBiale();
	void zapalCzarne();
	bool edytor();
	void odpalEdytor();
	void cofnijEdyt();
	void powtorzEdyt();
	void anulujEdycje();
	void zatwierdzEdycje(int);
	bool czyPlanszaOK();
	QString zapiszPlansze();
	int wyliczPremie(int);
	void wygenerujRuch(int);
	void dajHinta();
	void ruchKompa();
	void dfs(int, int, int, int);
	void dfsPionek(int, int, int, int, int, int, int, int, int);
	void dfsPilka(int, int, int, int, int, int, QPoint);
	void przerwij();
	Pionek * pionko(int, int);
	Pilka * pilko(int);
	bool automat();
	void setAutomat(bool);
	bool czyCzlowiek(int);
	bool wtrakcie();
	QLabel * statusik();
	void poustawiaj();
	bool podswietlacz(int, int);
	void poprawNajlRuchy();

signals:
	void koniec();
	void zlaPlansza();
	void puscEventy();

private slots:
	void autRuch();
};

#endif // PLANSZA_H
