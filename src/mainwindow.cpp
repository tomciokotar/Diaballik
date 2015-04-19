#include "mainwindow.h"

MainWindow::MainWindow(QApplication *a)
	: QMainWindow(0)
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	setMinimumSize(360, 407);


	gview = new Grafika(this);
	setCentralWidget(gview);

	scene = new QGraphicsScene(this);
	gview->setScene(scene);

	nazwapliku = "";
	poinformowano = false;
	app = a;

	nowagra = menuBar()->addMenu("Nowa gra");
	rozgrywka = menuBar()->addMenu("Rozgrywka");
	opcje = menuBar()->addMenu("Opcje");
	edytor = menuBar()->addMenu("Edytor");
	pomoc = menuBar()->addMenu("Pomoc");

	czlczl = new QAction("Człowiek-Człowiek", this);
	connect(czlczl, SIGNAL(triggered()), this, SLOT(generujCzlCzl()));
	nowagra->addAction(czlczl);

	czlkomp = new QAction("Człowiek-Komputer", this);
	connect(czlkomp, SIGNAL(triggered()), this, SLOT(generujCzlKomp()));
	nowagra->addAction(czlkomp);
	
	kompczl = new QAction("Komputer-Człowiek", this);
	connect(kompczl, SIGNAL(triggered()), this, SLOT(generujKompCzl()));
	nowagra->addAction(kompczl);

	kompkomp = new QAction("Komputer-Komputer", this);
	connect(kompkomp, SIGNAL(triggered()), this, SLOT(generujKompKomp()));
	nowagra->addAction(kompkomp);

	zapisz = new QAction("Zapisz", this);
	zapisz->setShortcut(Qt::CTRL + Qt::Key_S);
	connect(zapisz, SIGNAL(triggered()), this, SLOT(zapiszPlansze()));
	nowagra->addAction(zapisz);

	zapiszjako = new QAction("Zapisz jako...", this);
	zapiszjako->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);
	connect(zapiszjako, SIGNAL(triggered()), this, SLOT(zapiszJako()));
	nowagra->addAction(zapiszjako);

	wczytaj = new QAction("Wczytaj", this);
	connect(wczytaj, SIGNAL(triggered()), this, SLOT(wczytajPlansze()));
	nowagra->addAction(wczytaj);

	zakoncz = new QAction("Zakończ", this);
	connect(zakoncz, SIGNAL(triggered()), this, SLOT(close()));
	nowagra->addAction(zakoncz);


	zatwierdzruch = new QAction("Zatwierdź ruch", this);
	zatwierdzruch->setShortcut(Qt::CTRL + Qt::Key_F);
	connect(zatwierdzruch, SIGNAL(triggered()), this, SLOT(zatwierdz()));
	rozgrywka->addAction(zatwierdzruch);

	cofnij = new QAction("Cofnij ruch", this);
	cofnij->setShortcut(Qt::CTRL + Qt::Key_Z);
	connect(cofnij, SIGNAL(triggered()), this, SLOT(cofnijruch()));
	rozgrywka->addAction(cofnij);

	powtorz = new QAction("Powtórz ruch", this);
	powtorz->setShortcut(Qt::CTRL + Qt::Key_Y);
	connect(powtorz, SIGNAL(triggered()), this, SLOT(powtorzruch()));
	rozgrywka->addAction(powtorz);


	ruchkompa = new QAction("Wykonaj ruch komputera", this);
	connect(ruchkompa, SIGNAL(triggered()), this, SLOT(poruszKompem()));
	opcje->addAction(ruchkompa);

	dajhinta = new QAction("Daj podpowiedź", this);
	dajhinta->setShortcut(Qt::CTRL + Qt::Key_H);
	connect(dajhinta, SIGNAL(triggered()), this, SLOT(dajHinta()));
	opcje->addAction(dajhinta);

	edytuj = new QAction("Edytuj grę", this);
	connect(edytuj, SIGNAL(triggered()), this, SLOT(odpalEdytor()));
	opcje->addAction(edytuj);

	automat = new QAction("Komputer gra automatycznie", this);
	automat->setCheckable(true);
	connect(automat, SIGNAL(triggered()), this, SLOT(ustawAutomat()));
	opcje->addAction(automat);

	anulujgen = new QAction("Anuluj generowanie ruchu", this);
	anulujgen->setShortcut(Qt::CTRL + Qt::Key_A);
	connect(anulujgen, SIGNAL(triggered()), this, SLOT(przerwij()));
	opcje->addAction(anulujgen);


	zatwierdzedytb = new QAction("Zatwierdź - gra biały", this);
	connect(zatwierdzedytb, SIGNAL(triggered()), this, SLOT(zatwierdzEdycjeB()));
	edytor->addAction(zatwierdzedytb);

	zatwierdzedytc = new QAction("Zatwierdź - gra czerwony", this);
	connect(zatwierdzedytc, SIGNAL(triggered()), this, SLOT(zatwierdzEdycjeC()));
	edytor->addAction(zatwierdzedytc);

	anulujedyt = new QAction("Anuluj", this);
	connect(anulujedyt, SIGNAL(triggered()), this, SLOT(anulujEdycje()));
	edytor->addAction(anulujedyt);


	info = new QAction("Informacje o grze", this);
	info->setShortcut(Qt::Key_F1);
	connect(info, SIGNAL(triggered()), this, SLOT(informacje()));
	pomoc->addAction(info);


	status = new QLabel(this);
	statusBar()->addWidget(status);


	plansza = NULL;
	generujPlansze(true, true);
}

