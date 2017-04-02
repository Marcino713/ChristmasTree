#include "swiatla.h"

Swiatlo Swiatla[ILOSC_SWIATEL];
ID2D1BitmapBrush* PedzleSwiatla[ILOSC_PEDZLI_SWIATLA] = { 0 };

extern D2D1::Matrix3x2F MacierzChoinka;
extern ID2D1HwndRenderTarget* OknoD2D;

void InicjujSwiatla() {
	PedzleSwiatla[0] = UtworzPedzelPng(IDB_CZERWONY);
	PedzleSwiatla[1] = UtworzPedzelPng(IDB_FIOLETOWY);
	PedzleSwiatla[2] = UtworzPedzelPng(IDB_GRANATOWY);
	PedzleSwiatla[3] = UtworzPedzelPng(IDB_NIEBIESKI);
	PedzleSwiatla[4] = UtworzPedzelPng(IDB_POMARANCZOWY);
	PedzleSwiatla[5] = UtworzPedzelPng(IDB_ROZOWY);
	PedzleSwiatla[6] = UtworzPedzelPng(IDB_ZIELONY);
	PedzleSwiatla[7] = UtworzPedzelPng(IDB_ZOLTOPOMARANCZOWY);
	PedzleSwiatla[8] = UtworzPedzelPng(IDB_ZOLTOZIELONY);
	PedzleSwiatla[9] = UtworzPedzelPng(IDB_ZOLTY);

	Swiatla[0]  = UtwSwiatlo(318, 397);
	Swiatla[1]  = UtwSwiatlo(601, 432);
	Swiatla[2]  = UtwSwiatlo(725, 441);
	Swiatla[3]  = UtwSwiatlo(229, 526);
	Swiatla[4]  = UtwSwiatlo(688, 569);
	Swiatla[5]  = UtwSwiatlo(420, 71);
	Swiatla[6]  = UtwSwiatlo(405, 122);
	Swiatla[7]  = UtwSwiatlo(469, 125);
	Swiatla[8]  = UtwSwiatlo(347, 147);
	Swiatla[9]  = UtwSwiatlo(424, 180);
	Swiatla[10] = UtwSwiatlo(306, 229);
	Swiatla[11] = UtwSwiatlo(374, 216);
	Swiatla[12] = UtwSwiatlo(486, 213);
	Swiatla[13] = UtwSwiatlo(546, 230);
	Swiatla[14] = UtwSwiatlo(437, 254);
	Swiatla[15] = UtwSwiatlo(384, 281);
	Swiatla[16] = UtwSwiatlo(300, 300);
	Swiatla[17] = UtwSwiatlo(476, 298);
	Swiatla[18] = UtwSwiatlo(542, 313);
	Swiatla[19] = UtwSwiatlo(330, 331);
	Swiatla[20] = UtwSwiatlo(378, 320);
	Swiatla[21] = UtwSwiatlo(423, 326);
	Swiatla[22] = UtwSwiatlo(488, 342);
	Swiatla[23] = UtwSwiatlo(413, 367);
	Swiatla[24] = UtwSwiatlo(473, 377);
	Swiatla[25] = UtwSwiatlo(369, 399);
	Swiatla[26] = UtwSwiatlo(422, 415);
	Swiatla[27] = UtwSwiatlo(553, 401);
	Swiatla[28] = UtwSwiatlo(525, 430);
	Swiatla[29] = UtwSwiatlo(304, 448);
	Swiatla[30] = UtwSwiatlo(367, 450);
	Swiatla[31] = UtwSwiatlo(468, 473);
	Swiatla[32] = UtwSwiatlo(335, 503);
	Swiatla[33] = UtwSwiatlo(404, 509);
	Swiatla[34] = UtwSwiatlo(285, 530);
	Swiatla[35] = UtwSwiatlo(396, 564);
	Swiatla[36] = UtwSwiatlo(324, 580);
	Swiatla[37] = UtwSwiatlo(392, 614);
	Swiatla[38] = UtwSwiatlo(281, 629);
	Swiatla[39] = UtwSwiatlo(328, 657);
	Swiatla[40] = UtwSwiatlo(398, 655);
	Swiatla[41] = UtwSwiatlo(307, 697);
	Swiatla[42] = UtwSwiatlo(587, 500);
	Swiatla[43] = UtwSwiatlo(460, 536);
	Swiatla[44] = UtwSwiatlo(517, 531);
	Swiatla[45] = UtwSwiatlo(598, 546);
	Swiatla[46] = UtwSwiatlo(506, 584);
	Swiatla[47] = UtwSwiatlo(588, 614);
	Swiatla[48] = UtwSwiatlo(471, 625);
	Swiatla[49] = UtwSwiatlo(570, 655);
	Swiatla[50] = UtwSwiatlo(516, 665);
	Swiatla[51] = UtwSwiatlo(471, 680);
	Swiatla[52] = UtwSwiatlo(387, 706);
	Swiatla[53] = UtwSwiatlo(569, 703);
	Swiatla[54] = UtwSwiatlo(231, 325);
	Swiatla[55] = UtwSwiatlo(129, 431);
	Swiatla[56] = UtwSwiatlo(75, 564);
	Swiatla[57] = UtwSwiatlo(166, 545);
	Swiatla[58] = UtwSwiatlo(205, 580);
	Swiatla[59] = UtwSwiatlo(155, 666);
	Swiatla[60] = UtwSwiatlo(218, 664);
	Swiatla[61] = UtwSwiatlo(33, 686);
	Swiatla[62] = UtwSwiatlo(137, 694);
	Swiatla[63] = UtwSwiatlo(249, 703);
	Swiatla[64] = UtwSwiatlo(622, 322);
	Swiatla[65] = UtwSwiatlo(629, 704);
	Swiatla[66] = UtwSwiatlo(684, 674);
	Swiatla[67] = UtwSwiatlo(751, 685);
	Swiatla[68] = UtwSwiatlo(820, 677);
	Swiatla[69] = UtwSwiatlo(581, 306);
	Swiatla[70] = UtwSwiatlo(228, 429);

}

