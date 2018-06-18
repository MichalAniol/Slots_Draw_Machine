#pragma once
#include "Result_Board.h"

namespace Slots_Draw_Machine
{
	class Lines
	{
		// lista linii wystepujacych na danym slocie,
		// operacji sprawdzajacych
		// i losujacych w formie 'kwantowego prawdopodobienstwa'

#if TESTS == ON
	protected:
#else
	private:
#endif 
		Symbols *symb;               // wskaznik do komunikacji z obiektem
		Result_Board *board;        // wskaznik do komunikacji z obiektem
		Active active;             // lista wyszukanych linii na tablicy wynikow w celu obliczania rtp
		int num_lines;            // licznik elementow
		int shape[50][5];        // ksztalty
		bool used[50];          // tablica uzyc
		int left;              // licznik pozostalych do uzycia

		Items Get(int the_line);
		bool Check_Used(int the_line);
		void Mark_as_Used(int the_line);

		void Check_Length(int num_line, int look_for_symb, int special);

	public:
		Lines(Symbols &symb, Result_Board &board);
		void Add(int one, int two, int three);
		void Add(int one, int two, int three, int four, int five);

		bool Check_Crossing(int line_1st, int line_2nd);
		bool Check_Crossing(int line_1st, int line_2nd, int line_3rd);
		void Find_Lines(int special);

		int Random_Not_Used();
		void Reset_Used();

		void Print_on_Board(int symbol, int line, int length);

		// 'interface' do pobierania zmiennych z obiektu tej klasy
		int Get_Symb_Pos(int line, int ring);
		int Get_Num_Lines();
		int Get_Active_Num();
		int Get_Active_Symb(int counter);
		int Get_Active_Line(int counter);
		int Get_Active_Length(int counter);
		int Get_Left_Percent();
	};
}
