#include "stdafx.h"
#include "../Slots_Draw_Machine/Scenario_Holder.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Slots_Draw_Machine;

namespace Unit_Testy
{
	TEST_CLASS(_Scenario_Holder)
	{
		class Inserts_Scenario_Holder : public Scenario_Holder
		{
		public:
			int Get_num_scen() { return num_scen; }
			bool Get_Load_All_Hits() { return Load_All_Hits(); }
			int Get_Check_Hit(int x) { return Check_Hit(x); }
			int Get_sum_await_hits() { return sum_await_hits; }
			void Set_Use_Hit(int x) { return Use_Hit(x); }
		};

	public:
		TEST_METHOD(_Most_Scenario_Holder)
		{
			Scenario one_start, one_cycle;
			Scenario two_start, two_cycle;

			one_start.Add_Cycle(3, true);
			one_start.Add_Cycle(3, true);
			one_start.Add_Cycle(3, true);
			one_cycle.Add_Cycle(3, true);
			one_cycle.Add_Cycle(3, true);
			one_cycle.Add_Cycle(3, true);

			two_start.Add_Cycle(3, true);
			two_start.Add_Cycle(3, true);
			two_start.Add_Cycle(3, true);
			two_cycle.Add_Cycle(3, true);
			two_cycle.Add_Cycle(3, true);
			two_cycle.Add_Cycle(3, true);

			Inserts_Scenario_Holder holder;
			holder.Add_Scenario(one_start, one_cycle);
			holder.Add_Scenario(two_start, two_cycle);
			Assert::AreEqual(2, holder.Get_num_scen());

			holder.Activation(0);

			int sum_of_hits = 0;
			holder.Get_Load_All_Hits();
			holder.Get_Load_All_Hits();
			holder.Get_Load_All_Hits();
			holder.Get_Load_All_Hits();
			holder.Get_Load_All_Hits();
			holder.Get_Load_All_Hits();
			holder.Get_Load_All_Hits();
			holder.Get_Load_All_Hits();
			holder.Get_Load_All_Hits();
			holder.Get_Load_All_Hits();
			sum_of_hits = holder.Get_Check_Hit(0) + holder.Get_Check_Hit(1);
			Assert::AreEqual(sum_of_hits, holder.Get_sum_await_hits()); // czy sumy hitow sie zgadzaja

			holder.Set_Use_Hit(0);
			holder.Set_Use_Hit(1);

			sum_of_hits = holder.Get_Check_Hit(0) + holder.Get_Check_Hit(1);
			Assert::AreEqual(sum_of_hits, holder.Get_sum_await_hits());
		}

		TEST_METHOD(_Hit_form_Scenario)
		{
			Scenario one_start, one_cycle;
			Scenario two_start, two_cycle;

			one_start.Add_Cycle(3, true);
			one_start.Add_Cycle(3, true);
			one_start.Add_Cycle(3, true);
			one_cycle.Add_Cycle(3, true);
			one_cycle.Add_Cycle(3, true);
			one_cycle.Add_Cycle(3, true);

			two_start.Add_Cycle(3, true);
			two_start.Add_Cycle(3, true);
			two_start.Add_Cycle(3, true);
			two_cycle.Add_Cycle(3, true);
			two_cycle.Add_Cycle(3, true);
			two_cycle.Add_Cycle(3, true);

			Inserts_Scenario_Holder holder;
			holder.Add_Scenario(one_start, one_cycle);
			holder.Add_Scenario(two_start, two_cycle);
			
			holder.Activation(0);

			int result;
			int const START_TEST = 0;
			int const REPET_TEST = 100; // ilosc sprawdzen sredniej wynikow
			for (int i = START_TEST; i < REPET_TEST; ++i)
			{
				const int NONE = 0;
				result = NONE;

				const int FIRST_SCENARIO = 0;
				const int LAST_SCENARIO = 100; // ilosc cykli scenariuszy do zsumowania
				for (int j = FIRST_SCENARIO; j < LAST_SCENARIO; ++j)
				{
					const int START_CYCLE = 0;
					const int END_CYCLE = 3;
					for (int k = START_CYCLE; k < END_CYCLE; ++k)
					{
						int hit;
						hit = holder.Hit_form_Scenario();
						result += hit;
					}
				}

				int average;
				int const DEPRECIATION = 3; // najwyzsza warosc z cyklu na wypadek gdyby ostani cykl sie niewykonal w calosci
				average = (result + DEPRECIATION) / LAST_SCENARIO;

				Assert::AreEqual(3, average);
			}
		}
	};
}