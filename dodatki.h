#pragma once
#ifndef dodatki_h
#define dodatki_h

#include <d2d1.h>
#include <wincodec.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Windowscodecs.lib")

template<class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
	) {
	if (*ppInterfaceToRelease != NULL) {
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

const int GWIAZDKA_SZER = 9;
const int GWIAZDKA_WYS = 9;

void RysujObraz(ID2D1BitmapBrush* pedzel, int x, int y, int szer, int wys);
ID2D1BitmapBrush* UtworzPedzelPng(int Zasob);

#endif