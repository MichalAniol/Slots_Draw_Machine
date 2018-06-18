#include "stdafx.h"
#include "Probability.h"

namespace Slots_Draw_Machine
{
	Probability::Probability() :
		LAST_HAPPEN(ARRAY_LENGTH(used) - 1)
	{
		// zerowanie i czyszczenie zmiennych i tablic

		int const START_POSITION = 0;
		result = START_POSITION;

		int const FIRST_ITEM = 0;
		for (int item = FIRST_ITEM; item < ARRAY_LENGTH(scope); ++item) // zerowanie tablicy progow
		{
			scope[item] = START_POSITION;
		}

		num_scope = START_POSITION;

		Reset_Used();
	}

	int Probability::Draw_Percent()
	{
		// losowanie 1go ze 100 i usuniecie wylosowanego z puli nastepnych losowan

		int const LAST_5_PERCENT = 5;
		if (left < LAST_5_PERCENT) // jesli zostanie tylko piec roznych procentow do wylosowania
		{
			Reset_Used();
		}

		int const END_OF_HAPPEN = ARRAY_LENGTH(used);
		int random_item = rand() % END_OF_HAPPEN; // wylosuj 

		if (Check_Use(random_item)) // jesli zostal uzyty w tym cyklu, znajdz najblizszy nieuzyty
		{
			int memo_rand_item = random_item; // zapamietaj losowanie

			int const FIRST_ITEM = 0;
			do
			{
				++random_item; // przejdz do nastepnego
				if (random_item > LAST_HAPPEN) // jesli przekroczy zakres
				{
					random_item = FIRST_ITEM;
				}

				if (random_item == memo_rand_item) // jesli wszystkie zostaly zuzyte, wyczysc liste uzyc
				{
					Reset_Used();
				}

			} while (Check_Use(random_item))
				;
		}

		result = random_item; // zapamietaj wynik

		return random_item;
	}

	bool Probability::Check_Use(int item)
	{
		// sprawdzanie czy juz wylosowano ta wartosc

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_ITEM = 0;
		TEST_RANGE(item, FIRST_ITEM, LAST_HAPPEN)
#endif

		bool result;
		result = used[item];

		return result;
	}

	void Probability::Set_Scope(int val)
	{
		// ustawia wartosci prawdopodobienstwa progow w przypadku jednego progu - do losowan 0-1-owych niesymetrycznych

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH

#endif

		int const FIRST_SCOPE = 0;
		scope[FIRST_SCOPE] = LAST_HAPPEN; // pierwszy jest ustawiony na 100 % (czyli ma wartosc 99)

		int const SCOPE_ONE = 1;
		if (val > LAST_HAPPEN) // jesli przekracza 100 % 
		{
			scope[SCOPE_ONE] = LAST_HAPPEN; // to ustaw na 100 %
		}
		else // jesli mniej niz 100 %
		{
			scope[SCOPE_ONE] = val; // ustaw wartosc
		}

		int const NUM_OF_SCOPES = 2; // ustaw ilosc progow
		num_scope = NUM_OF_SCOPES;
	}

	void Probability::Set_Scope(Items &val)
	{
		// ustawia wartosci prawdopodobienstwa progow w przypadku wielu progow

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		for (int i = 1; i < val.counter; ++i)
		{
			ASSERT(val.val[i] > val.val[i - 1], "Niewlasciwa kolejnosc!!!")
		}
#endif

		int const FIRST_SCOPE = 0;
		for (int counter = FIRST_SCOPE; counter < val.counter; ++counter) // wpisz po kolei wszystkie progi
		{
			if (val.val[counter] > LAST_HAPPEN)
			{
				scope[counter] = LAST_HAPPEN; // jesli prog jest wiekszy niz 100 %
			}
			else
			{
				scope[counter] = val.val[counter]; // wpisz wartosc
			}
		}

		num_scope = val.counter; // zapisz ilosc progow
	}

	void Probability::Mark_Used()
	{
		// zaznaczanie uzycia wylosowanego procentu z zapamietanego wyniku

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_ITEM = 0;
		TEST_RANGE(result, FIRST_ITEM, LAST_HAPPEN)
#endif

			used[result] = true;
	}

	void Probability::Mark_Used(int item)
	{
		// zaznaczanie uzycia wylosowanego procentu z podanej wartosci

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_ITEM = 0;
		TEST_RANGE(item, FIRST_ITEM, LAST_HAPPEN)
#endif

			used[item] = true;
	}

	int Probability::Draw_Scope()
	{
		// zwraca prog z wylosowanego procentu
		
#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		int const START_POSITION = 0;
		ASSERT(num_scope == START_POSITION, "Nie ustawiono wartosci progow!!!")
#endif

		int draw;
		draw = Draw_Percent(); // losuj procent

		int const FIRST_SCOPE = 0;
		int const LAST_SCOPE = num_scope - 1;

		int outcome;
		outcome = LAST_SCOPE; // ustaw wynik na ostatni prog

		while (draw >= scope[outcome] // dopoki wylosowana liczba jest wieksza od progu
			&& outcome != FIRST_SCOPE) // i dopoki wynik nie jest mniejszy niz koniec listy progow
		{
			--outcome; // zmniejsz licznik na kolejny prog
		}

		return outcome;
	}

	void Probability::Reset_Used()
	{
		// czyszczenie tablicy losowan

		int const FIRST_ITEM = 0;
		for (int i = FIRST_ITEM; i < ARRAY_LENGTH(used); ++i)
		{
			used[i] = false;
		}

		int const ALL_ITEMS = ARRAY_LENGTH(used);
		left = ALL_ITEMS;
	}



	int Probability::Get_Resutl()
	{
		// zwraca zachowany wynik

		return result;
	}

	int Probability::Get_Left()
	{
		return left;
	}


}
