#pragma once
#ifndef swiatla_h
#define swiatla_h

#include <d2d1.h>
#include "dodatki.h"
#include "resource.h"

const int ILOSC_SWIATEL = 71;
const int ILOSC_PEDZLI_SWIATLA = 10;
const float ZMIEN_PRZEZROCZYSTOSC = 1.0 / 180.0;
const int PRAWD_ZASWIEC = 10;
const int PRAWD_WYLACZ = 50;

enum StanSwiatla {
Wylaczone, Zaswiecane, Zaswiecone, Swiecace, Gasnace, Zgaszone
};

struct Swiatlo {
	int x;
	int y;
	StanSwiatla Stan;
	ID2D1BitmapBrush* Pedzel;
	float Przezroczystosc;
};

inline Swiatlo UtwSwiatlo(int x, int y) { return { x, y, Wylaczone, NULL, 0.0f }; }
void InicjujSwiatla();
void WyswietlSwiatla();
void PrzetworzSwiatla();
void CzyscSwiatla();

#endif