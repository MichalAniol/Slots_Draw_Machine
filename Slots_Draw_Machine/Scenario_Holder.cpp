#include "stdafx.h"
#include "Scenario_Holder.h"

namespace Slots_Draw_Machine
{
	Scenario_Holder::Scenario_Holder()
	{
		int const NO_ITEM = 0;
		num_scen = NO_ITEM;

		int const FIRST_ITEM = 0;
		for (int i = FIRST_ITEM; i < ARRAY_LENGTH(hit); ++i) // zerowanie tablicy await_hits
		{
			hit[i] = NO_ITEM;
		}

		sum_await_hits = NO_ITEM;
	}

	bool Scenario_Holder::Load_All_Hits()
	{
		// sprawdza czy zdarzenia ze scenariuszy wystepuja w tej turze

		int const FIRST_SCENARIO = 0;
		for (int i = FIRST_SCENARIO; i < num_scen; ++i)
		{
			int what_scenario = scenario[0][i]->Get_First_Use(); // sprawdzenie czy scenariusz startowy zostal juz zuzyty
																	// bool'owe 0 i 1 odpowiada kolejno scenariuszom start i cycle

			int result;
			result = scenario[what_scenario][i]->Check_Hit();  // sprawdzenie czy trafilismy aktywator w scenariuszu i zapamietanie wyniku

			hit[i] += result;
			sum_await_hits += result; // sumowanie trafien
		}

		if (sum_await_hits > 0) return true; // gdy trafiono

		return false; // gdy nie trafiono
	}

	int Scenario_Holder::Check_Hit(int scenario_num)
	{
		// sprawdzenie czy zdarzenie z danego scenariusza wystepuje w tej turze

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_ITEM = 0;
		const int LAST_ITEM = num_scen - 1;
		TEST_RANGE(scenario_num, FIRST_ITEM, LAST_ITEM)
#endif

			return hit[scenario_num];
	}

	void Scenario_Holder::Use_Hit(int scenario_num)
	{
		// zaznaczenie zuzycia zdarzenia

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_ITEM = 0;
		const int LAST_ITEM = num_scen - 1;
		TEST_RANGE(scenario_num, FIRST_ITEM, LAST_ITEM)
			int const NO_ITEM = 0;
		ASSERT(hit[scenario_num] == NO_ITEM, "Brak Hita do zuzycia!!!")
#endif

		--hit[scenario_num];
		--sum_await_hits;
	}

	void Scenario_Holder::Activation(int games_at_this_slot)
	{
		// przygotowuje do uzycia wszystkie dodane scenariusze

		for (int i = 0; i < num_scen; i++)
		{
			scenario[0][i]->Activation(games_at_this_slot);
			scenario[1][i]->Activation(games_at_this_slot);
		}
	}

	void Scenario_Holder::Add_Scenario(Scenario &start_name, Scenario &cyclic_name)
	{
		// dodawanie scenariuszy
		// kolejnosc dodania jest hierarchia wykonywania scenariuszy,
		// jesli ma byc wykonanych kilka w jednej turze

#if RANGE == ON //--- SPRAWDZENIE NIE DODAJE WIECEJ ELEMENTOW NIZ JEST PRZYDZIELONE NA LISCIE
		const int FIRST_ITEM = 0;
		const int LAST_ITEM = ARRAY_LENGTH(hit) - 1;
		TEST_RANGE(num_scen, FIRST_ITEM, LAST_ITEM)
#endif

		scenario[0][num_scen] = &start_name;
		scenario[1][num_scen] = &cyclic_name;

		++num_scen;
	}

	int Scenario_Holder::Hit_form_Scenario()
	{
		// wskazuje ktory scenariusz nalezy wykonac w przypadku trafienia zdarzenia
		// kolejnosc ulozenia scenariuszy w holderze jest hierarchia wykonywania scenariuszy 

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		int const NO_ITEM = 0;
		ASSERT(num_scen == NO_ITEM, "Brak dodanych Scenariuszy!!!")
#endif

			int const NONE_HITS = 0;
		if (Load_All_Hits() == false) // jesli zaden scenariusz nie jest aktywny
		{
			return NONE_HITS;
		}

		const int FIRST_ITEM = 0;
		for (int scen = FIRST_ITEM; scen < num_scen; ++scen) // sprawdz po kolei scenariusze...
		{
			if (Check_Hit(scen) > 0) // ...ktory scenariusz daje hita
			{
				Use_Hit(scen); // zaznacz hita

				int const COUNT_FORM_ONE = 1;
				int result = scen + COUNT_FORM_ONE; // liczenie scenariuszy zaczyna sie od 1, 0 ozancza brak hita

				return result;
			}
		}

		return NONE_HITS; // zeby nie zglaszalo ostrzezen, ze sciezka kodu nic nie zwraca
	}
}