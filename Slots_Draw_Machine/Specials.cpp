#include "stdafx.h"
#include "Specials.h"

namespace Slots_Draw_Machine
{
	Specials::Specials(
		Lines_Designer &design,
		Symbols &symb,
		Result_Board &board, 
		Slot_Parameter &param, 
		Game &game, 
		RTP &rtp) :
		WILD_SETUP{ 
		{ false, false, true, true, true }, // czy wildy maja byc jeden nad drugim dla dwoch wildow
		{ false, false, false, true, true } } // czy wildy maja byc jeden nad drugim dla trzech wildow
	{
		this->design = &design;
		this->symb = &symb;
		this->board = &board;
		this->param = &param;
		this->game = &game;
		this->rtp = &rtp;

		int const VAL[8] = { 99, 90, 60, 40, 25, 12, 6, 3 }; // progi dla prawdopodobienstwa pojawienia sie mystery symbol

		Items mystery_scopes;
		mystery_scopes.counter = ARRAY_LENGTH(VAL); // tworzenie obiektu do przeslania progow
		for (int i = 0; i < mystery_scopes.counter; ++i)
		{
			mystery_scopes.val[i] = VAL[i];
		}

		mystery.Set_Scope(mystery_scopes); // nadanie wartosci progom w obiekcie 'Probability'

		Clear_Wild_Used();
	}

	void Specials::Copy_To_Back()
	{
		// kopiuje tablice wynikow w obiekcie 'board' z tablicy 'result' na tablice 'back'
		// uzywane w przypadkach zdarzen specjanych, ktore maja przykryc jakies symbole na tablicy wynikow

		board->backside_on = true; // wlaczenie flagi wskazujacej, ze zapisana jest tylna tablica wynikow, 
					  	  	      // gdy pojawiaja sie znaki specjalne przyslaniajace niektore symbole

		const int BOARD_START = 0;
		const int BOARD_WIDE = ARRAY_LENGTH(board->result);
		const int BORAD_HIGH = ARRAY_SECOND(board->result);
		for (int i = BOARD_START; i < BOARD_WIDE; ++i) // kopiowanie na tylna tablice;
		{
			for (int j = BOARD_START; j < BORAD_HIGH; ++j)
			{
				board->backside[i][j] = board->result[i][j];
			}
		}
	}

	Position Specials::Draw_Position()
	{
		// zwraca pozycje z okreslonej wielkosci tablicy wynikow

		Position pos;

		pos.x = rand() % (board->Get_Length()); // losuj ktory ring

		int ring_hight = board->Get_Height(pos.x);
		pos.y = rand() % ring_hight; // losuj pozycje na ringu

		return pos;
	}

	void Specials::Clear_Wild_Used()
	{
		// zeruje tablce urzyc wildow

		for (int i = 0; i < ARRAY_LENGTH(wild_used); ++i) // wyczyszczenie pamieci uzyc wildow
		{
			for (int j = 0; j < ARRAY_SECOND(wild_used); ++j)
			{
				bool const EMPTY = false;
				wild_used[i][j] = EMPTY;
			}
		}

		int const ALL_USED = ARRAY_LENGTH(wild_used) * ARRAY_SECOND(wild_used);
		wild_left = ALL_USED; // zerowanie licznika uzytych wildow
	}

