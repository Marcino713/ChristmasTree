#include "dodatki.h"

extern ID2D1HwndRenderTarget* OknoD2D;
extern IWICImagingFactory* WicFactory;

void RysujObraz(ID2D1BitmapBrush* pedzel, int x, int y, int szer, int wys) {
	D2D1_RECT_F rect;
	D2D1_MATRIX_3X2_F t;
	int sz = szer / 2.0;
	int w = wys / 2.0;

	rect.left = x - sz;
	rect.top = y - w;
	rect.right = x + sz;
	rect.bottom = y + w;

	if (szer % 2 == 1) rect.right++;
	if (wys  % 2 == 1) rect.bottom++;

	pedzel->GetTransform(&t);
	pedzel->SetTransform(D2D1::Matrix3x2F::Translation(D2D1::SizeF(rect.left, rect.top)));

	OknoD2D->FillRectangle(&rect, pedzel);

	pedzel->SetTransform(t);
}

ID2D1BitmapBrush* UtworzPedzelPng(int Zasob) {
	ID2D1Bitmap *obraz_d2d = NULL;
	IWICBitmapDecoder *pDecoder = NULL;
	IWICBitmapFrameDecode *pSource = NULL;
	IWICStream *pStream = NULL;
	IWICFormatConverter *pConverter = NULL;
	ID2D1BitmapBrush* p = NULL;
	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void *pImageFile = NULL;
	DWORD imageFileSize = NULL;

	imageResHandle = FindResource(HINST_THISCOMPONENT, MAKEINTRESOURCE(Zasob), RT_RCDATA);
	imageResDataHandle = LoadResource(HINST_THISCOMPONENT, imageResHandle);
	pImageFile = LockResource(imageResDataHandle);
	imageFileSize = SizeofResource(HINST_THISCOMPONENT, imageResHandle);

	WicFactory->CreateStream(&pStream);
	pStream->InitializeFromMemory(reinterpret_cast<BYTE*>(pImageFile), imageFileSize);
	WicFactory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnLoad, &pDecoder);
	pDecoder->GetFrame(0, &pSource);

	WicFactory->CreateFormatConverter(&pConverter);
	pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);

	OknoD2D->CreateBitmapFromWicBitmap(pConverter, NULL, &obraz_d2d);

	OknoD2D->CreateBitmapBrush(obraz_d2d, &p);
	p->SetExtendModeX(D2D1_EXTEND_MODE_WRAP);
	p->SetExtendModeY(D2D1_EXTEND_MODE_WRAP);

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&obraz_d2d);

	return p;
}