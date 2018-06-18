#include "stdafx.h"
#include "../Slots_Draw_Machine/Result_Board.cpp"
#include "../Slots_Draw_Machine/Specials.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Slots_Draw_Machine;

namespace Unit_Testy
{
	TEST_CLASS(_Result_Board)
	{
		class Inserts_Symbols : public Symbols
		{
		public:
			int Get_rtp(int x, int y) { return rtp[x][y]; }
			int Get_num_symb() { return size; }
			void Set_num_symb(int x) { size = x; }

			bool Get_used_percent(int x) { return used.percent[x]; }
			int Get_used_scope(int x) { return used.scope[x]; }
			int Get_used_left_all() { return used.all_left; }
			int  Get_used_left(int x) { return used.left[x]; }
			int Get_used_last(int x) { return used.recently_scope[x]; }
		};

		class Inserts_Result_Board : public Result_Board
		{
		public:
			Inserts_Result_Board(Symbols &symb) : Result_Board(symb) {}
			int Get_Get_Symbol(Position &x) { return Get_Symbol(x); }
			bool Get_On_Shape(Position &x) { return On_Shape(x); }
			void Set_Exam_Cels_Around(Position &x) { Exam_Cells_Around(x); }
			void Set_Clear_Used_Symb() { Clear_Used_Symb(); }

			int Get_exam_left() { return exam.left; }
			bool Get_exam_used(int x) { return exam.used[x]; }
		};

	public:
		TEST_METHOD(_Get_Set_Symbols)
		{
			Inserts_Symbols symb;
			Inserts_Result_Board board(symb);

			Items size;
			size.counter = 5;
			size.val[0] = 3;
			size.val[1] = 3;
			size.val[2] = 3;
			size.val[3] = 3;
			size.val[4] = 3;
			board.Set_Shape(size);

			Assert::AreEqual(5, board.Get_Length());

			Position pos;
			pos.x = 0; pos.y = 0;
			Assert::AreEqual(0, board.Get_Get_Symbol(pos)); // czy pierwszy element zerowy

			pos.x = 4; pos.y = 2;
			Assert::AreEqual(0, board.Get_Get_Symbol(pos)); // czy ostatni element zerowy

			int symbol = 1;
			board.Set_Symbol(pos, symbol);
			Assert::AreEqual(1, board.Get_Get_Symbol(pos));

			Assert::AreEqual(true, board.Get_On_Shape(pos));

			pos.x = 8; pos.y = 2;
			Assert::AreEqual(false, board.Get_On_Shape(pos));

			pos.x = 0; pos.y = 7;
			Assert::AreEqual(false, board.Get_On_Shape(pos));
		}

		TEST_METHOD(_Exam_Cels_Around)
		{
			Inserts_Symbols symb;
			symb.Add_RTP(1, 2, 3); // 1
			symb.Add_RTP(4, 5, 6); // 2
			symb.Add_RTP(7, 8, 9); // 3
			symb.Add_RTP(10, 20, 30); // 4
			symb.Add_RTP(1, 2, 3); // 5
			symb.Add_RTP(1, 2, 3); // 6
			symb.Add_RTP(1, 2, 3); // 7
			symb.Add_RTP(1, 2, 3); // 8
			symb.Add_RTP(1, 2, 3); // 9
			symb.Add_RTP(1, 2, 3); // 10

			Inserts_Result_Board board(symb);

			Items size;
			size.counter = 5;
			size.val[0] = 3;
			size.val[1] = 3;
			size.val[2] = 3;
			size.val[3] = 3;
			size.val[4] = 3;
			board.Set_Shape(size);

			board.Set_Clear_Used_Symb();

			Assert::AreEqual(10, board.Get_exam_left()); // ilosc pozostalych

			Position pos;

			int symbol = 1;
			pos.x = 0; pos.y = 0;
			board.Set_Symbol(pos, symbol);

			symbol = 2;
			pos.x = 1; pos.y = 0;
			board.Set_Symbol(pos, symbol);

			symbol = 3;
			pos.x = 2; pos.y = 0;
			board.Set_Symbol(pos, symbol);

			symbol = 4;
			pos.x = 0; pos.y = 1;
			board.Set_Symbol(pos, symbol);

			symbol = 5;
			pos.x = 2; pos.y = 1;
			board.Set_Symbol(pos, symbol);

			symbol = 6;
			pos.x = 0; pos.y = 2;
			board.Set_Symbol(pos, symbol);

			symbol = 7;
			pos.x = 1; pos.y = 2;
			board.Set_Symbol(pos, symbol);

			symbol = 8;
			pos.x = 2; pos.y = 2;
			board.Set_Symbol(pos, symbol);

			pos.x = 1; pos.y = 1;
			board.Set_Exam_Cels_Around(pos);
			Assert::AreEqual(2, board.Get_exam_left()); // ilosc pozostalych

			bool const patern[10] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 0 };
			for (int i = 0; i < 10; ++i)
			{
				Assert::AreEqual(patern[i], board.Get_exam_used(i));
			}

			pos.x = 0; pos.y = 0;
			board.Set_Exam_Cels_Around(pos);
			Assert::AreEqual(6, board.Get_exam_left()); // ilosc pozostalych*/

			bool const patern2[10] = { 1, 0, 1, 0, 1, 0, 0, 1, 0, 0 };
			for (int i = 0; i < 10; ++i)
			{
				Assert::AreEqual(patern2[i], board.Get_exam_used(i));
			}
		}
	};
}