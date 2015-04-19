#include "plansza.h"

int prz[8][2] = {{1,0}, {0,1}, {-1,0}, {0,-1}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};

Plansza::Plansza(bool gracza, bool graczb, QGraphicsScene *sc, QLabel *stat)
{
	czlowiek[0] = gracza;
	czlowiek[1] = graczb;
	ktogra = 0;

	scene = sc;
	status = stat;

	poustawiaj();

	for (int i = 0; i < 7; i++) {
		biale[i] = new Pionek(i, 0, 0, i, this, scene);
		pionki[i][0] = biale[i];
		pole(i,0)->postaw(biale[i]);
	}

	for (int i = 0; i < 7; i++) {
		czarne[i] = new Pionek(i, 6, 1, i, this, scene);
		pionki[i][6] = czarne[i];
		pole(i,6)->postaw(czarne[i]);
	}

	bialap = new Pilka(3, 0, 0, this, scene);
	pionek(3,0)->ustawPilke(true);
	czarnap = new Pilka(3, 6, 1, this, scene);
	pionek(3,6)->ustawPilke(true);
}

Plansza::Plansza(QString stan, QGraphicsScene *sc, QLabel *stat)
{
	QTextStream str(&stan);

	int czl[2];
	str >> ktogra >> czl[0] >> czl[1];
	czlowiek[0] = (bool) czl[0];
	czlowiek[1] = (bool) czl[1];

	scene = sc;
	status = stat;

	poustawiaj();

	for (int i = 0, x, y; i < 7; i++) {
		str >> x >> y;
		biale[i] = new Pionek(x, y, 0, i, this, scene);
		pionki[x][y] = biale[i];
		pole(x,y)->postaw(biale[i]);
	}

	for (int i = 0, x, y; i < 7; i++) {
		str >> x >> y;
		czarne[i] = new Pionek(x, y, 1, i, this, scene);
		pionki[x][y] = czarne[i];
		pole(x,y)->postaw(czarne[i]);
	}

	int x,y;

	str >> x >> y;
	bialap = new Pilka(x, y, 0, this, scene);
	pionek(x,y)->ustawPilke(true);

	str >> x >> y;
	czarnap = new Pilka(x, y, 1, this, scene);
	pionek(x,y)->ustawPilke(true);

	if (!czyPlanszaOK())
		emit zlaPlansza();

	int n;
	str >> n;

	for (int i = 0; i < n; i++) {
		QString st = "";
		QTextStream struch(&st);

		for (int j = 0, akt; j < 16; j++) {
			str >> akt;
			struch << akt << " ";
		}

		poprzednie.push(new Ruch(st, this));
	}
}

Plansza::~Plansza()
{
	przerwij();

	for (int i = 0; i < 7; i++) {
		 if (biale[i] != NULL)
			 biale[i]->timeline->setCurrentTime(300);
		 if (czarne[i] != NULL)
			 czarne[i]->timeline->setCurrentTime(300);
	 }

	 if (bialap != NULL)
		 bialap->timeline->setCurrentTime(100);
	 if (czarnap != NULL)
		 czarnap->timeline->setCurrentTime(100);

	 qDeleteAll(scene->items());

	 delete aktualny;
	 while (!poprzednie.empty())
		 delete poprzednie.pop();

	 while (!nastepne.empty())
		 delete nastepne.pop();
}

Pole * Plansza::pole(int i, int j)
{
	return pola[i][j];
}

Pionek * Plansza::pionek(int i, int j)
{
	return pionki[i][j];
}

Pionek * Plansza::bialy(int i)
{
	return biale[i];
}

Pionek * Plansza::czarny(int i)
{
	return czarne[i];
}

Pilka * Plansza::biala()
{
	return bialap;
}

Pilka * Plansza::czarna()
{
	return czarnap;
}

void Plansza::zgasPola()
{
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++) {
			pole(i,j)->zgas();

			if (pole(i,j)->pionek() != NULL)
				pole(i,j)->pionek()->zgas();
		}
}

bool Plansza::podswietlPola(Pionek *pionek)
{
	int i = pionek->wspl().x(), j = pionek->wspl().y();

		bool cokolwiek = false;

		for (int k = 0; k < 4; k++)
			if (podswietlacz(i + prz[k][0], j + prz[k][1]))
				cokolwiek = true;

		return cokolwiek;
}

