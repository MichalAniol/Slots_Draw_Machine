#include "stdafx.h"
#include "Symbols.h"

namespace Slots_Draw_Machine
{
	Symbols::Symbols() :
		LAST_RTP(ARRAY_LENGTH(rtp) - 1),
		LAST_RTP_ITEM(ARRAY_SECOND(rtp) - 1)
	{
		int const START_POSITION = 0; // na pozycji 0 znajduje sie pusty symbol
		size = START_POSITION;

		int const FIRST_ITEM = 0;
		for (int i = FIRST_ITEM; i < ARRAY_LENGTH(used.percent); ++i) // czyszczenie calej listy
		{
			used.percent[i] = false;
		}

		int const NONE = 0;
		used.all_left = NONE;

		int const EMPTY = -1;
		used.recently_scope[0] = EMPTY; // czyszczenie pamieci poprzednich losowan
		used.recently_scope[1] = EMPTY;
		used.recently_scope[2] = EMPTY;
		used.recently_scope[3] = EMPTY;

		used.recently_percent[0] = EMPTY;
		used.recently_percent[1] = EMPTY;
		used.recently_percent[2] = EMPTY;
		used.recently_percent[3] = EMPTY;
	}

	void Symbols::Add_RTP(int rtp_3rd)
	{
		// dodawanie jednej wartosci rtp dla symbolu

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int RTP_START = 0;
		TEST_RANGE(size, RTP_START, LAST_RTP)
#endif

		rtp[size][0] = rtp_3rd;

		int const NONE = 0;
		rtp[size][1] = NONE;
		rtp[size][2] = NONE;

		++size;
	}

	void Symbols::Add_RTP(int rtp_3rd, int rtp_4th, int rtp_5th)
	{
		// dodawanie trzech wartosi rtp dla symbolu

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int RTP_START = 0;
		TEST_RANGE(size, RTP_START, LAST_RTP)
#endif

		rtp[size][0] = rtp_3rd;
		rtp[size][1] = rtp_4th;
		rtp[size][2] = rtp_5th;

		++size;
	}

	void Symbols::Add_RTP_special(int rtp_5th)
	{
		// dodawanie specjalnej wartosi rtp dla linii skladajacej sie tylko z symboli specjalnych
		// nie zwieksza wielkosci tablicy symboli

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int RTP_START = 0;
		TEST_RANGE(size, RTP_START, LAST_RTP)
#endif

		int const NONE = 0;
		rtp[size][0] = NONE;
		rtp[size][1] = NONE;

		rtp[size][2] = rtp_5th;
	}

	void Symbols::Set_Spread(int spread)
	{
		// ustawienie prawdopodobienstwa symboli

		float rtp_sum = 0;
		float proportions[20];
		int const FIRST_ITEM = 0;
		for (int i = FIRST_ITEM; i < size; ++i)
		{
			proportions[i] = 0;
			for (int j = FIRST_ITEM; j < ARRAY_SECOND(rtp); ++j)
			{
				rtp_sum += rtp[i][j]; // zsumuj wszystkie rtp
				proportions[i] += rtp[i][j]; // zsumuj rtp dla kazdego symbolu
			}
		}

		for (int i = FIRST_ITEM; i < size; ++i) // logarytmiczne rozmieszczenie symboli, wyznaczane przez 'spread'
												   // duze rtp - maly procent, male rtp - duzy procent
		{
			float const HUNDRED_PERCENT = 100;
			proportions[i] = (rtp_sum / powf(proportions[i], (float(spread) / HUNDRED_PERCENT))) * (powf(float(spread), 1.1f));
		}

		rtp_sum = 0;
		for (int i = FIRST_ITEM; i < size; ++i) // sumowanie po rozmieszczeniu
		{
			rtp_sum += proportions[i];
		}

		Change_to_Percents(proportions, rtp_sum);

		Reset_Used();
	}

