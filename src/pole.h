#ifndef POLE_H
#define POLE_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPoint>

class Plansza;

#include "plansza.h"

class Pole : public QGraphicsRectItem
{
	Plansza *plansza;
	QColor kolor;
	QPoint wsplrz;
	Pionek *stojacy;

	bool podswietlone;

public:
	Pole(int, int, Plansza *, QGraphicsScene *);
	~Pole();

	void mousePressEvent(QGraphicsSceneMouseEvent *);
	void podswietl(bool);
	void podswietl();
	void zgas();
	bool swieci();
	QPoint wspl();
	void ustawWspl(QPoint);
	Pionek * pionek();
	void postaw(Pionek *);
};

#endif // POLE_H
