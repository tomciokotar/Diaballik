#include "pole.h"

Pole::Pole(int x, int y, Plansza *pl, QGraphicsScene *scene)
	: QGraphicsRectItem(x*50, y*50, 50, 50, 0, scene)
{
	if ((x + y)%2 == 0)
		kolor = QColor("#aaccee");
	else
		kolor = QColor("#aabbee");

	setBrush(QBrush(kolor, Qt::SolidPattern));

	plansza = pl;
	wsplrz = QPoint(x,y);
	podswietlone = false;
	stojacy = NULL;
}

Pole::~Pole()
{
}

void Pole::mousePressEvent(QGraphicsSceneMouseEvent *)
{
	if (swieci())
		if (plansza->ruchawy() != NULL) {
			if (plansza->edytor()) {
				plansza->popedyt.push( Przes(plansza->ruchawy()->wspl(), plansza->ruchawy(), wspl()) );
				plansza->nastedyt.clear();

				plansza->ruchawy()->zmianaPozycji(wspl());
			}
			else
				plansza->ruch()->nowePrzesuniecie(plansza->ruchawy(), wspl());

			if (!plansza->edytor()) {
				if (plansza->gracz() == 0)
					plansza->statusik()->setText("Aktualny gracz: biały.");
				else
					plansza->statusik()->setText("Aktualny gracz: czerwony.");
			}
		}
}

void Pole::podswietl(bool jawnie)
{
	podswietlone = true;

	if (jawnie)
		setBrush(QBrush(QColor("#aa00ee"), Qt::SolidPattern));
}

void Pole::podswietl()
{
	podswietl(true);
}

void Pole::zgas()
{
	podswietlone = false;
	setBrush(QBrush(kolor, Qt::SolidPattern));
}

bool Pole::swieci()
{
	return podswietlone;
}

QPoint Pole::wspl()
{
	return wsplrz;
}

void Pole::ustawWspl(QPoint pkt)
{
	wsplrz = pkt;
}

Pionek * Pole::pionek()
{
	return stojacy;
}

void Pole::postaw(Pionek *pion)
{
	stojacy = pion;
}