bool Plansza::podswietlPola(Pilka *pilka)
{
	QPoint ws = pilka->wspl();

	for (int k = 0; k < 8; k++)
		for (int i = ws.x() + prz[k][0], j = ws.y() + prz[k][1];
			 i >= 0 && i < 7 && j >= 0 && j < 7; i += prz[k][0], j += prz[k][1])
			if (!czyMoznaPodawac(pole(i,j), pilka))
				break;

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			if (pole(i,j)->pionek() != NULL)
				if (pole(i,j)->pionek()->swieci())
					return true;

	return false;
}

bool Plansza::czyMoznaPodawac(Pole *pole, Pilka *pilka)
{
	if (pole->pionek() != NULL) {
		if (pole->pionek()->gracz() == pilka->gracz()) {
			pole->pionek()->podswietl();
			return true;
		}
		else
			return false;
	}

	return true;
}

int Plansza::gracz()
{
	return ktogra;
}

Pionek * Plansza::ruchawy()
{
	return ruszany;
}

Pilka * Plansza::ruchawa()
{
	return ruszana;
}

void Plansza::setRuchawy(Pionek *p)
{
	if (ruszany != NULL)
		ruszany->setRuchawy(false);

	if (p != NULL) {
		p->setRuchawy(true);
		setRuchawa(NULL);
	}

	ruszany = p;
}

void Plansza::setRuchawa(Pilka *p)
{
	if (ruszana != NULL)
		ruszana->setRuchawa(false);

	if (p != NULL) {
		p->setRuchawa(true);
		setRuchawy(NULL);
	}

	ruszana = p;
}

void Plansza::przesunieto()
{
	zgasPola();
	setRuchawy(NULL);
	setRuchawa(NULL);
}

void Plansza::ustawPionek(QPoint pkt, Pionek *pion)
{
	pionki[pkt.x()][pkt.y()] = pion;
}

Ruch * Plansza::ruch()
{
	return aktualny;
}

void Plansza::nowyruch()
{
	ktogra ^= 1;
	aktualny = new Ruch();

	if (gracz() == 0)
		status->setText("Aktualny gracz: biały.");
	else
		status->setText("Aktualny gracz: czerwony.");

	if (!czlowiek[ktogra] && automat())
		ruchKompa();
}

void Plansza::zatwierdzruch(bool aut)
{
	if (ruch()->zrobiono() == 0 || (!czlowiek[ktogra] && !aut)) {
		status->setText("Nie można teraz zatwierdzić ruchu!");
		return;
	}

	przesunieto();

	wygrany = ktoWygral(ktogra);

	if (wygrany != -1)
		emit koniec();
	else {
		poprzednie.push(aktualny);
		nowyruch();
	}
}

void Plansza::zatwierdzruch()
{
	zatwierdzruch(false);
}

int Plansza::zwyciezca()
{
	return wygrany;
}

int Plansza::ktoWygral(int sprawdzany)
{
	if (sprawdzany == 0 && bialap->wspl().y() == 6)
		return sprawdzany;
	else if (sprawdzany == 1 && czarnap->wspl().y() == 0)
		return sprawdzany;

	if (czyBlokuje(sprawdzany^1) && ileStyka(sprawdzany) >= 3)
		return sprawdzany;
	else if (czyBlokuje(sprawdzany) && ileStyka(sprawdzany^1) >= 3)
		return sprawdzany^1;

	return -1;
}

bool Plansza::czyBlokuje(int sprawdzany)
{
	int dokad = 0;

	Pionek *pierwszy = NULL;
	for (int i = 0; i < 7; i++)
		if (pionek(0, i) != NULL && pionek(0, i)->gracz() == sprawdzany) {
			pierwszy = pionek(0, i);
			break;
		}

	while (pierwszy != NULL) {
		pierwszy = czyCosStoi(pierwszy);
		dokad++;
	}

	if (dokad == 7)
		return true;
	else
		return false;

}