	void Specials::Wilds_Positons(int num, int setup, int spec)
	{
		// wstawia okreslona ilosc wildow w wybranej formie na tablice wynikow

		Position pos;
		int const ONE_WILD = 1;
		num += ONE_WILD; // przesuniecie licznika, tak aby mial wrtosci 1, 2, 3, a nie 0, 1, 2
		if (num > ONE_WILD // jesli ma byc wrysowanych wiecej ni¿ tylko jeden wild
			&& WILD_SETUP[num - ONE_WILD][setup] == true) // i jesli wildy maja wystapic jeden pod drugim
		{
			pos.x = rand() % board->Get_Length(); // wylosuj na ktorym ringu

			int const NO_POSITION = -10;
			int const AROUND = 1;
			int last_pos_y = NO_POSITION; // w pamieci ostatniej pozycji zaznacz, ze nie by³o jeszcze losowania
			for (int i = 0; i < num + ONE_WILD; ++i)
			{
				do
				{
					pos.y = rand() % board->Get_Height(pos.x); // wylosuj pozycje na ringu

				} while (board->Get_Symbol(pos) == spec // jeszcze nie zaznaczona na tablicy wynikow
					&& !(last_pos_y + AROUND == pos.y // o jendna komorke nizej
						|| last_pos_y - AROUND == pos.y // lub o jedna komorke wyzej
						|| last_pos_y == NO_POSITION)) // lub jesli nie byla jeszcze losowana
					;

				board->Set_Symbol(pos, spec); // zaznacz na tablicy wynikow

				last_pos_y = pos.y; // zapamietaj
			}
		}
		else // jesli wildy mogoa byc ustawione dowolnie
		{
			for (int i = 0; i < num + ONE_WILD; ++i)
			{
				do
				{
					pos = Draw_Position(); // losuj nowa pozycje

				} while (board->Get_Symbol(pos) == spec) // jesli nie byla juz zaznaczona na tablicy wynikow
					;

				board->Set_Symbol(pos, spec); // zaznacz na tablicy wynikow
			}
		}
	}

	void Specials::Draw_Vertical(int num, int symbol)
	{
		// wrysowuje na tablicy wynikow kilka symboli w pionie

		Position pos;

		int const NO_SYMBOL = 0;
		int const MIN_BOARD_LENGTH = 3;
		if (design->mem.size > NO_SYMBOL // jesli w tej turze nie wylosowano zadneych linii
			|| board->Get_Length() == MIN_BOARD_LENGTH) // lub jesli tablica wynikow ma minimalna szerokosc
		{
			pos.x = rand() % board->Get_Length(); // wylosuj na ktorym ringu
		}
		else // jesli wylosowano jakiekolwiek linie
		{
			int const FIRST_THREE_RINGS = 3;
			int const REST_RINGS = board->Get_Length() - FIRST_THREE_RINGS;
			pos.x = (rand() % REST_RINGS) + FIRST_THREE_RINGS; // wylosuj 4tym lub dalszym ringu
		}

		int const TWO_SYMBOLS = 2;
		int const NO_POSITION = -10;
		int const AROUND = 1;
		int last_pos_y = NO_POSITION; // w pamieci ostatniej pozycji zaznacz, ze nie by³o jeszcze losowania

		for (int i = 0; i < num; ++i)
		{
			do
			{
				pos.y = rand() % board->Get_Height(pos.x); // wylosuj pozycje na ringu

			} while (board->Get_Symbol(pos) == symbol // jeszcze nie zaznaczona na tablicy wynikow
				&& !(last_pos_y + AROUND == pos.y // o jendna komorke nizej
					|| last_pos_y - AROUND == pos.y // lub o jedna komorke wyzej
					|| last_pos_y == NO_POSITION)) // lub jesli nie byla jeszcze losowana
				;

			board->Set_Symbol(pos, symbol); // zaznacz na tablicy wynikow

			last_pos_y = pos.y; // zapamietaj
		}
	}

	void Specials::Draw_Horizontal(int num, int symbol)
	{
		// wrysowuje na tablicy wynikow kilka symboli w poziomie

		Position pos;

		bool on_board;
		int mem_pos_x;
		do
		{
			pos = Draw_Position(); // losuj pozycje komorki na tablicy wynikow
			mem_pos_x = pos.x; // zapamietaj wylosowana pozycje x

			on_board = true;
			int const NEXT_CELL = 1;
			for (int i = NEXT_CELL; i < num; ++i) // sprawdz czy nastepne komorki mieszcza sie na tablicy wynikow
			{
				++pos.x; // przejdz do nastepnej komorki

				if (board->On_Shape(pos) == false) // sprawdz czy miesci sie w ksztalcie slotu
				{
					on_board = false; // zaznacz, ze komorka wystaje poza ksztalt
				}
			}

		} while (on_board == false) // tak dlugo, az wszystkie komorki znajduja sie na tablicy wyniow
			;

		pos.x = mem_pos_x; // przywroc zapamietana pozycje x

		int const FIRST_CELL = 0;
		for (int i = FIRST_CELL; i < num; ++i) // wrysuj na tablice wynikow
		{
			board->Set_Symbol(pos, symbol);

			++pos.x; // przejdz do nastepnej komorki
		}
	}