void WyswietlSwiatla() {
	Swiatlo* s;

	for (int i = 0; i < ILOSC_SWIATEL; i++) {
		s = &Swiatla[i];

		switch (s->Stan) {

		case Zaswiecane:
			s->Przezroczystosc += ZMIEN_PRZEZROCZYSTOSC;
			if (s->Przezroczystosc > 1.0) s->Przezroczystosc = 1.0;
			s->Pedzel->SetOpacity(s->Przezroczystosc);
			RysujObraz(s->Pedzel, s->x, s->y, 15, 15);
			break;

		case Zaswiecone:
		case Swiecace:
			s->Pedzel->SetOpacity(1.0);
			RysujObraz(s->Pedzel, s->x, s->y, 15, 15);
			break;

		case Gasnace:
			s->Przezroczystosc -= ZMIEN_PRZEZROCZYSTOSC;
			if (s->Przezroczystosc < 0.0) s->Przezroczystosc = 0.0;
			s->Pedzel->SetOpacity(s->Przezroczystosc);
			RysujObraz(s->Pedzel, s->x, s->y, 15, 15);
			break;
		}

	}

}

void PrzetworzSwiatla() {

	for (int i = 0; i < ILOSC_SWIATEL; i++) {

		switch (Swiatla[i].Stan) {

		case Wylaczone:
			if ((rand() % 100) < PRAWD_ZASWIEC) {
				Swiatla[i].Pedzel = PedzleSwiatla[rand() % ILOSC_PEDZLI_SWIATLA];
				Swiatla[i].Stan = Zaswiecane;
			}
			continue;

		case Zaswiecane:
			Swiatla[i].Stan = Zaswiecone;
			continue;

		case Zaswiecone:
			Swiatla[i].Stan = Swiecace;
			continue;

		case Swiecace:
			if ((rand() % 100) < PRAWD_WYLACZ) {
				Swiatla[i].Stan = Gasnace;
			}
			continue;

		case Gasnace:
			Swiatla[i].Stan = Zgaszone;
			continue;

		case Zgaszone:
			Swiatla[i].Stan = Wylaczone;
			continue;
		}

	}

}

void CzyscSwiatla() {
	for (int i = 0; i < ILOSC_PEDZLI_SWIATLA; i++) {
		SafeRelease(&PedzleSwiatla[i]);
	}
}