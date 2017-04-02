#pragma once
#ifndef main_h
#define main_h

#include "snieg.h"
#include "swiatla.h"
#include "dodatki.h"
#include "resource.h"
#include <time.h>
#include <string.h>
#include <immintrin.h>
#include <Windows.h>
#include <d2d1.h>
#include <wincodec.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Windowscodecs.lib")

const clock_t DLUGOSC_CZASU_KOLORU = 20000;
const int ILOSC_KOLOROW = 3;

const int DLUGOSC_CZASU_SWIATEL = 5000;

const int CHOINKA_SZER = 850;
const int CHOINKA_WYS = 800;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
void Rysuj();
void Zakoncz();
void ZmienRozmiar();
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void UtworzChoinke();
#endif