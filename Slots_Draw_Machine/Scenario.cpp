#include "stdafx.h"
#include "Scenario.h"

namespace Slots_Draw_Machine
{
	Scenario::Scenario() :
		LAST_CYCLE(ARRAY_LENGTH(cycle) - 1)
	{
		int const START_POSITION = 0;
		num_of_cycles = START_POSITION;
		sum_of_all_cycles = START_POSITION;
		scenario_position = START_POSITION;
		not_first_use = false;

		const int FIRST_ITEM = 0;
		const int USED_HIT = -1;
		cycle_hit[FIRST_ITEM] = USED_HIT;
	}

	int Scenario::Random(int min, int max)
	{
		// losuje z zakresu

		int result;
		result = (rand() % (max - min)) + min;

		return result;
	}

	void Scenario::Add_Cycle(int lenght, bool activ)
	{
		// dodaje cykl do listy

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_CYCLE = 0;
		TEST_RANGE(num_of_cycles, FIRST_CYCLE, LAST_CYCLE)
#endif

		cycle[num_of_cycles] = lenght;
		sum_of_all_cycles += lenght;

		cycle_active[num_of_cycles] = activ;
		++num_of_cycles;
	}

	void Scenario::Activation(int games_at_this_slot)
	{
		// przygotowuje scenariusz do uzycia, MUSI BYC WYKONANA PRZED UZYCIEM SCENARIUSZA

		scenario_position = games_at_this_slot;

		Set_as_Not_Used(); // zeby losowalo hity

		if (games_at_this_slot < sum_of_all_cycles) // sprawdzenie czy scenariusz jest juz zuzyty
		{
			not_first_use = false; // jesli ilosc dotychczasowych gier jest mniejsza niz suma wszystich cykli scenariusza
		}
		else
		{
			not_first_use = true;
		}
	}

	bool Scenario::Get_First_Use()
	{
		// sprawdza czy scenariusz jest uzywany pierwszy raz - istotne dla scenariuszy startowych

		return not_first_use;
	}

	bool Scenario::Check_Hit()
	{
		// sprawdza czy w danej turze scenariusz jest aktywny

		if (scenario_position >= sum_of_all_cycles)
		{
			scenario_position = 0;
			Set_as_Not_Used();
			not_first_use = true;
		}

		int sum_passed_cycles = 0;
		int cycle_num;
		for (cycle_num = 0; cycle_num < num_of_cycles; ++cycle_num) // oblicza, w ktorym cyklu jestesmy
		{
			int sum_with_next_cycle = sum_passed_cycles + cycle[cycle_num];

			if (sum_with_next_cycle > scenario_position) break;

			sum_passed_cycles = sum_with_next_cycle;
		}

		int cycle_positon = scenario_position - sum_passed_cycles; // w ktorym miejscu cyklu jestesmy

		bool result = false;
		if (cycle_positon == cycle_hit[cycle_num]) // sprawdza czy w tym miejscu znajduje sie hit
		{
			const int USED_HIT = -1;
			cycle_hit[cycle_num] = USED_HIT; // odznaczenie uzycia
			if (cycle_active[cycle_num] == true)
			{
				result = true;
			}
		}

		++scenario_position; // zwiekszenie licznika pozycji

		return result;
	}

	bool Scenario::Set_as_Not_Used()
	{
		// zeruje uzycia cykli scenariusza

		scenario_position = 0;

		int const FIRST_CYCLE = 0;
		for (int i = 0; i < num_of_cycles; ++i)
		{
			cycle_hit[i] = Random(FIRST_CYCLE, cycle[i]); // losowanie nowych hitow
		}

		return true;
	}
}