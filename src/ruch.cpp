#include "ruch.h"

Ruch::Ruch()
{
	poustawiaj();

	ilewogole = 0;
	podano = false;
}

Ruch::Ruch(QString stan, Plansza *plansza)
{
	QTextStream str(&stan);

	str >> ilewogole;

	poustawiaj();

	for (int i = 0; i < 3; i++) {
		int pion;
		str >> pion;

		if (pion > 0) {
			if (pion == 8) {
				pilka = plansza->biala();
				kolejnosc.push_back(2);
			}
			else {
				pionek[i] = plansza->bialy(pion - 1);
				kolejnosc.push_back(aktruch);
				aktruch++;
			}
		}
		else if (pion < 0) {
			if (pion == -8) {
				pilka = plansza->czarna();
				kolejnosc.push_back(2);
			}
			else {
				pionek[i] = plansza->czarny(-pion - 1);
				kolejnosc.push_back(aktruch);
				aktruch++;
			}
		}

		for (int j = 0, x, y; j < 2; j++) {
			str >> x >> y;
			wsplruchu[i][j] = QPoint(x,y);
		}
	}

	if (pilka != NULL)
		podano = true;
	else
		podano = false;
}

Ruch::~Ruch()
{
}

int Ruch::zrobiono()
{
	return kolejnosc.size();
}

int Ruch::ilewSumie()
{
	return ilewogole;
}

int Ruch::ilePrzesunieto()
{
	return aktruch;
}

bool Ruch::czyPodano()
{
	return podano;
}

void Ruch::nowePrzesuniecie(Pionek *pion, QPoint pkt)
{
	int kols = kolejnosc.size();

	wsplruchu[kols][0] = pion->wspl();
	wsplruchu[kols][1] = pkt;

	pionek[kols] = pion;

	Plansza *plansza = pionek[kols]->dajPlansze();
	while (!plansza->nastepne.empty())
		delete plansza->nastepne.pop();

	kolejnosc.push_back(aktruch);
	ilewogole = kolejnosc.size();
	aktruch++;

	pion->zmianaPozycji(pkt);
}

void Ruch::nowePrzesuniecie(Pilka *pilk, QPoint pkt)
{
	int kols = kolejnosc.size();

	wsplruchu[kols][0] = pilk->wspl();
	wsplruchu[kols][1] = pkt;

	pionek[kols] = NULL;
	pilka = pilk;
	podano = true;

	Plansza *plansza = pilka->dajPlansze();
	while (!plansza->nastepne.empty())
		delete plansza->nastepne.pop();

	kolejnosc.push_back(2);
	ilewogole = kolejnosc.size();

	pilk->zmianaPozycji(pkt);
}

void Ruch::cofnij(bool jawnie)
{
	if (zrobiono() == 0)
		return;

	int cofany = kolejnosc.back();
	kolejnosc.pop_back();

	int kols = kolejnosc.size();

	if (cofany == 2) {
		pilka->zmianaPozycji(wsplruchu[kols][0], jawnie);
		podano = false;
	}
	else {
		pionek[kols]->zmianaPozycji(wsplruchu[kols][0], jawnie);
		aktruch--;
	}
}

void Ruch::cofnij()
{
	cofnij(true);
}

void Ruch::powtorz(bool jawnie)
{
	if (kolejnosc.size() == ilewogole)
		return;

	int kols = kolejnosc.size();

	if (pionek[kols] == NULL) {
		pilka->zmianaPozycji(wsplruchu[kols][1], jawnie);
		podano = true;
		kolejnosc.push_back(2);
	}
	else {
		pionek[kols]->zmianaPozycji(wsplruchu[kols][1], jawnie);
		kolejnosc.push_back(aktruch);
		aktruch++;
	}
}

void Ruch::powtorz()
{
	powtorz(true);
}

QString Ruch::zapiszRuch()
{
	QString stan = "";
	QTextStream str(&stan);

	str << ilewogole << "\n";

	for (int i = 0; i < 3; i++) {
		if (pionek[i] != NULL) {
			if (pionek[i]->gracz() == 0)
				str << pionek[i]->nr() + 1 << " ";
			else
				str << -(pionek[i]->nr() + 1) << " ";
		}
		else if (pilka != NULL && i < ilewogole) {
			if (pilka->gracz() == 0)
				str << "8 ";
			else
				str << "-8 ";
		}
		else
			str << "0 ";

		for (int j = 0; j < 2; j++)
			str << wsplruchu[i][j].x() << " " << wsplruchu[i][j].y() << " ";

		str << "\n";
	}

	return stan;
}

void Ruch::poustawiaj()
{
	pionek[0] = NULL;
	pionek[1] = NULL;
	pionek[2] = NULL;
	pilka = NULL;

	aktruch = 0;
}
