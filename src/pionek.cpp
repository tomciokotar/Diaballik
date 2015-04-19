#include "pionek.h"

Pionek::Pionek(int x, int y, int kol, int nr, Plansza *pl, QGraphicsScene *scene)
	: QGraphicsRectItem(5 + x*50, 5 + y*50, 40, 40, 0 ,scene)
{
	if (kol == 0)
		kolor = QColor(Qt::white);
	else
		kolor = QColor(Qt::red);

	setBrush(QBrush(kolor, Qt::SolidPattern));

	plansza = pl;
	kolorPionka = kol;
	numer = nr;
	czypilka = false;
	podswietlone = false;
	ruszany = false;
	wsplrz = QPoint(x,y);

	timeline = new QTimeLine(300);
	timeline->setCurrentTime(300);
	timeline->setFrameRange(0, 20);

	anim = new QGraphicsItemAnimation();
	anim->setItem(this);
	anim->setTimeLine(timeline);
}

Pionek::~Pionek()
{
	delete timeline;
	delete anim;
}

void Pionek::mousePressEvent(QGraphicsSceneMouseEvent *)
{
	if (swieci()) {
		if (plansza->ruchawa() != NULL) {
			if (plansza->edytor()) {
				plansza->popedyt.push( Przes(plansza->ruchawa()->wspl(), plansza->ruchawa(), wspl()) );
				plansza->nastedyt.clear();

				plansza->ruchawa()->zmianaPozycji(wspl());
			}
			else
				plansza->ruch()->nowePrzesuniecie(plansza->ruchawa(), wspl());
		}
	}
	else if (!ruchawy() && !pilka()) {
		plansza->zgasPola();

		if (plansza->edytor()) {
			plansza->zapalWszystko();
			plansza->setRuchawy(this);
		}
		else if (plansza->gracz() == gracz() && plansza->czyCzlowiek(gracz()) && plansza->ruch()->ilePrzesunieto() < 2) {
			if (plansza->podswietlPola(this))
				plansza->setRuchawy(this);
			else
				potrzasnij();
		}
		else
			potrzasnij();
	}
	else if (ruchawy()) {
		plansza->zgasPola();
		plansza->setRuchawy(NULL);
	}
	else {
		plansza->zgasPola();
		potrzasnij();
	}

	if (!plansza->edytor()) {
		if (plansza->gracz() == 0)
			plansza->statusik()->setText("Aktualny gracz: biały.");
		else
			plansza->statusik()->setText("Aktualny gracz: czerwony.");
	}
}

int Pionek::gracz()
{
	return kolorPionka;
}

void Pionek::przesun(QPoint pkt)
{
	timeline->setCurrentTime(300);

	QPoint kier = QPoint(pkt.x() - wspl().x(), pkt.y() - wspl().y());
	anim->setItem(this);
	anim->clear();
	anim->setPosAt(1, QPoint(x() + 50 * kier.x() * scale(), y() + 50 * kier.y() * scale()));

	timeline->start();
}

QPoint Pionek::wspl()
{
	return wsplrz;
}

void Pionek::ustawWspl(QPoint pkt)
{
	wsplrz = pkt;
}

bool Pionek::pilka()
{
	return czypilka;
}

void Pionek::ustawPilke(bool p)
{
	czypilka = p;
}

bool Pionek::swieci()
{
	return podswietlone;
}

void Pionek::podswietl(bool jawnie)
{
	podswietlone = true;

	if (jawnie)
		setBrush(QBrush(QColor("#aa00ee"), Qt::SolidPattern));
}

void Pionek::podswietl()
{
	podswietl(true);
}

void Pionek::zgas()
{
	podswietlone = false;
	setBrush(QBrush(kolor, Qt::SolidPattern));
}

void Pionek::zmianaPozycji(QPoint pkt, bool jawnie)
{
	if (jawnie)
		przesun(pkt);

	plansza->ustawPionek(wsplrz, NULL);
	plansza->ustawPionek(pkt, this);
	plansza->pole(wsplrz.x(), wsplrz.y())->postaw(NULL);
	plansza->pole(pkt.x(), pkt.y())->postaw(this);

	wsplrz = pkt;
	plansza->przesunieto();
}

void Pionek::zmianaPozycji(QPoint pkt)
{
	zmianaPozycji(pkt, true);
}

bool Pionek::ruchawy()
{
	return ruszany;
}

void Pionek::setRuchawy(bool r)
{
	ruszany = r;

	if (r == true)
		setBrush(QBrush(QColor(Qt::green), Qt::SolidPattern));
	else
		setBrush(QBrush(kolor, Qt::SolidPattern));
}

Plansza * Pionek::dajPlansze()
{
	return plansza;
}

void Pionek::potrzasnij()
{
	timeline->setCurrentTime(300);

	anim->setItem(this);
	anim->clear();
	anim->setPosAt(0.25, QPoint(x() + 5 * scale(), y()));
	anim->setPosAt(0.75, QPoint(x() - 5 * scale(), y()));
	anim->setPosAt(1, QPoint(x(), y()));

	timeline->start();
}

int Pionek::nr()
{
	return numer;
}
