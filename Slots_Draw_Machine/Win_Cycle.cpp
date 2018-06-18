#include "stdafx.h"
#include "Win_Cycle.h"

namespace Slots_Draw_Machine
{
	Win_Cycle::Win_Cycle()
	{
		int const NONE_ITEM = 0;
		size = NONE_ITEM;
	}

	void Win_Cycle::Reset()
	{
		// czyszczenie tablicy losowan

		int const FIRST_ITEM = 0;
		counter = FIRST_ITEM; // zerowanie licznika 

		for (int i = FIRST_ITEM; i < size; ++i) // reset wszystkich elementow
		{
			win[i] = false;
		}

		int lucky;
		lucky = rand() % size;

		win[lucky] = true; // zaznaczenie jednej wygranej
	}

	bool Win_Cycle::Check()
	{
		// zwraca czy w danej turze ma pojawic sie wygrana

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		int const NONE_ITEM = 0;
		ASSERT(size == NONE_ITEM, "Nie ustawiono wielkosci cyklu!!!")
#endif

			if (counter >= size) // jesli osiagnie koniec zakresu
			{
				Reset();
			}

		bool result;
		result = win[counter]; // pobierz wynik z tablicy

		return result;
	}

	void Win_Cycle::Change_Size()
	{
		// zmienia rozmiar cyklu, w ktorym nastepuje jedna wygrana
		// inaczej mowiac: dlugosc cyklu oznacza jak czesto srednio nastepuje wygrana

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		int const MIN_SIZE = 1;
		int const MAX_SIZE = ARRAY_LENGTH(win);
		TEST_RANGE(next_size, MIN_SIZE, MAX_SIZE)
#endif

			if (size != next_size) // jesli wczesniej nie ustalono juz takiej wielkosci
								   //  zabezpieczenie przed czyszczeniem tablicy wyborow jesli wartosc sie nie zmienia
			{
				size = next_size;
				Reset();
			}
	}

	void Win_Cycle::Mark_Used()
	{
		// przechodzi do kolejnej tury w cyklu

		++counter; // zwiekszenie licznika

		if (counter > size)
		{
			int const START_POSITOIN = 0;
			counter = START_POSITOIN;
		}
	}



	int Win_Cycle::Get_Size()
	{
		// pobiera wielkosc cyklu

		return size;
	}
}