#pragma once
#include "Data_Holders.h"

namespace Slots_Draw_Machine
{
	class Win_Cycle
	{
		// losowanie jednej wygranej z okeslonej wielkosci cyklu
		// np jesli cykl wynosi: 3 to w 30 losowanich wygrana pojawi sie 10 razy
		// w losowym momencie cyklu

#if TESTS == ON
	protected:
#else
	private:
#endif
		bool win[20];    // do projektowania trafienia w danym cyklu o wielkosci 'size'
		int size;       // ilosc elementow
		int counter;  // licznik, w ktorym miejscu tablicy 'win' sie znajdujemy

		void Reset();

	public:
		int next_size; // projektowany rozmiar na nastepna ture

		Win_Cycle();

		bool Check();
		void Change_Size();
		void Mark_Used();

		// 'interface' do pobierania zmiennej z obiektu tej klasy
		int Get_Size();
	};
}