Pionek * Plansza::czyCosStoi(Pionek *pion)
{
	if (pion->wspl().x() == 6)
		return NULL;

	int x = pion->wspl().x(), y = pion->wspl().y();

	for (int i = -1; i <= 1; i++)
		if (y+i >= 0 && y+i < 7 && pole(x+1, y+i)->pionek() != NULL
		   && pole(x+1, y+i)->pionek()->gracz() == pion->gracz())
			return pole(x+1, y+i)->pionek();

	return NULL;
}

int Plansza::ileStyka(int sprawdzany)
{
	int wynik = 0;
	int x,y;

	for (int i = 0; i < 7; i++) {
		if (sprawdzany == 0) {
			x = biale[i]->wspl().x(), y = biale[i]->wspl().y();

			if (y < 6 && pionek(x, y+1) != NULL && pionek(x, y+1)->gracz() != sprawdzany)
				wynik++;
		}
		else {
			x = czarne[i]->wspl().x(), y = czarne[i]->wspl().y();

			if (y > 0 && pionek(x, y-1) != NULL && pionek(x, y-1)->gracz() != sprawdzany)
				wynik++;
		}
	}

	return wynik;

}

void Plansza::cofnijCalyRuch()
{
	if (poprzednie.empty()) {
		status->setText("Nie można cofnąć ruchu.");
		return;
	}

	if (aktualny->ilewSumie() > 0)
		nastepne.push(aktualny);
	else
		delete aktualny;

	aktualny = poprzednie.pop();
	ktogra ^= 1;
}

void Plansza::powtorzCalyRuch()
{
	if (nastepne.empty()) {
		status->setText("Nie można powtórzyć ruchu.");
		return;
	}

	poprzednie.push(aktualny);
	aktualny = nastepne.pop();
	ktogra ^= 1;
}

void Plansza::zapalWszystko()
{
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			if (pole(i,j)->pionek() == NULL)
				pole(i,j)->podswietl();
}

void Plansza::zapalBiale()
{
	for (int i = 0; i < 7; i++)
		if (!biale[i]->pilka())
			biale[i]->podswietl();
}

void Plansza::zapalCzarne()
{
	for (int i = 0; i < 7; i++)
		if (!czarne[i]->pilka())
			czarne[i]->podswietl();
}

bool Plansza::edytor()
{
	return edytowana;
}

void Plansza::odpalEdytor()
{
	przesunieto();
	edytowana = true;
}

void Plansza::cofnijEdyt()
{
	if (popedyt.empty()) {
		status->setText("Nie można cofnąć ruchu.");
		return;
	}

	Przes prz = popedyt.pop();

	if (prz.pionek() != NULL)
		prz.pionek()->zmianaPozycji(prz.skad());
	else
		prz.pilka()->zmianaPozycji(prz.skad());

	nastedyt.push(prz);
}

void Plansza::powtorzEdyt()
{
	if (nastedyt.empty()) {
		status->setText("Nie można powtórzyć ruchu.");
		return;
	}

	Przes prz = nastedyt.pop();

	if (prz.pionek() != NULL)
		prz.pionek()->zmianaPozycji(prz.dokad());
	else
		prz.pilka()->zmianaPozycji(prz.dokad());

	popedyt.push(prz);
}

void Plansza::anulujEdycje()
{
	while (!popedyt.empty())
		cofnijEdyt();

	nastedyt.clear();
	edytowana = false;

	przesunieto();
}

void Plansza::zatwierdzEdycje(int biezacy)
{
	popedyt.clear();
	nastedyt.clear();
	edytowana = false;

	delete aktualny;
	aktualny = new Ruch();

	while (!poprzednie.empty())
		delete poprzednie.pop();

	while (!nastepne.empty())
		delete nastepne.pop();

	przesunieto();
	ktogra = biezacy;

	wygrany = ktoWygral(ktogra);

	if (wygrany != -1)
		emit koniec();
	else {
		wygrany = ktoWygral(ktogra^1);

		if (wygrany != -1)
			emit koniec();
	}
}

bool Plansza::czyPlanszaOK()
{
	int wygra = ktoWygral(0);
	int wygrb = ktoWygral(1);

	if (wygra == -1 || wygrb == -1)
		return true;
	else if (wygra != wygrb)
		return false;

	return true;
}

