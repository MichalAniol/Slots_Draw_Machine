#include "stdafx.h"
#include "../Slots_Draw_Machine/Lines.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Slots_Draw_Machine;

namespace Unit_Testy
{
	TEST_CLASS(_Lines)
	{
	public:

		class Inserts_Lines : public Lines
		{
		public:
			Inserts_Lines(Symbols &symb, Result_Board &board) : Lines(symb, board) {}
			int Get_line_num() { return num_lines; }
			Items Get_Get(int x) { return Get(x); }
			bool Get_Check_Used(int x) { return Check_Used(x); }
			void Get_Mark_as_Used(int x) { Mark_as_Used(x); }
			void Set_Reset_Used() { Reset_Used(); }
			bool Get_Check_Crosing(int x, int y) { return Check_Crossing(x, y); }
			bool Get_Check_Crosing(int x, int y, int z) { return Check_Crossing(x, y, z); }
			void Set_Print_on_Board(int y, int z, int q) { Print_on_Board(y, z, q); }
			int Get_Random_Not_Used() { return Random_Not_Used(); }
		};

		class Inserts_Result_Board : public Result_Board
		{
		public:
			Inserts_Result_Board(Symbols &symb) : Result_Board(symb) {}
			int Get_Get_Symbol(Position &x) { return Get_Symbol(x); }
		};


		TEST_METHOD(_Add_Get_Line)
		{
			Symbols symb;
			Result_Board board(symb);
			Inserts_Lines lines(symb, board);
			lines.Add(1, 2, 3); // 0
			lines.Add(0, 1, 2, 3, 4); // 1
			lines.Add(0, 1, 2, 3, 4); // 2

			lines.Add(1, 2, 3); // 3
			lines.Add(1, 2, 3); // 4
			lines.Add(1, 2, 3); // 5
			lines.Add(1, 2, 3); // 6
			lines.Add(1, 2, 3); // 7
			lines.Add(1, 2, 3); // 8
			lines.Add(1, 2, 3); // 9
			lines.Add(1, 2, 3); // 10
			lines.Add(1, 2, 3); // 11
			lines.Add(1, 2, 3); // 12
			lines.Add(1, 2, 3); // 13
			lines.Add(1, 2, 3); // 14
			lines.Add(1, 2, 3); // 15
			lines.Add(1, 2, 3); // 16
			lines.Add(1, 2, 3); // 17
			lines.Add(1, 2, 3); // 18
			lines.Add(1, 2, 3); // 19
			lines.Add(1, 2, 3); // 20
			lines.Add(1, 2, 3); // 21
			lines.Add(1, 2, 3); // 22
			lines.Add(1, 2, 3); // 23
			lines.Add(1, 2, 3); // 24
			lines.Add(1, 2, 3); // 25
			lines.Add(1, 2, 3); // 26
			lines.Add(1, 2, 3); // 27
			lines.Add(1, 2, 3); // 28
			lines.Add(1, 2, 3); // 29
			lines.Add(1, 2, 3); // 30
			lines.Add(1, 2, 3); // 31
			lines.Add(1, 2, 3); // 32
			lines.Add(1, 2, 3); // 33
			lines.Add(1, 2, 3); // 34
			lines.Add(1, 2, 3); // 35
			lines.Add(1, 2, 3); // 36
			lines.Add(1, 2, 3); // 37
			lines.Add(1, 2, 3); // 38
			lines.Add(1, 2, 3); // 39
			lines.Add(1, 2, 3); // 40
			lines.Add(1, 2, 3); // 41
			lines.Add(1, 2, 3); // 42
			lines.Add(1, 2, 3); // 43
			lines.Add(1, 2, 3); // 44
			lines.Add(1, 2, 3); // 45
			lines.Add(1, 2, 3); // 46
			lines.Add(1, 2, 3); // 47
			lines.Add(1, 2, 3); // 48
			lines.Add(0, 1, 2, 3, 4); // 49

			Assert::AreEqual(50, lines.Get_line_num());

			Items test_line = lines.Get_Get(0);
			Assert::AreEqual(1, test_line.val[0]);
			Assert::AreEqual(3, test_line.val[2]);
			test_line = lines.Get_Get(2);
			Assert::AreEqual(0, test_line.val[0]);
			test_line = lines.Get_Get(49);
			Assert::AreEqual(0, test_line.val[0]);
			Assert::AreEqual(4, test_line.val[4]);
		}

		TEST_METHOD(_Useing)
		{
			Symbols symb;
			Result_Board board(symb);
			Inserts_Lines lines(symb, board);

			lines.Add(1, 1, 1, 1, 1); // 0
			lines.Add(2, 2, 2, 2, 2); // 1
			lines.Add(1, 2, 3); // 2

			Assert::AreEqual(false, lines.Get_Check_Used(0));
			lines.Get_Mark_as_Used(0);
			Assert::AreEqual(true, lines.Get_Check_Used(0));

			lines.Set_Reset_Used();
			Assert::AreEqual(false, lines.Get_Check_Used(0));
		}

		TEST_METHOD(_Check_Crossing)
		{
			Symbols symb;
			Result_Board board(symb);

			Items size;
			size.counter = 5;
			size.val[0] = 3;
			size.val[1] = 3;
			size.val[2] = 3;
			size.val[3] = 3;
			size.val[4] = 3;

			board.Set_Shape(size);

			Inserts_Lines lines(symb, board);

			lines.Add(1, 1, 1, 1, 1); // 0
			lines.Add(2, 2, 2, 2, 2); // 1
			lines.Add(1, 2, 3, 2, 1); // 2

			Assert::AreEqual(true, lines.Get_Check_Crosing(1, 1));
			Assert::AreEqual(false, lines.Get_Check_Crosing(0, 1));
			Assert::AreEqual(true, lines.Get_Check_Crosing(0, 2));
		}

		TEST_METHOD(_Check_Crossing_for_3)
		{
			Symbols symb;
			Result_Board board(symb);

			Items size;
			size.counter = 5;
			size.val[0] = 3;
			size.val[1] = 3;
			size.val[2] = 3;
			size.val[3] = 3;
			size.val[4] = 3;
			board.Set_Shape(size);

			Inserts_Lines lines(symb, board);

			lines.Add(1, 1, 1, 1, 1); // 0
			lines.Add(2, 2, 2, 2, 2); // 1
			lines.Add(3, 3, 3, 3, 3); // 2
			lines.Add(1, 2, 3, 2, 1); // 3

			Assert::AreEqual(false, lines.Get_Check_Crosing(0, 1, 2));
			Assert::AreEqual(true, lines.Get_Check_Crosing(0, 0, 0));
			Assert::AreEqual(true, lines.Get_Check_Crosing(0, 1, 3));
		}

		TEST_METHOD(_Print_on_Board)
		{

			Symbols symb;
			Result_Board board(symb);

			Items size;
			size.counter = 5;
			size.val[0] = 3;
			size.val[1] = 3;
			size.val[2] = 3;
			size.val[3] = 3;
			size.val[4] = 3;
			board.Set_Shape(size);

			Inserts_Lines lines(symb, board);
			lines.Add(1, 1, 1, 1, 1); // 0
			lines.Add(2, 2, 2, 2, 2); // 1
			lines.Add(0, 0, 0, 0, 0); // 2

			int length = 2; // poniewaz zostanie do niej dodane 3, a 5 to max dlugosc tablicy wynikowej
			lines.Set_Print_on_Board(1, 0, length);
			lines.Set_Print_on_Board(2, 1, length);
			lines.Set_Print_on_Board(3, 2, length);

			Position pos;
			pos.x = 0;
			pos.y = 0;
			int outcome;
			outcome = board.Get_Symbol(pos);

			Assert::AreEqual(3, outcome);

			pos.y = 1;
			outcome = board.Get_Symbol(pos);
			Assert::AreEqual(1, outcome);

			pos.y = 2;
			outcome = board.Get_Symbol(pos);
			Assert::AreEqual(2, outcome);
		}

		TEST_METHOD(_Random_Not_Used_Lines)
		{
			Symbols symb;
			Result_Board board(symb);
			Inserts_Lines lines(symb, board);

			lines.Add(1, 1, 1, 1, 1); // 0
			lines.Add(1, 2, 3, 2, 1); // 1
			lines.Add(1, 3, 2, 3, 1); // 2
			lines.Add(3, 2, 1, 2, 3); // 3
			lines.Add(1, 1, 2, 3, 3); // 4
			lines.Add(3, 3, 2, 1, 1); // 5
			int const LAST_LINE = 5;

			int used = 3;
			lines.Get_Mark_as_Used(used);

			int result;
			bool test = true;
			for (int repeat = 0; repeat < 100; ++repeat) // czy losuje nieuzyte z zakresu
			{
				result = lines.Get_Random_Not_Used();

				if (result == used) test = false;
				if (result > LAST_LINE) test = false;
			}

			Assert::AreEqual(true, test);
		}
	};
}