#ifndef PILKA_H
#define PILKA_H

#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QPoint>

class Plansza;

#include "plansza.h"

class Pilka : public QGraphicsEllipseItem
{
	Plansza *plansza;
	QGraphicsItemAnimation *anim;
	QPoint wsplrz;
	QColor kolor;

	int kolorPilki;
	bool ruszana;

public:
	QTimeLine *timeline;

	Pilka(int, int, int, Plansza *, QGraphicsScene *);
	~Pilka();

	void mousePressEvent(QGraphicsSceneMouseEvent *);
	int gracz();
	void przesun(QPoint);
	QPoint wspl();
	void ustawWspl(QPoint);
	void zmianaPozycji(QPoint, bool);
	void zmianaPozycji(QPoint);
	bool ruchawa();
	void setRuchawa(bool);
	Plansza * dajPlansze();
	void potrzasnij();

};

#endif // PILKA_H
