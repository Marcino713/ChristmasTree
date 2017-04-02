#include "main.h"

int EkranX = 0;
int EkranY = 0;
int KlientX = 0;
int KlientY = 0;

int CzasKoloru = 0;
int IdKoloru = 1;

int CzasSwiatel = 0;

LPCWSTR TYTUL_OKNA = L"Choinka";
LPCWSTR KLASA_OKNA = L"wndChoinka";
HWND hOkno = 0;
ID2D1Factory* D2DFactory = NULL;
ID2D1HwndRenderTarget* OknoD2D = NULL;
ID2D1BitmapBrush* Pedzel = NULL;
ID2D1SolidColorBrush* PedzelChoinka = NULL;
IWICImagingFactory* WicFactory = 0;
ID2D1PathGeometry* Choinka = NULL;
D2D1::Matrix3x2F MacierzChoinka = D2D1::Matrix3x2F::Identity();

__m128 Kolory[ILOSC_KOLOROW];
__m128* Kolor1 = 0;
__m128* Kolor2 = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	EkranX = GetSystemMetrics(SM_CXSCREEN);
	EkranY = GetSystemMetrics(SM_CYSCREEN);

	srand(time(NULL));

	CoInitialize(NULL);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_ID2D1Factory, 0, (void**)&D2DFactory);
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&WicFactory);

	WNDCLASS wc = {};
	wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = KLASA_OKNA;
	
	RegisterClass(&wc);

	hOkno = CreateWindow(KLASA_OKNA, TYTUL_OKNA, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, NULL);

	RECT rc;
	GetClientRect(hOkno, &rc);
	KlientX = rc.right - rc.left;
	KlientY = rc.bottom - rc.top;

	D2D1_SIZE_U size = D2D1::SizeU(KlientX, KlientY);
	
	D2DFactory->CreateHwndRenderTarget(&D2D1::RenderTargetProperties() , &D2D1::HwndRenderTargetProperties(hOkno, size), &OknoD2D);
	ZmienRozmiar();

	Pedzel = UtworzPedzelPng(IDB_SNIEZKA);

	Kolory[0] = { 1.0f, 0.28627f, 0.22745f, 0.98039f };
	Kolory[1] = { 1.0f, 0.51765f, 0.51373f, 0.58824f };
	Kolory[2] = { 1.0f, 0.13725f, 0.72941f, 0.8549f };

	OknoD2D->CreateSolidColorBrush(D2D1::ColorF::ColorF(D2D1::ColorF::MediumSeaGreen), &PedzelChoinka);

	InicjujSnieg();
	InicjujSwiatla();
	UtworzChoinke();

	Kolor1 = &Kolory[0];
	Kolor2 = &Kolory[1];

	ShowWindow(hOkno, SW_SHOWNORMAL);
	UpdateWindow(hOkno);

	CzasKoloru = clock() + DLUGOSC_CZASU_KOLORU;
	CzasSwiatel = clock() + DLUGOSC_CZASU_SWIATEL;

	MSG msg;

	while (true) {

		Rysuj();

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

			if (msg.message == WM_QUIT) {
				Zakoncz();
				return 0;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}

}

void Rysuj() {

	time_t czas = clock();

	if (czas >= CzasKoloru) {
		if (++IdKoloru >= ILOSC_KOLOROW) {
			IdKoloru = 0;
		}

		Kolor1 = Kolor2;
		Kolor2 = &Kolory[IdKoloru];

		CzasKoloru += DLUGOSC_CZASU_KOLORU;
		
	}

	if (czas >= CzasSwiatel) {
		CzasSwiatel += DLUGOSC_CZASU_SWIATEL;
		PrzetworzSwiatla();
	}

	LosujSniezke();


	float wsp = ((float)(CzasKoloru-czas) / (float)DLUGOSC_CZASU_KOLORU);
	__m128 k1 = _mm_mul_ps(*Kolor1, _mm_broadcast_ss(&wsp));
	wsp = 1.0 - wsp;
	__m128 k2 = _mm_mul_ps(*Kolor2, _mm_broadcast_ss(&wsp));
	__m128 kol = _mm_add_ps(k1, k2);

	OknoD2D->BeginDraw();

	OknoD2D->Clear(D2D1::ColorF(D2D1::ColorF(kol.m128_f32[1], kol.m128_f32[2], kol.m128_f32[3])));
	OknoD2D->SetTransform(MacierzChoinka);
	OknoD2D->FillGeometry(Choinka, PedzelChoinka);
	WyswietlSwiatla();

	OknoD2D->SetTransform(D2D1::Matrix3x2F::Identity());
	PrzetworzSnieg();

	OknoD2D->EndDraw();
}

void Zakoncz() {
	UsunSnieg();
	CzyscSwiatla();
	SafeRelease(&Pedzel);
	SafeRelease(&PedzelChoinka);
	SafeRelease(&Choinka);
	SafeRelease(&WicFactory);
	SafeRelease(&OknoD2D);
	SafeRelease(&D2DFactory);
	CoUninitialize();
}

