#include "stdafx.h"
#include "../Slots_Draw_Machine/Symbols.cpp"
#include "../Slots_Draw_Machine/Result_Board.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Slots_Draw_Machine;

namespace Unit_Testy
{
	TEST_CLASS(_Symbols)
	{
	public:
		class Inserts_Symbols : public Symbols
		{
		public:
			int Get_Rand_Not_Used() { return Rand_Percent(); }
			int Get_rtp(int x, int y) { return rtp[x][y]; }
			int Get_num_symb() { return size; }
			void Set_num_symb(int x) { size = x; }
			void Get_Change_to_Percents(float x[], float y) { Change_to_Percents(x, y); }

			bool Get_used_percent(int x) { return used.percent[x]; }
			int Get_used_scope(int x) { return used.scope[x]; }
			int Get_used_left_all() { return used.all_left; }
			int  Get_used_left(int x) { return used.left[x]; }
			int Get_used_recently_scope(int x) { return used.recently_scope[x]; }
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


		TEST_METHOD(_Add_GetRTP_Symbols)
		{
			Inserts_Symbols symb;

			symb.Add_RTP(1, 2, 3); // 1
			Assert::AreEqual(1, symb.Get_rtp(0, 0)); // czy dodano
			Assert::AreEqual(2, symb.Get_rtp(0, 1));
			Assert::AreEqual(3, symb.Get_rtp(0, 2));

			symb.Add_RTP(1, 2, 3); // 2
			Assert::AreEqual(1, symb.Get_rtp(1, 0));
			Assert::AreEqual(2, symb.Get_rtp(1, 1));
			Assert::AreEqual(3, symb.Get_rtp(1, 2));

			symb.Add_RTP(1); // 3
			symb.Add_RTP(1); // 4
			symb.Add_RTP(1); // 5
			symb.Add_RTP(1); // 6
			symb.Add_RTP(1); // 7
			symb.Add_RTP(1); // 8
			symb.Add_RTP(1); // 9
			symb.Add_RTP(1); // 10
			symb.Add_RTP(1); // 11
			symb.Add_RTP(1); // 12
			symb.Add_RTP(1); // 13
			symb.Add_RTP(1); // 14
			symb.Add_RTP(1); // 15
			symb.Add_RTP(1); // 16
			symb.Add_RTP(1); // 17
			symb.Add_RTP(1); // 18
			symb.Add_RTP(1); // 19
			symb.Add_RTP(1); // 20

			Assert::AreEqual(20, symb.Get_num_symb()); // czy sumy sie zgadzja

			int counter = 1, line_lenght = 2;
			Assert::AreEqual(3, symb.Get_RTP(counter, line_lenght)); // czy podaje prawidlowe rtp
			line_lenght = 0;
			Assert::AreEqual(1, symb.Get_RTP(counter, line_lenght));
		}

		TEST_METHOD(_Add_Exam_Symbols)
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

		TEST_METHOD(_Spread)
		{
			Inserts_Symbols symb;

			symb.Add_RTP( 30, 60, 140 ); // Cat
			symb.Add_RTP( 20, 50, 100 ); // Nile
			symb.Add_RTP( 15, 40, 80 ); // Oasis
			symb.Add_RTP( 15, 35, 70 ); // Regalia
			symb.Add_RTP( 10, 30, 50 ); // Urn
			symb.Add_RTP( 10, 25, 40 ); // A
			symb.Add_RTP( 8, 16, 30 ); // K
			symb.Add_RTP( 5, 10, 20 );  // Q

			symb.Set_Spread(150);

			Assert::AreEqual(0, symb.Get_used_scope(0)); // sprawdza progi podane w procentach
			Assert::AreEqual(2, symb.Get_used_scope(1)); 
			Assert::AreEqual(6, symb.Get_used_scope(2)); 
			Assert::AreEqual(11, symb.Get_used_scope(3)); 
			Assert::AreEqual(18, symb.Get_used_scope(4)); 
			Assert::AreEqual(27, symb.Get_used_scope(5)); 
			Assert::AreEqual(40, symb.Get_used_scope(6));
			Assert::AreEqual(60, symb.Get_used_scope(7)); 
			Assert::AreEqual(100, symb.Get_used_scope(8));

			Assert::AreEqual(0, symb.Get_used_left(0)); // sprawdza wartosci pomiedzy progami
			Assert::AreEqual(2, symb.Get_used_left(1));
			Assert::AreEqual(4, symb.Get_used_left(2));
			Assert::AreEqual(5, symb.Get_used_left(3));
			Assert::AreEqual(7, symb.Get_used_left(4));
			Assert::AreEqual(9, symb.Get_used_left(5));
			Assert::AreEqual(13, symb.Get_used_left(6));
			Assert::AreEqual(20, symb.Get_used_left(7));
			Assert::AreEqual(40, symb.Get_used_left(8));

			Assert::AreEqual(100, symb.Get_used_left_all()); // zliczona calosc

			int memo[9];
			for (int i = 0; i < ARRAY_LENGTH(memo); ++i)
			{
				memo[i] = symb.Get_used_left(i);
			}

			int result;
			result = symb.Get_Random();
			symb.Mark_Used();

			Assert::AreEqual(99, symb.Get_used_left_all()); // zliczona calosc
			//Assert::AreEqual(memo[result] - 1, symb.Get_used_left(result)); // wnik dla roznic miedzy progami

			//Assert::AreEqual(result, symb.Get_used_recently_scope(0)); // zapis poprzednich losowan

			//symb.Get_Random();
			//symb.Mark_Use();
			//Assert::AreEqual(result, symb.Get_used_recently_scope(1)); // zapis poprzednich losowan

			//symb.Get_Random();
			//symb.Mark_Use();
			//Assert::AreEqual(result, symb.Get_used_recently_scope(2)); // zapis poprzednich losowan

			//symb.Set_Spread(150);
			//Assert::AreEqual(100, symb.Get_used_left_all()); // zliczona calosc
		}

		TEST_METHOD(_Rand_Not_Used)
		{
			Inserts_Symbols symb;
			symb.Add_RTP(30, 60, 140); // Cat
			symb.Add_RTP(20, 50, 100); // Nile
			symb.Add_RTP(15, 40, 80); // Oasis
			symb.Add_RTP(15, 35, 70); // Regalia
			symb.Add_RTP(10, 30, 50); // Urn
			symb.Add_RTP(10, 25, 40); // A
			symb.Add_RTP(8, 16, 30); // K
			symb.Add_RTP(5, 10, 20);  // Q

			symb.Set_Spread(150);

			Assert::AreEqual(100, symb.Get_used_left_all()); // zliczona calosc

			for (int i = 0; i < 10; ++i)
			{
				symb.Get_Random();
				symb.Mark_Used();
			}

			Assert::AreEqual(90, symb.Get_used_left_all()); // zliczona calosc
		}
	};
}