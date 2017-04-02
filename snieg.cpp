#include "snieg.h"

//Zarzadzanie
Sniezka* Snieg[DLUGOSC_SNIEGU] = { 0 };
extern ID2D1BitmapBrush* Pedzel;

//Wspolrzedne
extern int EkranX;
extern int EkranY;
int PozycjaXMin = 0;
int PozycjaXMax = 0;
int PozycjaXDodaj = 0;
float Kierunek = 1.0;

//Czas losowania
clock_t nastepna_sniezka = 0;
int CzasNastSniezkiMin = 0;
int CzasNastSniezkiDodaj = 0;

//Predkosc
float PredkoscXMin = 0.0;
float PredkoscXDodaj = 0.0;
float PredkoscYMin = 0.0;
float PredkoscYDodaj = 0.0;


void LosujNastSniezke() {
	int czas = (rand() % CzasNastSniezkiDodaj) + CzasNastSniezkiMin;
	nastepna_sniezka = clock() + czas * CZAS_MIN_SNIEG;
}

void InicjujSnieg() {
	LosujPogode();
	UtworzNowaSniezke();
	LosujNastSniezke();
}

void LosujPogode() {
	int ilosc = rand() % 100;

	//Wiatr
	if (ilosc < 20) {	//Malo

		PredkoscXMin = -0.15;
		PredkoscXDodaj = 0.3;
		PredkoscYMin = 0.1;
		PredkoscYDodaj = 0.2;

	} else if (ilosc < 70) {	//Srednio

		Kierunek = ((rand() % 100) > 50) ? 1.0 : -1.0;
		PredkoscXMin = 0.0;
		PredkoscXDodaj = 0.9;
		PredkoscYMin = 0.3;
		PredkoscYDodaj = 0.4;

	} else {	//Duzo

		Kierunek = ((rand() % 100) > 50) ? 1.0 : -1.0;
		PredkoscXMin = 0.0;
		PredkoscXDodaj = 5.0;
		PredkoscYMin = 0.7;
		PredkoscYDodaj = 1.3;

	}

	//Zakres losowanych pozycji
	PozycjaXMin = 0;
	PozycjaXMax = EkranX;

	float przesunx = (float)EkranY / (PredkoscYMin);
	przesunx *= (PredkoscXMin + PredkoscXDodaj);

	if (Kierunek < 0.0) {	//W lewo
		PozycjaXMax += (int)przesunx;
	} else {	//W prawo
		PozycjaXMin -= (int)przesunx;
	}

	PozycjaXDodaj = PozycjaXMax - PozycjaXMin;

	//Snieg
	ilosc = rand() % 100;

	if (ilosc < 5) {	//Malo
		CzasNastSniezkiMin = 60;
		CzasNastSniezkiDodaj = 20;
	} else if (ilosc < 45) {	//Srednio
		CzasNastSniezkiMin = 15;
		CzasNastSniezkiDodaj = 35;
	} else {	//Duzo
		CzasNastSniezkiMin = 0;
		CzasNastSniezkiDodaj = 15;
	}
}

int PobierzWolnaKomorke() {
	for (int i = 0; i < DLUGOSC_SNIEGU; i++) {
		if (Snieg[i] == 0) return i;
	}

	return DLUGOSC_SNIEGU-1;
}

void UtworzNowaSniezke() {
	Sniezka* sn = new Sniezka;
	
	sn->x = (rand() % PozycjaXDodaj) + PozycjaXMin;
	sn->y = -9;
	sn->vx = ((((float)(rand() % 1000)) / 1000.0) * PredkoscXDodaj + PredkoscXMin) * Kierunek;
	sn->vy = (((float)(rand() % 1000)) / 1000.0) * PredkoscYDodaj + PredkoscYMin;
	Snieg[PobierzWolnaKomorke()] = sn;
}

void PrzetworzSnieg() {
	Sniezka* sn;

	for (int i = 0; i < DLUGOSC_SNIEGU; i++) {
		if (Snieg[i] == 0) continue;

		sn = Snieg[i];
		sn->x += sn->vx;
		sn->y += sn->vy;

		if (sn->x < (PozycjaXMin-10) || sn->x > (PozycjaXMax+10) || sn->y > EkranY) {
			delete Snieg[i];
			Snieg[i] = 0;
		} else {
			RysujObraz(Pedzel, sn->x, sn->y, 9, 9);
		}
	}
}

void LosujSniezke() {
	if (clock() >= nastepna_sniezka) {
		UtworzNowaSniezke();
		LosujNastSniezke();
	}
}

void UsunSnieg() {
	for (int i = 0; i < DLUGOSC_SNIEGU; i++) {
		if (Snieg[i]) delete Snieg[i];
	}
}