#include "stdafx.h"
#include "../Slots_Draw_Machine/Lines_Designer.cpp"
#include "../Slots_Draw_Machine/Slot_01.cpp"
#include "../Slots_Draw_Machine/Data_Holders.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Slots_Draw_Machine;

namespace Unit_Testy
{
	TEST_CLASS(_RTP_Designer)
	{
	public:
		class Inserts_RTP_Designer : public Lines_Designer
		{
		public:
			Inserts_RTP_Designer(
				Lines &lines, 
				Symbols &symb, 
				Result_Board &board, 
				Slot_Parameter &param, 
				Game &game, 
				Score &score,
				Shield &shield,
				Mega_Wins &mega_wins) :
				Lines_Designer(lines, symb, board, param, game, score, shield, mega_wins) {}
			int Get_mem_lines(int x) { return mem_line[x]; }
			int Get_Nearest_Not_Crosing(int x, int y) { return Nearest_No_Cross(x, y); }
			Items Get_Nearest_Not_Crosing(int x, int y, int z) { return Nearest_No_Cross(x, y, z); }
			void Set_Design_Crosing(int x) { Design_Cross(x); };
		};


		TEST_METHOD(_Design_Crosing_Lines)
		{
			Symbols symb;
			Result_Board board(symb);
			Slot_Parameter param;
			Game game;
			Score score;
			Shield shield;
			Mega_Wins mega_wins;

			Lines lines(symb, board);
			lines.Add(1, 1, 1, 1, 1); // 0
			lines.Add(1, 2, 3, 2, 1); // 1
			lines.Add(1, 3, 2, 3, 1); // 2
			lines.Add(3, 2, 1, 2, 3); // 3
			lines.Add(1, 1, 2, 3, 3); // 4
			lines.Add(3, 3, 2, 1, 1); // 5
			int const LAST_LINE = 5;

			Inserts_RTP_Designer lindes(lines, symb, board, param, game, score, shield, mega_wins);

			int lenght;
			int const LAST_MEM = 5;

			bool test = true;
			for (int repeat = 0; repeat < 100; ++repeat) // ilosc sprawdzen
			{
				lenght = (rand() % (LAST_MEM - 2)) + 2; // zakres 0d 2 do 5

				lindes.Set_Design_Crosing(lenght);

				for (int counter = 1; counter < lenght; ++counter)
				{
					if (lindes.Get_mem_lines(counter - 1) == lindes.Get_mem_lines(counter)) test = false; // czy losuje rozne
				}

				Assert::AreEqual(true, test);
			}
		}

		TEST_METHOD(_Design_No_Cross_Lines)
		{
			Symbols symb;
			Result_Board board(symb);
			Slot_Parameter param;
			Game game;
			Score score;
			Shield shield;
			Mega_Wins mega_wins;

			Lines lines(symb, board);
			lines.Add(1, 1, 1, 1, 1); // 0
			lines.Add(1, 2, 3, 2, 1); // 1
			lines.Add(1, 3, 2, 3, 1); // 2
			lines.Add(3, 2, 1, 2, 3); // 3
			lines.Add(1, 1, 2, 3, 3); // 4
			lines.Add(3, 3, 2, 1, 1); // 5
			int const LAST_LINE = 5;

			Inserts_RTP_Designer lindes(lines, symb, board, param, game, score, shield, mega_wins);

			int lenght;
			int const LAST_MEM = 3;

			bool test = true;
			for (int repeat = 0; repeat < 100; ++repeat) // ilosc sprawdzen
			{
				lenght = (rand() % (LAST_MEM - 2)) + 2; // zakres 0d 2 do 3

				lindes.Set_Design_Crosing(lenght);

				for (int counter = 1; counter < lenght; ++counter)
				{
					if (lindes.Get_mem_lines(counter - 1) == lindes.Get_mem_lines(counter)) test = false; // czy losuje rozne
				}

				Assert::AreEqual(true, test);
			}
		}

		TEST_METHOD(_Nearest_Not_Crosing_Line)
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

			Slot_Parameter param;
			Game game;
			Score score;
			Shield shield;
			Mega_Wins mega_wins;

			Lines lines(symb, board);
			lines.Add(1, 1, 1, 1, 1); // 0
			lines.Add(1, 2, 3, 2, 1); // 1
			lines.Add(1, 3, 2, 3, 1); // 2
			lines.Add(3, 2, 1, 2, 3); // 3
			lines.Add(1, 1, 2, 3, 3); // 4
			lines.Add(3, 3, 2, 1, 1); // 5
			int const LAST_LINE = 5;

			Inserts_RTP_Designer lindes(lines, symb, board, param, game, score, shield, mega_wins);

			Assert::AreEqual(-1, lindes.Get_Nearest_Not_Crosing(0, 1)); // brak linii niepoktywajacych siê

			lines.Add(2, 2, 2, 2, 2); // 6
			Assert::AreEqual(6, lindes.Get_Nearest_Not_Crosing(0, 1));

			lines.Add(3, 3, 3, 3, 3); // 7
			Assert::AreEqual(7, lindes.Get_Nearest_Not_Crosing(6, 7));
			Assert::AreEqual(0, lindes.Get_Nearest_Not_Crosing(6, 1));

			lines.Add(3, 3, 3, 3, 3); // 8
			lines.Add(3, 3, 3, 3, 3); // 9
			lines.Add(3, 3, 3, 3, 3); // 10
			lines.Add(3, 3, 3, 3, 3); // 11
			Assert::AreEqual(0, lindes.Get_Nearest_Not_Crosing(7, 11));
		}

		TEST_METHOD(_Nearest_Not_Crosing_3_Lines)
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

			Slot_Parameter param;
			Game game;
			Score score;
			Shield shield;
			Mega_Wins mega_wins;

			Lines lines(symb, board);
			lines.Add(1, 1, 1, 1, 1); // 0
			lines.Add(1, 2, 3, 2, 1); // 1
			lines.Add(1, 3, 2, 3, 1); // 2
			lines.Add(3, 2, 1, 2, 3); // 3
			lines.Add(1, 1, 2, 3, 3); // 4
			lines.Add(3, 3, 2, 1, 1); // 5
			int const LAST_LINE = 5;

			Inserts_RTP_Designer lindes(lines, symb, board, param, game, score, shield, mega_wins);

			Items result;
			result = lindes.Get_Nearest_Not_Crosing(0, 1, 2);
			Assert::AreEqual(-1, result.val[0]); // brak linii niepoktywajacych siê

			lines.Add(2, 2, 2, 2, 2); // 6
			lines.Add(3, 3, 3, 3, 3); // 7

			result = lindes.Get_Nearest_Not_Crosing(0, 1, 2);
			Assert::AreEqual(0, result.val[0]);
			Assert::AreEqual(6, result.val[1]);
			Assert::AreEqual(7, result.val[2]);
		}
	};
}
