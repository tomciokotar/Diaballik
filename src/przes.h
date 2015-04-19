#ifndef PRZES_H
#define PRZES_H

#include <QPoint>

class Pionek;
class Pilka;

#include "pionek.h"
#include "pilka.h"

class Przes
{
	QPoint wspl[2];
	Pionek *ruszany;
	Pilka *ruszana;

public:
	Przes();
	Przes(QPoint, Pionek*, QPoint);
	Przes(QPoint, Pilka*, QPoint);

	QPoint skad();
	QPoint dokad();
	Pionek * pionek();
	Pilka * pilka();
};

#endif // PRZES_H
