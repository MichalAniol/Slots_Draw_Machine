#pragma once
#include "Scenario.h"

namespace Slots_Draw_Machine
{
	class Scenario_Holder
	{
		// lista wszystkich scenariuszy na slocie ulozona w hierarchii waznosci
		// z procedura podajaca czy i ktory scenariusz jest aktywny w danej turze

#if TESTS == ON
	protected:
#else
	private:
#endif

		Scenario *scenario[2][8]; // tablica wskaznikow na scenariusze
		int num_scen;            // ilosc dodanych scenariuszy
		int hit[8];             // ilosc aktywacji poszczegolnych scenariuszy
		int sum_await_hits;    // suma wszystkich aktywacji

		bool Load_All_Hits();
		int Check_Hit(int scenario_num);
		void Use_Hit(int scenario_num);

	public:
		Scenario_Holder();
		void Activation(int games_at_this_slot);

		void Add_Scenario(Scenario &start_name, Scenario &cyclic_name);
		int Hit_form_Scenario();
	};
}
