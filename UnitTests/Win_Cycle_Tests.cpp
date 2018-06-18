#include "stdafx.h"
#include "../Slots_Draw_Machine/Win_Cycle.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Slots_Draw_Machine;

namespace Unit_Testy
{
	TEST_CLASS(_Win_Cycle)
	{
	public:
		TEST_METHOD(_Check_Win_Cycle)
		{
			int size = 1;
			Win_Cycle cycle;
			cycle.next_size = size;
			cycle.Change_Size();

			bool outcome;
			int result = 0;
			int repeat = 20;
			for (int i = 0; i < repeat; ++i)
			{
				for (int j = 0; j < size; ++j)
				{
					outcome = cycle.Check();
					result += outcome;
					cycle.Mark_Used();
				}
			}
			Assert::AreEqual(repeat, result);

			size = 5;
			cycle.next_size = size;
			cycle.Change_Size();
			result = 0;
			for (int i = 0; i < repeat; ++i)
			{
				for (int j = 0; j < size; ++j)
				{
					outcome = cycle.Check();
					result += outcome;
					cycle.Mark_Used();
				}
			}
			Assert::AreEqual(repeat, result);

			size = 10;
			cycle.next_size = size;
			cycle.Change_Size();
			result = 0;
			for (int i = 0; i < repeat; ++i)
			{
				for (int j = 0; j < size; ++j)
				{
					outcome = cycle.Check();
					result += outcome;
					cycle.Mark_Used();
				}
			}
			Assert::AreEqual(repeat, result);
		}
	};
}