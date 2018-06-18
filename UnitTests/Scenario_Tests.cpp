#include "stdafx.h"
#include "../Slots_Draw_Machine/Scenario.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Slots_Draw_Machine;

namespace Unit_Testy
{
	TEST_CLASS(_Scenario)
	{
		class Inserts_Scenario : public Scenario
		{
		public:
			int Get_Num_of_Cycles() { return num_of_cycles; }
			int Get_Sum_of_all_Cycles() { return sum_of_all_cycles; }
			int Get_Scenario_Position() { return scenario_position; }
			int Get_Cycle(int x) { return cycle[x]; }
			bool Get_Cycle_Active(int x) { return cycle_active[x]; }
			int Get_Cycle_Hit(int x) { return cycle_hit[x]; }
			int Get_Random(int x, int y) { return Random(x, y); }
		};

	public:
		TEST_METHOD(_All_Scenario)
		{
			Inserts_Scenario scenario;
			scenario.Add_Cycle(10, true); // cykl pelen
			scenario.Add_Cycle(10, false); // cykl pusty
			scenario.Add_Cycle(10, true); // dodano 3 cykle po 10 tur kazdy

			scenario.Activation(0);

			Assert::AreEqual(0, scenario.Get_Scenario_Position());

			scenario.Set_as_Not_Used(); // tu odpalony tecznie do celow sprawdzenia dzialania, 
										// uruchamia sie automatucznie przy pierwszym uzyciu Check_Hit()

			Assert::AreEqual(3, scenario.Get_Num_of_Cycles());
			Assert::AreEqual(30, scenario.Get_Sum_of_all_Cycles());
			Assert::AreEqual(0, scenario.Get_Scenario_Position());
			Assert::AreEqual(10, scenario.Get_Cycle(0));
			Assert::AreEqual(true, scenario.Get_Cycle_Active(0));

			int cycle_num = 0;
			int cycle_hit = scenario.Get_Cycle_Hit(cycle_num);
			int rest_of_cycle = scenario.Get_Cycle(cycle_num) - cycle_hit;
			for (int i = 0; i < cycle_hit; ++i) // pierwszy cykl
			{
				Assert::AreEqual(false, scenario.Check_Hit());
			}
			Assert::AreEqual(true, scenario.Check_Hit()); // tu powinno znajdowac sie trafienie
			for (int i = 0; i < rest_of_cycle - 1; ++i) // resta cyklu
			{
				Assert::AreEqual(false, scenario.Check_Hit());
			}
			Assert::AreEqual(-1, scenario.Get_Cycle_Hit(cycle_num)); // czy cykl zostal odznaczony joko zurzyty

			cycle_num = 1;
			cycle_hit = scenario.Get_Cycle_Hit(cycle_num);
			rest_of_cycle = scenario.Get_Cycle(cycle_num) - cycle_hit;
			for (int i = 0; i < cycle_hit; ++i) // drugi cykl
			{
				Assert::AreEqual(false, scenario.Check_Hit());
			}
			Assert::AreEqual(false, scenario.Check_Hit()); // tu powinno znajdowac sie trafienie - cykl pusty
			for (int i = 0; i < rest_of_cycle - 1; ++i)  // resta cyklu
			{
				Assert::AreEqual(false, scenario.Check_Hit());
			}
			Assert::AreEqual(-1, scenario.Get_Cycle_Hit(cycle_num)); // czy cykl zostal odznaczony joko zurzyty

			scenario.Set_as_Not_Used(); // reset urzycia

			cycle_num = 0;
			cycle_hit = scenario.Get_Cycle_Hit(cycle_num);
			rest_of_cycle = scenario.Get_Cycle(cycle_num) - cycle_hit;
			for (int i = 0; i < cycle_hit; ++i) // pierwszy cykl
			{
				Assert::AreEqual(false, scenario.Check_Hit());
			}
			Assert::AreEqual(true, scenario.Check_Hit()); // tu powinno znajdowac sie trafienie
			for (int i = 0; i < rest_of_cycle - 1; ++i) // resta cyklu
			{
				Assert::AreEqual(false, scenario.Check_Hit());
			}
			Assert::AreEqual(-1, scenario.Get_Cycle_Hit(cycle_num)); // czy cykl zostal odznaczony joko zurzyty

			cycle_num = 1;
			cycle_hit = scenario.Get_Cycle_Hit(cycle_num);
			rest_of_cycle = scenario.Get_Cycle(cycle_num) - cycle_hit;
			for (int i = 0; i < cycle_hit; ++i) // drugi cykl
			{
				Assert::AreEqual(false, scenario.Check_Hit());
			}
			Assert::AreEqual(false, scenario.Check_Hit()); // tu powinno znajdowac sie trafienie - cykl pusy
			for (int i = 0; i < rest_of_cycle - 1; ++i) // resta cyklu
			{
				Assert::AreEqual(false, scenario.Check_Hit());
			}
			Assert::AreEqual(-1, scenario.Get_Cycle_Hit(cycle_num)); // czy cykl zostal odznaczony joko zurzyty

			int min = 0;
			int max = 9;
			bool test_random;
			int result;
			int const REPEAT_TEST = 10;
			for (int i = 0; i < REPEAT_TEST; ++i)
			{
				test_random = false;
				result = scenario.Get_Random(min, max);
				if (result >= min && result <= max)
				{
					test_random = true;
				}
				Assert::AreEqual(true, test_random);
			}
		}
	};
}