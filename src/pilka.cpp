#include "pilka.h"

Pilka::Pilka(int x, int y, int kol, Plansza *pl, QGraphicsScene *scene)
	: QGraphicsEllipseItem(10 + x*50, 10 + y*50, 30, 30, 0 ,scene)
{
	if (kol == 0)
		kolor = QColor(Qt::blue);
	else
		kolor = QColor(Qt::black);

	setBrush(QBrush(kolor, Qt::SolidPattern));

	plansza = pl;
	kolorPilki = kol;
	ruszana = false;
	wsplrz = QPoint(x,y);

	timeline = new QTimeLine(200);
	timeline->setCurrentTime(200);
	timeline->setFrameRange(0, 20);

	anim = new QGraphicsItemAnimation();
	anim->setItem(this);
	anim->setTimeLine(timeline);
}

Pilka::~Pilka()
{
	delete timeline;
	delete anim;
}

void Pilka::mousePressEvent(QGraphicsSceneMouseEvent *)
{
	if (!ruchawa()) {
		plansza->zgasPola();

		if (plansza->edytor()) {
			if (gracz() == 0)
				plansza->zapalBiale();
			else
				plansza->zapalCzarne();

			plansza->setRuchawa(this);
		}
		else if (plansza->gracz() == gracz() && plansza->czyCzlowiek(gracz()) && !plansza->ruch()->czyPodano()) {
			if (plansza->podswietlPola(this))
				plansza->setRuchawa(this);
			else
				potrzasnij();
		}
		else
			potrzasnij();
	}
	else {
		plansza->zgasPola();
		plansza->setRuchawa(NULL);
	}

	if (!plansza->edytor()) {
		if (plansza->gracz() == 0)
			plansza->statusik()->setText("Aktualny gracz: biały.");
		else
			plansza->statusik()->setText("Aktualny gracz: czerwony.");
	}
}

int Pilka::gracz()
{
	return kolorPilki;
}

void Pilka::przesun(QPoint pkt)
{
	timeline->setCurrentTime(200);

	QPoint kier = QPoint(pkt.x() - wspl().x(), pkt.y() - wspl().y());
	anim->setItem(this);
	anim->clear();
	anim->setPosAt(1, QPoint(x() + 50 * kier.x() * scale(), y() + 50 * kier.y() * scale()));

	timeline->start();
}

QPoint Pilka::wspl()
{
	return wsplrz;
}

void Pilka::ustawWspl(QPoint pkt)
{
	wsplrz = pkt;
}

void Pilka::zmianaPozycji(QPoint pkt, bool jawnie)
{
	if (jawnie)
		przesun(pkt);

	plansza->pionek(wsplrz.x(), wsplrz.y())->ustawPilke(false);
	plansza->pionek(pkt.x(), pkt.y())->ustawPilke(true);

	wsplrz = pkt;
	plansza->przesunieto();
}

void Pilka::zmianaPozycji(QPoint pkt)
{
	zmianaPozycji(pkt, true);
}

bool Pilka::ruchawa()
{
	return ruszana;
}

void Pilka::setRuchawa(bool r)
{
	ruszana = r;

	if (r == true)
		setBrush(QBrush(QColor(Qt::green), Qt::SolidPattern));
	else
		setBrush(QBrush(kolor, Qt::SolidPattern));

}

Plansza * Pilka::dajPlansze()
{
	return plansza;
}

void Pilka::potrzasnij()
{
	timeline->setCurrentTime(300);

	anim->setItem(this);
	anim->clear();
	anim->setPosAt(0.25, QPoint(x() + 5 * scale(), y()));
	anim->setPosAt(0.75, QPoint(x() - 5 * scale(), y()));
	anim->setPosAt(1, QPoint(x(), y()));

	timeline->start();
}
