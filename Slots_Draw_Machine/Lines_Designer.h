#pragma once
#include "Lines.h"
#include "Win_Cycle.h"

namespace Slots_Draw_Machine
{
	class Lines_Designer
	{
		// projektuje odpowiednia ilosc linii dla zadanego zakresu rtp
		// na podstawie obiektow generujacych prawdopodobienstwa
		// oraz wrysowanie projektu na tablice wynikow

#if TESTS == ON
	protected:
#else
	private:
#endif
		int const LAST_MEM;
		int rtp_for_tour;    // suma rtp * aktualny zaklad

		Win_Cycle win;      // obiekty okreslajace prawdopodobienstwo wygranej w danej turze
		Probability cover; // prawdopodobienstwa zdarzen 
		Probability num_cover;
		Probability num_not_cover;
		Probability line_length;

		Lines *lines; // wskazniki do komunikacji z innymi obiektami w tym slocie
		Symbols *symb;
		Result_Board *board;
		Slot_Parameter *param;
		Game *game;
		Score *score;
		Shield *shield;
		Mega_Wins *mega_wins;

		int Nearest_No_Cross(int base_line, int search_around);
		Items Nearest_No_Cross(int line_1st, int line_2nd, int line_3rd);

		void Design_No_Cross(int counter);
		bool Find_No_Cross(int min_rtp, int max_rtp);

		void Design_Cross(int counter);
		bool Find_Cross(int min_rtp, int max_rtp);

	public:
		Mem mem;

		Lines_Designer(
			Lines &lines, // przeslanie wskaznikow
			Symbols &symb,
			Result_Board &board,
			Slot_Parameter &param,
			Game &game,
			Score &score,
			Shield &shield,
			Mega_Wins &mega_wins
		);

		void Change_Probability(
			int win,
			int cover,
			Items &num_cover,
			Items &num_not_cover,
			Items &line_length);

		void Draw(int min_rtp, int max_rtp);
		void Execute();

		void Draw_Any();

		void Draw_No_Hits();
		void Execute_No_Hits(int line, int symb);

		void Mark_Use_All_Probabilities();

		void Reset_Least();
		void Restart_All();

		// 'interface' do komunikacji z obiektami w tej klasie
		bool Get_Win_Check();
		void Set_Win_Mark_Used();
		int Get_Win_Next_Size();
		void Set_Win_Next_Size(int size);
		void Change_Win_Size();
	};
}
