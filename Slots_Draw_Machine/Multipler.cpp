#include "stdafx.h"
#include "Multipler.h"

namespace Slots_Draw_Machine
{
	Multipler::Multipler(
		Lines_Designer &design,
		Slot_Parameter &param,
		Game &game,
		Shield &shield,
		RTP &rtp,
		Mega_Wins &mega_wins,
		Score &score)
	{
		this->design = &design;
		this->param = &param;
		this->game = &game;
		this->shield = &shield;
		this->rtp = &rtp;
		this->mega_wins = &mega_wins;
		this->score = &score;
	}

	float Multipler::Major()
	{
		// oblicza mnoznik w zaleznosci od aktywnych zdarzen i cykli w tej turze

		float const DEFAULT = 1;
		float multipler = DEFAULT;

		design->Set_Win_Next_Size(param->cycle.win); // ustawia standardowa dlugosc cyklu 'win'

		if (game->slot_today_games < param->cycle.everyday_start // jesli sa to pierwsze tury dzis na tym slocie
			|| game->all_today_games < shield->when_game_start) // jesli sa to pierwsze tury tego dnia
		{
			multipler = rtp->multip.go_up;  // to zwieksz mnoznik
			return multipler; // i nie sprawdzaj reszy parametrow
		}

		multipler *= Score_Goal(); // mnoznik redukcji zbyt duzej wygranej lub przegranej

		if (shield->must_win == true) // jesli wymuszona wygrana
		{
			multipler = rtp->multip.go_up * rtp->multip.go_up; // zwroc duzy mnoznik
			return multipler; // i nie szukaj dalej
		}

		if (game->can_lose == true) // jesli ma przegrac
		{
			multipler = rtp->multip.go_down; // to obniz mnoznik
			int const THREE_TOURS = 3;
			design->Set_Win_Next_Size(design->Get_Win_Next_Size() + THREE_TOURS); // i wydluz cykl 'win'
			return multipler;
		}

		multipler *= Rise_Cycle(); // mnoznik cykli zwiekszonych wygranych

		int const FREE_SPINS_OFF = 0;
		if (game->free_spins != FREE_SPINS_OFF) // jesli trwaja free spiny
		{
			multipler = DEFAULT; // ustaw standardowy mnoznik

			int const TOURS = 2;
			design->Set_Win_Next_Size(TOURS); // i ustaw 'win' na cykl do minimalnej dlugosci, czyli do dlugosci 2 tur
		}

		design->Change_Win_Size(); // zmiana dlugosc cyklu 'win' 

		return multipler;
	}

	float Multipler::Rise_Cycle()
	{
		// mnoznik wynikajacy z cykli zwiekszonych wygranych 

		float multipler;

		int const GAME_OVER = 0;
		if (param->cycle.raise_on == true // jesli jest podniesiona flaga zwiekszonych wygranych
			|| game->raise > GAME_OVER) // lub jesli przegrano wszystkie kredyty i wlaczyly sie zwiekszone wygrane po przegranej
		{
			multipler = rtp->multip.go_up; // to zwieksz mnoznik
		}
		else // jesli nie
		{
			float const DEFAULT = 1;
			multipler = DEFAULT; // to ustaw mnoznik na standardowy
		}

		Rise_Counter(); // nalicz kolejna ture w liczniku

		return multipler;
	}

	void Multipler::Rise_Counter()
	{
		// licznik cykli zwiekszonych wygranych

		--param->cycle.counter;

		if (param->cycle.counter < 0) // kiedy licznik sie skonczy
		{
			if (param->cycle.raise_on == true) // jesli skonczyl sie cykl podwyzszonych wygranych
			{
				int noise = (mega_wins->noise / 2) + (rand() % mega_wins->noise); // odchylenie od dlugosci cyklu
				param->cycle.counter = mega_wins->length + noise; // ustawienie cyklu

				param->cycle.raise_on = false;
			}
			else // jesli skonczyl sie cykl zwyklych wygranych
			{
				int noise = (mega_wins->ture_noise / 2) + (rand() % mega_wins->ture_noise); // odchylenie od dlugosci cyklu
				param->cycle.counter = mega_wins->ture_length + noise; // ustawienie cyklu

				int const TOURS = 2;
				design->Set_Win_Next_Size(design->Get_Win_Next_Size() + TOURS); // wydluzenie cyklu 'win'

				param->cycle.raise_on = true;
			}
		}
	}

	void Multipler::Rise_Start(int tours)
	{
		// aktywuje zwiekszone wygrane przez wskazana ilosc tur

		int noise = (mega_wins->noise / 2) + (rand() % mega_wins->noise); // odchylenie od dlugosci cyklu

		param->cycle.counter = tours + noise; // ustawienie dlugosci cyklu

		param->cycle.raise_on = false; // podnosi flage zwiekszonych wygranych
	}

	float Multipler::Score_Goal()
	{
		// mnoznik regulujacy zdobywane kredyty

		int const DEFAULT = 1;
		float multipler = DEFAULT;

		int goal = score->goal - (score->reduction / 2);
		if (goal > game->credits) // jesli kredytow jest znacznie mniej niz zaklada cel
		{
			multipler = rtp->multip.go_up; // to podnies mnoznik
		}

		goal = score->goal + (score->reduction / 2);
		if (goal < game->credits) // jesli kredytow jest znacznie wiecej niz zaklada cel
		{
			multipler = rtp->multip.go_down; // to obniz mnoznik

			int add_tours = 2;// (rand() % 5) + 2;
			design->Set_Win_Next_Size(add_tours); // i wydluz cykl 'win' o dwie tury
		}

		Score_Counter(); // nalicz kolejna ture w liczniku

		return multipler;
	}

	void Multipler::Score_Counter()
	{
		// licznik cykli zmian celu kredytow do zdobycia

		++score->counter; // zwieksz licznik
		if (score->counter >= score->cycle) // jesli licznik dotrze do konca cyklu
		{
			int const START_POSITION = 0;
			score->counter = START_POSITION; // zeruj licznik

			int aberrance;
			do
			{
				int reduct = score->reduction;
				aberrance = rand() % reduct; // wylosuj odchylenie w gore lub w dol

				if (rand() % 2 > 0)
				{
					aberrance = -aberrance;
				}

			} while (score->goal + aberrance > score->max // mieszczace sie w zakresie
				|| score->goal + aberrance < score->min);

			score->goal += aberrance; // zmien cel
		}
	}
}
