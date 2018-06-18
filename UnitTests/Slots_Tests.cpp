#include "stdafx.h"
#include "../Slots_Draw_Machine/Slot_Base.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Slots_Draw_Machine;
using namespace std;

namespace Unit_Testy
{
	TEST_CLASS(_Slot_Base)
	{
	public:
		class Inserts_Slot_Base : public Slot_Base
		{
		public:
			void Open_the_Test_File() { file.open("Test.cfg", ios::in); }
			bool Test_File() { return (file.good() == true); }
			void Close_File() { file.close(); }
			void Set_Find_this_Line(string x) { Find_this_Line(x); }
			void Set_Read_Line(int x) { Read_Line(x); }
			void Set_Read_Line(Items x, int y) { Read_Line(x, y); }
			void Get_Read_Scenario(Scenario &s) { Read_Scenario(s); }
		};

		TEST_METHOD(_Read_Value)
		{
			Inserts_Slot_Base slot;

			slot.Open_the_Test_File();  // wczytanie parametrow z pliku konfiguracyjnego
			if (slot.Test_File() == true)
			{
				string slot_data_begin = "-------  TEST  -------";
				slot.Set_Find_this_Line(slot_data_begin);

				int val = 0;
				slot.Set_Read_Line(val);
				Assert::AreEqual(1, val);

				Items item;
				slot.Set_Read_Line(item, 3);
				Assert::AreEqual(1, item.val[0]);
				Assert::AreEqual(2, item.val[1]);
				Assert::AreEqual(3, item.val[2]);

				slot.Set_Read_Line(item, 4);
				Assert::AreEqual(1, item.val[0]);
				Assert::AreEqual(2, item.val[1]);
				Assert::AreEqual(3, item.val[2]);
				Assert::AreEqual(4, item.val[3]);

				slot.Close_File();
			}
		}

		TEST_METHOD(_Read_Sceanrio)
		{
			Inserts_Slot_Base slot;

			slot.Open_the_Test_File();  // wczytanie parametrow z pliku konfiguracyjnego
			if (slot.Test_File() == true)
			{
				string slot_data_begin = "-------  SCENARIO  -------";
				slot.Set_Find_this_Line(slot_data_begin);

				class Inserts_Scenario : public Scenario
				{
				public:
					int Get_Cycle(int x) { return cycle[x]; }
					bool Get_Cycle_Active(int x) { return cycle_active[x]; }
				};

				Inserts_Scenario scenario;
				slot.Get_Read_Scenario(scenario);
				Assert::AreEqual(1, scenario.Get_Cycle(0));
				Assert::AreEqual(2, scenario.Get_Cycle(1));
				Assert::AreEqual(3, scenario.Get_Cycle(2));
				Assert::AreEqual(4, scenario.Get_Cycle(3));

				Assert::AreEqual(true, scenario.Get_Cycle_Active(0));
				Assert::AreEqual(false, scenario.Get_Cycle_Active(1));
				Assert::AreEqual(true, scenario.Get_Cycle_Active(2));
				Assert::AreEqual(false, scenario.Get_Cycle_Active(3));

				slot.Close_File();
			}
		}
	};
}