	int Specials::Mummy_Not_Used(bool *tab, int &num)
	{
		// sprawdza uzycie ustawien duzych obrazkow mumi

		int const TAB_ARRAY = 10;

		if (num == TAB_ARRAY) // jesli wykorzystano wszystkie komorki pamieci
		{
			int const FIRST_ITEM = 0;
			for (int i = FIRST_ITEM; i < TAB_ARRAY; ++i) // to wyczysc
			{
				*(tab + i) = false;
			}
			int NO_DRAWS = 0;
			num = NO_DRAWS; // zeruj licznik
		}

		int setup;
		do
		{
			setup = rand() % TAB_ARRAY; // losuj rodzaj ustawienia

		} while (*(tab + setup) == true) // az trafisz nieuzyte
			;

		++num; // zwieksz licznik wykorzystanych komorek pamieci

		return setup;
	}

	void Specials::Expand_Wild(int spec)
	{
		// losowanie wildow

		Copy_To_Back(); // kopiowanie tablicy wynikow z liniami na tylna tablice wynikow

		Position pos;
		int const CENTRAL_RING = board->Get_Length() / 2;
		pos.x = CENTRAL_RING;

		int const FIRST_SYMB = 0;
		int const RING_HIGHT = board->Get_Height(CENTRAL_RING);
		for (int i = FIRST_SYMB; i < RING_HIGHT; i++) // rysowanie srodkowego ringu wildami
		{
			pos.y = i;
			board->Set_Symbol(pos, spec);
		}

		bool first_and_second = false; // pamiec wystapienia na ringach - na pierwszym i drugim maksymalnie po jednym wildzie
		int const NUM_RAND_WILDS = 3;
		for (int i = 0; i < NUM_RAND_WILDS; ++i)
		{
			do
			{
				pos = Draw_Position();

			} while (board->Get_Symbol(pos) == spec // tylko nie w tym samym miejscu
				|| ((pos.x == 0 || pos.x == 1) && first_and_second == true)) // i tylko jeden na pierwszym ringu lub na drugim
				;

			board->Set_Symbol(pos, spec); // wstaw symbol
			if (pos.x == 0 || pos.x == 1) first_and_second = true; // zaznacz jesli wild pojawil sie z przodu
		}
	}

	void Specials::Wild(int spec)
	{
		// losuje wildy i wstawia je na tablicy wynikow

		Copy_To_Back();

		int setup;
		int num;
		do // znajdz nieuzyte ulozenie symboli
		{
			int const KINDS = 5;
			setup = rand() % KINDS;

			int const MAX_WILDS = 3;
			num = (rand() % MAX_WILDS);

		} while (wild_used[num][setup] == true)
			;

		Wilds_Positons(num, setup, spec);

		wild_used[num][setup] = true; // zaznacz uzycie


		--wild_left; // zmniejsz licznik pozostalych ustawin do uzycia

		int const NONE = 0;
		if (wild_left == NONE) // jesli uzyto wszystkich
		{
			Clear_Wild_Used(); // wyczysc tablice i zrestartuj licznik
		}
	}

	void Specials::Free_Spins(int num, int spec)
	{
		// losowanie okreslonej liczby freespinow

		const int THREE_FREE_SPIN = 3;

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int ONE_FREE_SPIN = 1;
		TEST_RANGE(num, ONE_FREE_SPIN, THREE_FREE_SPIN)
#endif

		Position pos;

		bool use[3] = { false, false, false }; // jesli trzy symbole to wlacz 10 bezplatnych tur
		if (num == THREE_FREE_SPIN)
		{
			int const TEN_FREE_TOURS = 10;
			game->free_spins = TEN_FREE_TOURS;
		}

		for (int i = 0; i < num; ++i) // ilosc free spinow
		{
			int ring;
			do
			{
				int const NUM_FREE_SPINS = 3;
				ring = rand() % NUM_FREE_SPINS; // losowanie na ktorym ringu pojawi sie free spin

			} while (use[ring] == true)
				;

			int const EVERY_ONE = 1; // dla tablicy szerokiej na 3 ringów
			int setup = EVERY_ONE; // wspolczynik rozciagniecia free spinow na tablicy wynikow
			if (board->Get_Length() == 5) // dla tablicy szerokiej na 5 ringów
			{
				int const EVERY_OTHER = 2;
				setup = EVERY_OTHER;
			}

			pos.x = ring * setup; // rozlozenie trafien na tablicy wynikow

			int ring_height = board->Get_Height(pos.x);
			pos.y = rand() % ring_height; // losowanie w ktorym miejscu ringu

			board->Set_Symbol(pos, spec); // wstawienie symbolu

			use[ring] = true; // zaznaczenie ze na tym ringu juz sie pojawil
		}
	}

