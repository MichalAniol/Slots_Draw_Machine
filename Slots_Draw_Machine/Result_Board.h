#pragma once
#include "Probability.h"
#include "Symbols.h"

namespace Slots_Draw_Machine
{
	class Result_Board
	{
		// tablica wynikow losowan
		// oraz procedury wyszukujace w tablicy
		// i wpisujace symbole w tablicy

#if TESTS == ON
	protected:
#else
	private:
#endif
		int length; // szerokosc slotu
		Items height; // wysokosci poszczegolnych ringow

		Exam exam; // wystepowanie symboli na tablicy wynikow

		Symbols *symb; // wskaznik do komunikowania sie z obiektem

		void Note_from_Posit(Position &pos);
		void Clear_Used_Symb();
		void Exam_Cells_Around(Position &pos);
		int Get_Rand_from_Exam();

	public:
		int result[5][7];     // tablica wynikow
		int backside[5][7];  // tablica przed pojawieniem sie znakow specjalnych
		bool backside_on;   // flaga zapisu na tablicy 'back'

		Result_Board(Symbols &symb);
		void Set_Shape(Items &size);

		void Clear();
		bool Fill_Up();

		// 'interface' do pobierania zmiennych z obiektu tej klasy
		bool On_Shape(Position &pos);
		int Get_Length();
		int Get_Height(int ring);
		int Get_Symbol(Position &pos);
		void Set_Symbol(Position &pos, int symbol);
	};
}