MainWindow::~MainWindow()
{
	delete plansza;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
	if (e->oldSize().width() != -1)
		gview->fitInView(scene->sceneRect());
}

void MainWindow::closeEvent(QCloseEvent *)
{
	przerwij();
}

void MainWindow::informacje()
{
	QMessageBox::information(this, "Pomoc", "Twoim celem w grze jest doprowadzenie piłki na linię przeciwnika. Powodzenia!");
}

void MainWindow::generujPlansze(bool gracza, bool graczb)
{
	if (plansza != NULL) {
		if (plansza->wtrakcie()) {
			status->setText("Nie można utworzyć nowej planszy w trakcie generowania ruchu.");
			return;
		}

		przerwij();

		delete plansza;
	}

	plansza = new Plansza(gracza, graczb, scene, status);
	sconnectuj();
}

void MainWindow::koniecGry()
{
	QString wygrany;

	if (plansza->zwyciezca() == 0)
		wygrany = "białego";
	else
		wygrany = "czerwonego";

	QMessageBox::information(this, "Koniec gry!", "Koniec gry! Gratulacje dla " + wygrany + " gracza!");
	generujPlansze(true, true);
}

void MainWindow::zatwierdz()
{
	plansza->zatwierdzruch();
}

void MainWindow::cofnijruch()
{
	przerwij();

	if (plansza->edytor())
		plansza->cofnijEdyt();
	else if (plansza->ruch()->zrobiono() > 0)
		plansza->ruch()->cofnij();
	else {
		plansza->cofnijCalyRuch();
		plansza->ruch()->cofnij();
	}
}

void MainWindow::powtorzruch()
{
	przerwij();

	if (plansza->edytor())
		plansza->powtorzEdyt();
	else if (plansza->ruch()->zrobiono() < plansza->ruch()->ilewSumie())
		plansza->ruch()->powtorz();
	else {
		plansza->powtorzCalyRuch();
		plansza->ruch()->powtorz();
	}
}

void MainWindow::odpalEdytor()
{
	przerwij();
	plansza->odpalEdytor();
	trybEdycji();
}

void MainWindow::anulujEdycje()
{
	plansza->anulujEdycje();
	trybGry();
}

void MainWindow::zatwierdzEdycje(int biezacy)
{
		if (!plansza->czyPlanszaOK()) {
		QMessageBox::information(this, "Niepoprawna plansza",
		 "Nie można zatwierdzić edycji - wszyscy gracze obecnie wygrywają grę. Anuluj edycję, bądź ją popraw.");
		}
		else {
		plansza->zatwierdzEdycje(biezacy);
				trybGry();
		}
}

void MainWindow::zatwierdzEdycjeB()
{
	zatwierdzEdycje(0);
}

void MainWindow::zatwierdzEdycjeC()
{
	zatwierdzEdycje(1);
}

void MainWindow::zapiszPlansze()
{
	przerwij();

	if (nazwapliku != "") {
		QTextDocumentWriter writer(nazwapliku);
				QTextDocument doc(plansza->zapiszPlansze(), this);

				bool ok = writer.write(&doc);

				if (ok) {
			if (!poinformowano && plansza->ruch()->zrobiono() > 0) {
				QMessageBox::information(this, "Zapisano", "Zapisano planszę. Uwaga: niezatwierdzone ruchy nie zostały zapisane.");
				poinformowano = true;

				status->setText("Plansza zapisana!");
			}
		}
		else
			QMessageBox::information(this, "Błąd", "Nie można zapisać stanu planszy do pliku.");
	}
	else
		zapiszJako();
}

void MainWindow::wczytajPlansze()
{
	QString nazwa = QFileDialog::getOpenFileName(this, "Wczytaj planszę...",
							QString(), "Pliki .txt (*.txt);;Wszystkie pliki (*)");

	QFile plik(nazwa);

	if (plik.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream str(&plik);
		QString stan = str.readAll();

		if (!parsuj(stan)) {
			QMessageBox::information(this, "Błąd", "Plik z planszą jest niepoprawny.");

			plik.close();
			return;
		}

		if (plansza != NULL) {
			if (plansza->wtrakcie()) {
				status->setText("Nie można utworzyć nowej planszy w trakcie generowania ruchu.");
				return;
			}

			przerwij();

			delete plansza;
		}

		plansza = new Plansza(stan, scene, status);
		sconnectuj();

		plik.close();
	}
	else
		QMessageBox::information(this, "Błąd", "Nie można wczytać planszy.");

}