QString Plansza::zapiszPlansze()
{
	QString stan = "";
	QTextStream str(&stan);
	
	int wyk = ruch()->zrobiono();
	
	while (ruch()->zrobiono() > 0)
		ruch()->cofnij(false);

	str << ktogra <<  " " << (int) czlowiek[0] << " " << (int) czlowiek[1] << "\n";

	for (int i = 0; i < 7; i++)
		str << bialy(i)->wspl().x() << " " << bialy(i)->wspl().y() << "\n";

	for (int i = 0; i < 7; i++)
		str << czarny(i)->wspl().x() << " " << czarny(i)->wspl().y() << "\n";

	str << biala()->wspl().x() << " " << biala()->wspl().y() << "\n";
	str << czarna()->wspl().x() << " " << czarna()->wspl().y() << "\n";

	str <<  poprzednie.size() << "\n";

	for (int i = 0; i < poprzednie.size(); i++)
		str << poprzednie.at(i)->zapiszRuch();

	while (ruch()->zrobiono() < wyk)
		ruch()->powtorz(false);

	return stan;
}

int Plansza::wyliczPremie(int kto)
{
	if (kto == 0) {
		if (czarna()->wspl().y() == 0)
			return -1000;
		else if (biala()->wspl().y() == 6)
			return 1000;
	}
	else {
		if (biala()->wspl().y() == 6)
			return -1000;
		else if (czarna()->wspl().y() == 0)
			return 1000;
	}

	int wynik = 0, wsp;

	if (kto == 0)
		wsp = 1;
	else
		wsp = -1;

	for (int i = 0; i < 7; i++) {
			wynik += bialy(i)->wspl().y() * wsp;
			wynik -= (6 - czarny(i)->wspl().y()) * wsp;
	}

	wynik += biala()->wspl().y() * wsp;
	wynik -= (6 - czarna()->wspl().y()) * wsp;

	return wynik - 2*ileStyka(kto);
}

void Plansza::wygenerujRuch(int kto)
{
	if (czywtrakcie) {
		status->setText("Ruch jest już generowany.");
		return;
	}
	
	ktogeneruje = kto;
	najlgen.clear();
	popgen.clear();
	przerwac = false;
	czywtrakcie = true;

	status->setText("Mądry komputerek myśli... (Ctrl + A, żeby nie myślał).");

	alfa = -1001;

	dfs(ruch()->ilePrzesunieto(), ruch()->czyPodano(), kto, 0);

	if (przerwac) {
		status->setText("Komputerek już nie myśli :(");

		czywtrakcie = false;
		return;
	}

	zegar.start();
	czywtrakcie = false;

	status->setText("Mądry komputerek już wymyślił :)");
}

void Plansza::dajHinta()
{
	if (!czlowiek[ktogra]) {
		status->setText("Hinty nie są dostępne dla komputera.");
		return;
	}

	wygenerujRuch(ktogra);
}

void Plansza::ruchKompa()
{
	if (czlowiek[ktogra]) {
		status->setText("Aktualnie gra człowiek - komputer nie może wykonać ruchu.");
		return;
	}

	wygenerujRuch(ktogra);
}

void Plansza::dfs(int przes, int pod, int gracz, int poziom)
{
	if (przerwac)
		return;

	if (poziom == 0 && alfa == 1000)
		return;
	if (poziom == 1 && alfa >= beta)
		return;

	if (przes < 2) {
		for (int i = 0; i < 7; i++)
			if (!pionko(gracz, i)->pilka()) {
				int x = pionko(gracz, i)->wspl().x();
				int y = pionko(gracz, i)->wspl().y();

				for (int j = 0; j < 4; j++)
					dfsPionek(i, x, y, prz[j][0], prz[j][1], przes, pod, gracz, poziom);
			}
	}

	if (pod < 1) {
		QPoint ws = pilko(gracz)->wspl();

		for (int k = 0; k < 8; k++)
			for (int i = ws.x() + prz[k][0], j = ws.y() + prz[k][1];
				 i >= 0 && i < 7 && j >= 0 && j < 7; i += prz[k][0], j += prz[k][1])
				if (pole(i,j)->pionek() != NULL) {
					if (pole(i,j)->pionek()->gracz() == gracz)
						dfsPilka(i, j, przes, pod, gracz, poziom, ws);
					else
						break;
				}
	}

	if (przes == 0 && pod == 0)
		return;

	if (poziom == 0 && wyliczPremie(gracz) == 1000) {
		alfa = 1000;

		poprawNajlRuchy();
	}
	else if (poziom == 1 && wyliczPremie(gracz^1) == -1000)
		beta = -1000;

	if (poziom == 0) {
		emit puscEventy();

		beta = 1000;

		dfs(0, 0, gracz^1, 1);

		if (alfa < beta) {
			alfa = beta;

			poprawNajlRuchy();
		}
	}
	else if (poziom == 1) {
		int premia = wyliczPremie(gracz^1);

		if (beta > premia)
			beta = premia;
	}
}

