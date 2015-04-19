#include "przes.h"

Przes::Przes()
{
}

Przes::Przes(QPoint pocz, Pionek *pion, QPoint kon)
{
	wspl[0] = pocz;
	wspl[1] = kon;
	ruszany = pion;
	ruszana = NULL;
}

Przes::Przes(QPoint pocz, Pilka *pilk, QPoint kon)
{
	wspl[0] = pocz;
	wspl[1] = kon;
	ruszany = NULL;
	ruszana = pilk;
}

QPoint Przes::skad()
{
	return wspl[0];
}

QPoint Przes::dokad()
{
	return wspl[1];
}

Pionek * Przes::pionek()
{
	return ruszany;
}

Pilka * Przes::pilka()
{
	return ruszana;
}