void ZmienRozmiar(){
	if (!KlientX || !KlientY) {
		tagRECT klient = {};
		GetClientRect(hOkno, &klient);
		KlientX = klient.right - klient.left;
		KlientY = klient.bottom - klient.top;
	}

	if (OknoD2D) {
		OknoD2D->Resize(D2D1::SizeU(KlientX, KlientY));
	}

	float wsp = 1.0;

	if (CHOINKA_SZER > KlientX || CHOINKA_WYS > KlientY) {
		wsp = (float)KlientX / (float)CHOINKA_SZER;

		if ((int)(wsp * (float)CHOINKA_WYS) > KlientY) {
			wsp = (float)KlientY / (float)CHOINKA_WYS;
		}

	}

	float x = ((float)KlientX - wsp*CHOINKA_SZER)/2.0;
	float y = ((float)KlientY - wsp*CHOINKA_WYS);

	MacierzChoinka = D2D1::Matrix3x2F::Translation(0, 65) * D2D1::Matrix3x2F::Scale(wsp, wsp) * D2D1::Matrix3x2F::Translation(x, y);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
		case WM_SIZE: {
			KlientX = LOWORD(lParam);
			KlientY = HIWORD(lParam);
			ZmienRozmiar();
			return 0;
		}

		case WM_DISPLAYCHANGE:
			InvalidateRect(hwnd, NULL, FALSE);
			return 0;

		case WM_PAINT:
			Rysuj();
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}

}


void UtworzChoinke() {
	using D2D1::Point2F;

	ID2D1GeometrySink* Sink = NULL;
	D2D1_BEZIER_SEGMENT b;

	D2DFactory->CreatePathGeometry(&Choinka);
	Choinka->Open(&Sink);

	Sink->BeginFigure(Point2F(420, 732), D2D1_FIGURE_BEGIN_FILLED);	

	//Lewa - galezie od dolu:
	//1
	b.point1 = Point2F(341, 732);
	b.point2 = Point2F(92, 722);
	b.point3 = Point2F(2, 679);
	Sink->AddBezier(b);

	b.point1 = Point2F(175, 689);
	b.point2 = Point2F(303, 621);
	b.point3 = Point2F(329, 577);
	Sink->AddBezier(b);

	//2
	b.point1 = Point2F(269, 578);
	b.point2 = Point2F(123, 601);
	b.point3 = Point2F(69, 563);
	Sink->AddBezier(b);

	b.point1 = Point2F(186, 549);
	b.point2 = Point2F(273, 506);
	b.point3 = Point2F(313, 462);
	Sink->AddBezier(b);

	//3
	b.point1 = Point2F(236, 474);
	b.point2 = Point2F(132, 446);
	b.point3 = Point2F(113, 424);
	Sink->AddBezier(b);

	b.point1 = Point2F(237, 432);
	b.point2 = Point2F(339, 389);
	b.point3 = Point2F(357, 347);
	Sink->AddBezier(b);

	//4
	b.point1 = Point2F(323, 346);
	b.point2 = Point2F(230, 336);
	b.point3 = Point2F(184, 320);
	Sink->AddBezier(b);

	b.point1 = Point2F(261, 315);
	b.point2 = Point2F(316, 309);
	b.point3 = Point2F(384, 254);
	Sink->AddBezier(b);

	//5
	b.point1 = Point2F(350, 252);
	b.point2 = Point2F(261, 241);
	b.point3 = Point2F(238, 223);
	Sink->AddBezier(b);

	b.point1 = Point2F(276, 226);
	b.point2 = Point2F(365, 216);
	b.point3 = Point2F(400, 166);
	Sink->AddBezier(b);

	//6
	b.point1 = Point2F(363, 169);
	b.point2 = Point2F(347, 166);
	b.point3 = Point2F(296, 148);
	Sink->AddBezier(b);

	b.point1 = Point2F(322, 154);
	b.point2 = Point2F(414, 101);
	b.point3 = Point2F(430, 19);
	Sink->AddBezier(b);


	//Prawa strona
	//6
	b.point1 = Point2F(436, 101);
	b.point2 = Point2F(529, 155);
	b.point3 = Point2F(555, 151);
	Sink->AddBezier(b);

	b.point1 = Point2F(502, 167);
	b.point2 = Point2F(488, 168);
	b.point3 = Point2F(450, 164);
	Sink->AddBezier(b);

	//5
	b.point1 = Point2F(483, 216);
	b.point2 = Point2F(577, 223);
	b.point3 = Point2F(615, 227);
	Sink->AddBezier(b);

	b.point1 = Point2F(594, 243);
	b.point2 = Point2F(495, 255);
	b.point3 = Point2F(460, 255);
	Sink->AddBezier(b);

	//4
	b.point1 = Point2F(536, 309);
	b.point2 = Point2F(585, 311);
	b.point3 = Point2F(661, 316);
	Sink->AddBezier(b);

	b.point1 = Point2F(623, 342);
	b.point2 = Point2F(524, 347);
	b.point3 = Point2F(489, 345);
	Sink->AddBezier(b);

	//3
	b.point1 = Point2F(498, 388);
	b.point2 = Point2F(602, 436);
	b.point3 = Point2F(737, 444);
	Sink->AddBezier(b);

	b.point1 = Point2F(698, 453);
	b.point2 = Point2F(603, 471);
	b.point3 = Point2F(533, 461);
	Sink->AddBezier(b);

	//2
	b.point1 = Point2F(575, 502);
	b.point2 = Point2F(663, 550);
	b.point3 = Point2F(781, 565);
	Sink->AddBezier(b);

	b.point1 = Point2F(749, 607);
	b.point2 = Point2F(623, 588);
	b.point3 = Point2F(528, 579);
	Sink->AddBezier(b);

	//1
	b.point1 = Point2F(569, 604);
	b.point2 = Point2F(671, 682);
	b.point3 = Point2F(847, 678);
	Sink->AddBezier(b);

	b.point1 = Point2F(763, 726);
	b.point2 = Point2F(509, 731);
	b.point3 = Point2F(422, 732);
	Sink->AddBezier(b);


	Sink->EndFigure(D2D1_FIGURE_END::D2D1_FIGURE_END_CLOSED);
	Sink->Close();
	SafeRelease(&Sink);
}