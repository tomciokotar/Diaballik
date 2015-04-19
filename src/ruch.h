#ifndef RUCH_H
#define RUCH_H

#include <QObject>
#include <QPoint>
#include <QVector>
#include <QTextStream>

class Pionek;
class Pilka;
class Plansza;

#include "pionek.h"
#include "pilka.h"
#include "plansza.h"

class Ruch : public QObject
{
	Q_OBJECT

	QPoint wsplruchu[3][2];

	Pionek *pionek[3];
	Pilka *pilka;

	int aktruch, ilewogole;
	bool podano;

	QVector<int> kolejnosc;
	/*
	  0 - pierwszy ruch
	  1 - drugi ruch
	  2 - podanie
	*/

public:
	Ruch();
	Ruch(QString, Plansza *);
	~Ruch();

	int zrobiono();
	int ilewSumie();
	int ilePrzesunieto();
	bool czyPodano();
	void nowePrzesuniecie(Pionek *, QPoint);
	void nowePrzesuniecie(Pilka *, QPoint);
	void cofnij(bool);
	void cofnij();
	void powtorz(bool);
	void powtorz();
	QString zapiszRuch();
	void poustawiaj();
};

#endif // RUCH_H