bool MainWindow::parsuj(QString stan)
{
	QTextStream str(&stan);

	for (int i = 0, a; i < 3; i++) {
		str >> a;

		if (a != 0 && a != 1)
			return false;
	}

	int pola[7][7];

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 7; j++)
			pola[i][j] = 0;

	for (int i = 0, x, y; i < 14; i++) {
		str >> x >> y;

		if (qMin(x,y) < 0 || qMax(x,y) >= 7)
			return false;

		if (pola[x][y] != 0)
			return false;
		else
			pola[x][y] = i + 1;
	}

	int x,y;

	str >> x >> y;
	if (pola[x][y] < 1 || pola[x][y] > 7)
		return false;

	str >> x >> y;
	if (pola[x][y] < 8 || pola[x][y] > 14)
		return false;

	int n;
	str >> n;

	for (int i = 0, ile; i < n; i++) {
		str >> ile;

		if (ile < 1 || ile > 3)
			return false;

		for (int j = 0, pion, pocz[2], kon[2]; j < 3; j++) {
			str >> pion >> pocz[0] >> kon[0] >> pocz[1] >> kon[1];

			if (pion < -8 || pion > 8)
				return false;

			if (pion == 0)
				if (pocz[0] != 0 || pocz[1] != 0 || kon[0] != 0 || kon[1] != 0)
					return false;

			for (int k = 0; k < 2; k++)
				if (pocz[k] < 0 || pocz[k] >= 7 || kon[k] < 0 || kon[k] >= 7)
					return false;

			if (pion != 0 && pocz[0] == pocz[1] && kon[0] == kon[1])
				return false;
		}
	}

	return true;
}

void MainWindow::zapiszJako()
{
	przerwij();

	nazwapliku = QFileDialog::getSaveFileName(this, "Zapisz planszę jako...",
						 QString(), "Pliki .txt (*.txt);;Wszystkie pliki (*)");

	if (nazwapliku == "")
		QMessageBox::information(this, "Błąd", "Nie można zapisać stanu planszy do pliku bez nazwy.");
	else
		zapiszPlansze();
}

void MainWindow::zlaPlansza()
{
	QMessageBox::information(this, "Błąd",
		 "Wygenerowano nieprawidłową planszę (każdy gracz wygrywa). Zostanie stworzona nowa, standardowa plansza.");

	generujPlansze(true, true);
}

void MainWindow::dajHinta()
{
	plansza->dajHinta();
}

void MainWindow::puscEventy()
{
	app->processEvents();
}

void MainWindow::przerwij()
{
	plansza->setAutomat(false);
	automat->setChecked(false);

	plansza->przerwij();
}

void MainWindow::generujCzlCzl()
{
	generujPlansze(true, true);
}

void MainWindow::generujCzlKomp()
{
	generujPlansze(true, false);
}

void MainWindow::generujKompCzl()
{
		generujPlansze(false, true);
}

void MainWindow::generujKompKomp()
{
	generujPlansze(false, false);
}

void MainWindow::poruszKompem()
{
	if (plansza->automat()) {
		status->setText("Komputer gra teraz automatycznie, nie można ręcznie wygenerować jego ruchu.");
		return;
	}

	plansza->ruchKompa();
}

void MainWindow::ustawAutomat()
{
	if (plansza->automat())
		przerwij();
	else {
		plansza->setAutomat(true);
		automat->setChecked(true);

		plansza->ruchKompa();
	}
}

void MainWindow::sconnectuj()
{
	connect(plansza, SIGNAL(koniec()), this, SLOT(koniecGry()));
	connect(plansza, SIGNAL(zlaPlansza()), this, SLOT(zlaPlansza()));
	connect(plansza, SIGNAL(puscEventy()), this, SLOT(puscEventy()));

	trybGry();
}

void MainWindow::trybGry()
{
	zapisz->setEnabled(true);
	zapiszjako->setEnabled(true);
	zatwierdzruch->setEnabled(true);
	ruchkompa->setEnabled(true);
	dajhinta->setEnabled(true);
	edytuj->setEnabled(true);
	automat->setEnabled(true);
	anulujgen->setEnabled(true);
	zatwierdzedytb->setDisabled(true);
	zatwierdzedytc->setDisabled(true);
	anulujedyt->setDisabled(true);

	if (plansza->gracz() == 0)
		status->setText("Aktualny gracz: biały.");
	else
		status->setText("Aktualny gracz: czerwony.");
}

void MainWindow::trybEdycji()
{
	zapisz->setDisabled(true);
	zapiszjako->setDisabled(true);
	zatwierdzruch->setDisabled(true);
	ruchkompa->setDisabled(true);
	dajhinta->setDisabled(true);
	edytuj->setDisabled(true);
	automat->setDisabled(true);
	anulujgen->setDisabled(true);
	zatwierdzedytb->setEnabled(true);
	zatwierdzedytc->setEnabled(true);
	anulujedyt->setEnabled(true);

	status->setText("Tryb edycji.");
}