	void Symbols::Change_to_Percents(float part[], float sum)
	{
		// zmiana na procenty

		int const START_POSITION = 0;
		float temp_sum = START_POSITION;

		int const FIRST_ITEM = 0;
		int const LAST_ITEM = size - 1;
		int const END_OF_LIST = FIRST_ITEM - 1;

		int const HUNDRED_PERCENT = 100;
		for (int i = LAST_ITEM; i > END_OF_LIST; --i) // procenty obliczane od dolu tablicy
		{
			float result = START_POSITION;
			float temp_result;

			result = ((part[i] / sum) * HUNDRED_PERCENT); // wynik procentu we float

			temp_result = HUNDRED_PERCENT - (result + temp_sum); // odwrocenie / dopelnienie procenta
			temp_sum += result;
			result = temp_result;

			bool more_than_half;
			more_than_half = (result - int(result) > .5f); // zaokraglenie 
			if (more_than_half == true)
			{
				++result;
			}

			used.scope[i] = int(result);
		}

		used.scope[size] = HUNDRED_PERCENT; // domkniecie tablicy
	}

	int Symbols::Get_RTP(int symbol, int line_length)
	{
		// sprawdzanie rtp dla symbolu 

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_ITEM = -1;
		const int LAST_ITEM = size - 1 + 1; // z dodanym znakiem specjalnym nie wliczonym w 'size'
		TEST_RANGE(symbol, FIRST_ITEM, LAST_RTP)
			TEST_RANGE(line_length, FIRST_ITEM, LAST_ITEM)
#endif

			if (symbol > 0)
			{
				int const REDUCTION = 1;
				return rtp[symbol - REDUCTION][line_length]; // w symbolach na tablicy wynikow '0' to brak symbolu, 
													        //a tablica rtp symboli zaczyna sie od 0, dlatego -1

			}
			
		int const NONE = 0;
		return NONE;
	}

	int Symbols::Get_Random()
	{
		// zwraca wylosowany symbol z wyliczonych progow procentowych i zapamietuje go
		// pamieta ostatnie 3 symbole, aby sie nie powtorzyly oraz aby moc je odznaczyc po zakonczeniu tury 

		int symbol;
		int not_used;
		int const REPEATS = 100;
		int counter = REPEATS;
		do {
			symbol = 0;
			not_used = Rand_Percent();

			while (used.scope[symbol] <= not_used
				&& symbol < size) // szukanie progu odpowiadajacego danemu symbolowi
			{
				++symbol;
			}

			--counter;
			if (counter == 0) // licznik wyszukañ; gdy zostanie osiagniety, to natepuje restart uzyc
			{
				Reset_Used();
				counter = REPEATS;
			}

		} while (symbol == used.recently_scope[0] // porownanie z zapamietanymi wynikami, aby sie nie powtarzaly przez 3 tury
			|| symbol == used.recently_scope[1]
			|| symbol == used.recently_scope[2])
			;

		used.recently_scope[3] = used.recently_scope[2]; // zapamietanie progu
		used.recently_scope[2] = used.recently_scope[1]; 
		used.recently_scope[1] = used.recently_scope[0];
		used.recently_scope[0] = symbol;

		used.recently_percent[3] = used.recently_percent[2]; // zapamietanie losowan
		used.recently_percent[2] = used.recently_percent[1]; 
		used.recently_percent[1] = used.recently_percent[0];
		used.recently_percent[0] = not_used;

		return symbol;
	}

	int Symbols::Rand_Percent()
	{
		// losowanie nieuzytego procentu 

		int const LAST_ITEM = ARRAY_LENGTH(used.percent) - 1;
		int random = rand() % LAST_ITEM; // losowanie

		int look_for_not_used = random;
		while (used.percent[look_for_not_used] == true)
		{
			++look_for_not_used;
			if (look_for_not_used > LAST_ITEM) // jesli przekroczy zakres
			{
				int const FIRST_ITEM = 0;
				look_for_not_used = FIRST_ITEM;
			}

			if (look_for_not_used == random) // jesli przeszuka wszystkie
			{
				Reset_Used();
			}
		}

		return look_for_not_used;
	}