	void Specials::Bonus_Game(int num, int spec)
	{
		// losowanie okreslonej liczby freespinow

		const int THREE_BONUS = 3;

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int ONE_BONUS = 1;
		TEST_RANGE(num, ONE_BONUS, THREE_BONUS)
#endif

			Position pos;

		bool use[3] = { false, false, false }; // jesli trzy symbole to dolicz wygrana z bonus game do rtp
		if (num == THREE_BONUS)
		{
			int const MIN_BONUS_MULTIP = 3;
			int const MAX_BONUS_MULTIP = 15;

			int bonus_multipler = (rand() % (MAX_BONUS_MULTIP - MIN_BONUS_MULTIP)) + MIN_BONUS_MULTIP;

			int bet_val = param->bet.value.val[param->bet.choose]; // pobranie wartosci zakladu

			rtp->bonus = bonus_multipler * bet_val; // ustawienie wartosci bonusu
		}

		for (int i = 0; i < num; ++i) // ilosc bonusow
		{
			int ring;
			do
			{
				int const NUM_FREE_SPINS = 3;
				ring = rand() % NUM_FREE_SPINS; // losowanie na ktorym ringu pojawi sie bonus

			} while (use[ring] == true)
				;

			int const EVERY_ONE = 1; // dla tablicy szerokiej na 3 ringów
			int setup = EVERY_ONE; // wspolczynik rozciagniecia bonusow na tablicy wynikow
			if (board->Get_Length() == 5) // dla tablicy szerokiej na 5 ringów
			{
				int const EVERY_OTHER = 2;
				setup = EVERY_OTHER;
			}

			pos.x = ring * setup; // rozlozenie trafien na tablicy wynikow

			int ring_height = board->Get_Height(pos.x);
			pos.y = rand() % ring_height; // losowanie w ktorym miejscu ringu

			board->Set_Symbol(pos, spec); // wstawienie symbolu

			use[ring] = true; // zaznaczenie ze na tym ringu juz sie pojawil
		}
	}
	
	void Specials::Mystery(int spec)
	{
		Copy_To_Back();

		int symbol;
		int const NO_LINES = 0;
		if (design->mem.size > NO_LINES) // jesli wylosowano w tej turze jakies linie
		{
			int what_mem = rand() % design->mem.size; // losuje symbol jaki ma sie kryc pod mystery symbol
			symbol = design->mem.symb[what_mem]; // z poli wylosowanych wczesniej linii
		}
		else // jesli nie bylo linii
		{
			symbol = symb->Get_Random(); // wylosuj z puli symboli;
			symb->Mark_Used(); // zaznacz uzycie
		}

		int draws = mystery.Draw_Scope(); // wylosuj ile ma sie pojawic mystery symbols
		mystery.Mark_Used(); // odnotuj uzycie prawdopodobienstwa

		Position pos;

		for (int i = 0; i < draws; ++i)
		{
			int const REPETS = 15;
			int const END_REPET = 0;
			int counter = REPETS;
			do
			{
				pos = Draw_Position(); // losuj pozycje mystery symbolu

				--counter;

			} while (board->backside[pos.x][pos.y] == spec // tylko nie w tym samym miejscu
				&& (counter > END_REPET // i tak dlugo jak licznik sie nie skonczy
				|| board->Get_Symbol(pos) != symbol)) // lub nie trafisz na symbol, ktory ma sie znajdowac pod mystery symbolem
				;

			board->backside[pos.x][pos.y] = spec; // wstaw mystery symbol
			board->Set_Symbol(pos, symbol); // wstaw trafiony symbol pod mystery symbol
		}
	}

	void Specials::Pictures_1_x_2_or_3(int symbol)
	{
		int const ZERO_OR_ONE = 2;
		int num = rand() % ZERO_OR_ONE;

		Draw_Vertical(num, symbol);
	}

