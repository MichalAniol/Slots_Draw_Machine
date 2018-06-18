#pragma once
#include "Data_Holders.h"

namespace Slots_Draw_Machine
{
	class Scenario
	{
		// lista cykli scenariusza
		// i procedury sprawdzajacej aktywnosc cyklu w danej turze

	private:
		const int LAST_CYCLE;

#if TESTS == ON
	protected:
#else
	private:
#endif
#if SHOW_READ_VAL == ON
	public:
#endif
		int num_of_cycles;            // ilosc cykli w scenariuszu
		int sum_of_all_cycles;       // suma wszystkich cykli w scenariuszu
		int scenario_position;      // aktualna pozycja wsrod wszystkich cykli zwiekszana co ture
		bool not_first_use;        // flaga uzycia scenariusza, podnosi sie po przejsciu raz przez wszystkie cykle
								  // lub jesli ilosc gier na danym slocie jest wieksza niz suma wszystkich cykli

		int cycle[20];          // tablica dlugosci cykli
		bool cycle_active[20]; // tablica flag aktywnosci cylu - jesli jest podniesiona to w cyklu wystapi 
							  // jeden raz aktywacja zdarzenia specjalnego przypisanego do scenariusza
		int cycle_hit[20];   // w ktorym momencie cyklu nastapi aktywacja

		int Random(int min, int max);

	public:
		Scenario();
		void Add_Cycle(int lenght, bool activ);
		void Activation(int games_at_this_slot);

		bool Get_First_Use();
		bool Check_Hit();
		bool Set_as_Not_Used();
	};
}