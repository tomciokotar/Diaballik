#ifndef PIONEK_H
#define PIONEK_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QPoint>

class Plansza;

#include "plansza.h"

class Pionek : public QGraphicsRectItem
{
	Plansza *plansza;
	QGraphicsItemAnimation *anim;
	QPoint wsplrz;
	QColor kolor;

	int kolorPionka, numer;
	bool czypilka;
	bool podswietlone;
	bool ruszany;

public:
	QTimeLine *timeline;

	Pionek(int, int, int, int, Plansza *, QGraphicsScene *);
	~Pionek();

	void mousePressEvent(QGraphicsSceneMouseEvent *);
	int gracz();
	void przesun(QPoint);
	QPoint wspl();
	void ustawWspl(QPoint);
	bool pilka();
	void ustawPilke(bool);
	bool swieci();
	void podswietl(bool);
	void podswietl();
	void zgas();
	void zmianaPozycji(QPoint, bool);
	void zmianaPozycji(QPoint);
	bool ruchawy();
	void setRuchawy(bool);
	Plansza * dajPlansze();
	void potrzasnij();
	int nr();
};

#endif // PIONEK_H