void Plansza::dfsPionek(int i, int x, int y, int wx, int wy, int przes, int pod, int gracz, int poziom)
{
	if (qMin(x + wx, y + wy) >= 0 && qMax(x + wx, y + wy) < 7 && pole(x + wx, y + wy)->pionek() == NULL) {
		popgen.push( Przes(QPoint(x,y), pionko(gracz, i), QPoint(x + wx, y + wy)) );
		pionko(gracz, i)->zmianaPozycji(QPoint(x + wx, y + wy), false);
		dfs(przes + 1, pod, gracz, poziom);
		pionko(gracz, i)->zmianaPozycji(QPoint(x, y), false);
		popgen.pop();
	}
}

void Plansza::dfsPilka(int x, int y, int przes, int pod, int gracz, int poziom, QPoint ws)
{
	popgen.push( Przes(ws, pilko(gracz), QPoint(x,y)) );
	pilko(gracz)->zmianaPozycji(QPoint(x,y), false);
	dfs(przes, pod + 1, gracz, poziom);
	pilko(gracz)->zmianaPozycji(ws, false);
	popgen.pop();
}

void Plansza::przerwij()
{
	przerwac = true;
	zegar.stop();
}

Pionek * Plansza::pionko(int gracz, int nr)
{
	if (gracz == 0)
		return biale[nr];
	else
		return czarne[nr];
}

Pilka * Plansza::pilko(int gracz)
{
	if (gracz == 0)
		return bialap;
	else
		return czarnap;
}

bool Plansza::automat()
{
	return automatycznie;
}

void Plansza::setAutomat(bool aut)
{
	automatycznie = aut;
}

bool Plansza::czyCzlowiek(int kto)
{
	return czlowiek[kto];
}

bool Plansza::wtrakcie()
{
	return czywtrakcie;
}

void Plansza::autRuch()
{
	if (!najlgen.empty()) {
		Przes polecany = najlgen.pop();

		if (polecany.pionek() != NULL)
			ruch()->nowePrzesuniecie(polecany.pionek(), polecany.dokad());
		else
			ruch()->nowePrzesuniecie(polecany.pilka(), polecany.dokad());
	}

	if (najlgen.empty()) {
		zegar.stop();

		if (!czlowiek[ktogra])
			zatwierdzruch(true);
	}
}

QLabel * Plansza::statusik()
{
	return status;
}

void Plansza::poustawiaj()
{
	ktogeneruje = 0;
	ruszany = NULL;
	ruszana = NULL;
	wygrany = -1;
	edytowana = false;
	przerwac = false;
	automatycznie = false;
	czywtrakcie = false;

	zegar.setInterval(400);
	connect(&zegar, SIGNAL(timeout()), this, SLOT(autRuch()));

	aktualny = new Ruch();

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++) {
			pola[i][j] = new Pole(i, j, this, scene);
			pionki[i][j] = NULL;
		}
}

bool Plansza::podswietlacz(int x, int y)
{
	if (qMin(x,y) >= 0 && qMax(x,y) < 7 && pole(x,y)->pionek() == NULL) {
		pole(x,y)->podswietl();
		return true;
	}
	else
		return false;
}

void Plansza::poprawNajlRuchy()
{
	najlgen.clear();
	for (int i = popgen.size() - 1; i >= 0; i--)
		najlgen.push(popgen.at(i));
}





