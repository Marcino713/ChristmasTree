#pragma once
#ifndef snieg_h
#define snieg_h

#include <d2d1.h>
#include <time.h>
#include "dodatki.h"

const int DLUGOSC_SNIEGU = 900;
const int CZAS_MIN_SNIEG = (int)((1.0 / 30.0) * 1000.0);

struct Sniezka {
	float x;
	float y;
	float vx;
	float vy;
};

void LosujNastSniezke();
void InicjujSnieg();
void LosujPogode();
int PobierzWolnaKomorke();
void UtworzNowaSniezke();
void PrzetworzSnieg();
void LosujSniezke();
void UsunSnieg();

#endif