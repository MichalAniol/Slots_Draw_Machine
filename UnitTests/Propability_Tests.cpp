#include "stdafx.h"
#include "../Slots_Draw_Machine/Probability.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Slots_Draw_Machine;

namespace Unit_Testy
{
	TEST_CLASS(_Propability)
	{
	public:
		class Inserts_Propability : public Probability
		{
		public:
			bool Get_Check_Use(int x) { return Check_Use(x); }
			void Get_Mark_Use() { Mark_Used(); }
			void Set_Reset() { Reset_Used(); }
			int Get_Draw() { return Draw_Percent(); }
			void Set_result(int x) { result = x; }
		};

		TEST_METHOD(_Most_Propability)
		{
			Inserts_Propability propa;

			Assert::AreEqual(false, propa.Get_Check_Use(0));
			Assert::AreEqual(false, propa.Get_Check_Use(1));
			Assert::AreEqual(false, propa.Get_Check_Use(10));
			Assert::AreEqual(false, propa.Get_Check_Use(99));

			int random;
			int const START_TEST = 0;
			int const REPET_TEST = 300;
			for (int i = START_TEST; i < REPET_TEST; ++i)
			{
				random = propa.Get_Draw();
				Assert::AreEqual(false, propa.Get_Check_Use(random)); // czy zawsze losuje puste
				propa.Set_result(random);
				propa.Get_Mark_Use();
				Assert::AreEqual(true, propa.Get_Check_Use(random)); // czy zaznacza
			}

			propa.Set_Reset();

			Assert::AreEqual(false, propa.Get_Check_Use(0)); // czy wyczyszczono liste uzycia
			Assert::AreEqual(false, propa.Get_Check_Use(1));
			Assert::AreEqual(false, propa.Get_Check_Use(10));
			Assert::AreEqual(false, propa.Get_Check_Use(99));

			int item;
			item = 10;

			propa.Set_result(item);
			propa.Get_Mark_Use();
			Assert::AreEqual(true, propa.Get_Check_Use(item)); // czy zaznacza uzycie

			item = 0;
			propa.Set_result(item);
			propa.Get_Mark_Use();
			Assert::AreEqual(true, propa.Get_Check_Use(item));

			item = 64;
			propa.Set_result(item);
			propa.Get_Mark_Use();
			Assert::AreEqual(true, propa.Get_Check_Use(item));

			item = 99;
			propa.Set_result(item);
			propa.Get_Mark_Use();
			Assert::AreEqual(true, propa.Get_Check_Use(item));
		}

		TEST_METHOD(_Cycle_Count_Propability)
		{
			Inserts_Propability propa;

			propa.Set_Scope(30);
			int result = 0;
			int temp;
			for (int j = 0; j < 100; ++j)
			{
				temp = propa.Draw_Scope();
				result += temp;
				propa.Mark_Used();
			}

			Assert::AreEqual(30, result);

			Items item;
			item.val[0] = 99;
			item.val[1] = 70;
			item.val[2] = 10;
			item.val[3] = 5;
			item.counter = 4;
			propa.Set_Scope(item);

			result = 0;
			for (int j = 0; j < 100; ++j)
			{
				temp = propa.Draw_Scope();
				result += temp;
				propa.Mark_Used();
			}

			Assert::AreEqual(85, result);
		}
	};
}