	Items Symbols::Get_A_Few_Random(int num)
	{
		// losuje kilka symboli na raz, maksymalnie 3

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_ITEM = 1;
		const int LAST_ITEM = 3;
		TEST_RANGE(num, FIRST_ITEM, LAST_ITEM)
#endif

		Items few_symb;
		few_symb.counter = num; // ile elementow znajduje sie w tablicy

		few_symb.val[0] = Get_Random(); // losuj pierwszy

		int const REPEATS = 30;
		int counter = REPEATS;
		switch (num)
		{
		case 1: break;
		case 2:
		{
			do {
				few_symb.val[1] = Get_Random(); // losowanie symbolu

				--counter;
				if (counter == 0) // licznik wyszukañ; gdy zostanie osiagniety, to nastepuje restart uzyc
				{
					Reset_Used();
					counter = REPEATS;
				}
			} while (few_symb.val[0] == few_symb.val[1]) // losuj az trafisz inny
				;
		} break;
		case 3:
		{
			do {
				few_symb.val[1] = Get_Random(); // losowanie symbolu

				--counter;
				if (counter == 0) // licznik wyszukan; gdy zostanie osiagniety, to nastepuje restart uzyc
				{
					Reset_Used();
					counter = REPEATS;
				}
			} while (few_symb.val[0] == few_symb.val[1]) // losuj az trafisz inny
				;
			counter = 30;
			do {
				few_symb.val[2] = Get_Random(); // losowanie symbolu

				--counter;
				if (counter == 0) // licznik wyszukan; gdy zostanie osiagniety, to nastepuje restart uzyc
				{
					Reset_Used();
					counter = REPEATS;
				}
			} while (few_symb.val[0] == few_symb.val[2]
				|| few_symb.val[1] == few_symb.val[2]) // losuj trzeci symbol, rozny od pozostalych dwoch
				;
		} break;
		}

		return few_symb;
	}

	void Symbols::Mark_Used()
	{
		// zaznaczenie, ze symbol zostal uzyty

		int const END_PERCENT = ARRAY_LENGTH(used.recently_percent);
		int const FIRST_PERCENT = 0;
		for (int num = FIRST_PERCENT; num < END_PERCENT; ++num) // zaznaczanie uzycia procentow z listy zapamietanych
		{
			int what_scope = used.recently_scope[num];

			int what_percent = used.recently_percent[num];

			bool percent_used = used.percent[what_percent];

			int const EMPTY = -1;
			if (what_scope != EMPTY // zapamietany procent nie jest pusty
				&& percent_used == false) // czy juz zaznaczony
			{
				used.percent[what_percent] = true; // zaznaczenie

				--used.all_left; // zmniejszenie licznika calego zbioru

				--used.left[what_scope]; // zmniejszenie licznika danego progu
			}
		}
	}

	void Symbols::Reset_Used()
	{
		// czyszczenie tablicy uzyc symbolu

		int const FIRST_ITEM = 0;
		for (int i = FIRST_ITEM; i < ARRAY_LENGTH(used.percent); ++i) // czyszczenie tablicy procentow
		{
			used.percent[i] = false;
		}

		int sum = 0;
		int const LAST_ITEM = size + 1;
		for (int i = FIRST_ITEM; i < LAST_ITEM; ++i) // ustawianie wielkosci pomiedzy progami
		{
			int number;
			number = used.scope[i] - sum;

			used.left[i] = number;

			sum += number;
		}

		int const ALL_ITEMS = ARRAY_LENGTH(used.percent);
		used.all_left = ALL_ITEMS; // reset licznika wszystkich czesci miedzy progami

		int const EMPTY = -1;
		used.recently_percent[0] = EMPTY; // kasowanie pamieci wylosowanych procentow
		used.recently_percent[1] = EMPTY;
		used.recently_percent[2] = EMPTY;
		used.recently_percent[3] = EMPTY;

		used.recently_scope[0] = EMPTY; // kasowanie pamieci wylosowanych progow
		used.recently_scope[1] = EMPTY;
		used.recently_scope[2] = EMPTY;
		used.recently_scope[3] = EMPTY;
	}



	int Symbols::Get_Size()
	{
		// zwraca ilosc symboli wystepujacych na danym slocie

		return size;
	}

	int Symbols::Get_Left()
	{
		// zwraca ilosc symboli, ktore nie zostaly uzyte  

		return used.all_left;
	}
}