	void Specials::Mummy_Smal(int spec)
	{
		int const DRAWS = ARRAY_LENGTH(mummy_smal);

		int setup = Mummy_Not_Used(&mummy_smal[0], num_mummy_smal);

		int const KIND_2X1 = 2; 
		if (setup > KIND_2X1) // jesli maja sie pojawic w pionie
		{
			int num;
			int const KIND_1X3 = 5;
			if (setup > KIND_1X3)
			{
				num = 3;
			}
			else
			{
				num = 2;
			}

			Draw_Vertical(num, spec);
		}
		else // jesli maja sie pojawic w poziomie
		{
			int const NUM = 2;

			Draw_Horizontal(NUM, spec);
		}
	}

	void Specials::Mummy_Medium(int spec)
	{
		int const KINDS[10] = { 1, 2, 2, 3, 3, 3, 4, 4, 4, 4 };

		int setup = Mummy_Not_Used(&mummy_medium[0], num_mummy_medium);
		int kind = KINDS[setup];

		Position pos;

		pos.x = rand() % kind; // losuj przesuniecie w poziomie

		int const VERTICAL_MOVE = 2;
		pos.y = rand() % VERTICAL_MOVE; // losuj przesuniecie w pionie

		int const SIZE_X = 6;
		int length_x = SIZE_X - kind; // ustal wielkosc obrazka, kolejno: 5, 4, 3 i 2
		int const LENGHT_Y = 2;
		int mem_pos_x = pos.x; // ustaw wysokosc obrazka na 2

		for (int y = 0; y < LENGHT_Y; y++) // wrysuj rysunek na rablicy wynikow
		{
			for (int x = 0; x < length_x; x++)
			{
				board->Set_Symbol(pos, spec);
				++pos.x;
			}
			++pos.y;
			pos.x = mem_pos_x;
		}
	}

	void Specials::Mummy_Big(int spec)
	{
		int const KINDS[10] = { 1, 2, 2, 3, 3, 3, 4, 4, 4, 4 };

		int setup = Mummy_Not_Used(&mummy_big[0], num_mummy_big);
		int kind = KINDS[setup];

		Position pos;

		pos.x = rand() % kind; // losuj przesuniecie w poziomie

		int const FIRST_LINE = 0;
		pos.y = FIRST_LINE; // ustaw pozycje y na pierwsza komorke od gory

		int const SIZE_X = 6;
		int length_x = SIZE_X - kind; // ustal wielkosc obrazka, kolejno: 5, 4, 3 i 2
		int const LENGHT_Y = 3;
		int mem_pos_x = pos.x; // ustaw wysokosc obrazka na 3

		for (int y = 0; y < LENGHT_Y; y++) // wrysuj rysunek na rablicy wynikow
		{
			for (int x = 0; x < length_x; x++)
			{
				board->Set_Symbol(pos, spec);
				++pos.x;
			}
			++pos.y;
			pos.x = mem_pos_x;
		}
	}

	void Specials::Mummy_Restart()
	{
		int const START_POSITION = 0;

		num_mummy_smal = START_POSITION;
		num_mummy_medium = START_POSITION;
		num_mummy_big = START_POSITION;

		int const LENGTH = ARRAY_LENGTH(mummy_smal);
		bool const NOT_USED = false;

		for (int i = 0; i < LENGTH; ++i)
		{
			mummy_smal[i] = NOT_USED;
			mummy_medium[i] = NOT_USED;
			mummy_big[i] = NOT_USED;
		}
	}

	void Specials::Wadjet(int spec)
	{
		Position pos;
		
		int const FIRST_RING = 0; // ustawienie slotu na pierwszy ring
		pos.x = FIRST_RING;

		int const UP_OR_DOWN = 2; 
		pos.y = rand() % UP_OR_DOWN; // losowanie czy obrazek ma byc na dole czy u gory

		int const TWO_CELL = 2;
		for (int y = 0; y < TWO_CELL; ++y) // rysowanie duzego obrazka
		{
			int mem_pos_x = pos.x;
			for (int x = 0; x < TWO_CELL; ++x)
			{
				board->Set_Symbol(pos, spec);
				++pos.x;
			}
			++pos.y;
			pos.x = mem_pos_x;
		}
	}
}
