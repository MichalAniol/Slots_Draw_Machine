#pragma once
#include "Scenario_Holder.h"
#include "Multipler.h"

namespace Slots_Draw_Machine
{
	class Slot_Base
	{
#if TESTS == ON
	public:
		int drop_counter[5]; // licznik korekt prawdopodobienstwa w sytacjach nieuzyskania pozadanego wyniku
#else
	protected:
#endif
		Lines lines; // obiekty odpowiadajace za poszczegolne elementy slotu
		Symbols symb;
		Specials spec;
		Result_Board board;
		Slot_Parameter param;
		Lines_Designer design;
		Scenario_Holder scenario;
		Multipler multi;

		RTP rtp; // parametry indywidualne kazdego slota
		Score score;
		Shield shield;
		Mega_Wins mega_wins;

#if SHOW_READ_VAL == ON
		void Print_Scenario(Scenario scenario);
#endif

	protected:
		std::string line;       // zmienna do wczytywania parametrow
		std::fstream file;     // nazwa pliku z parametrami

		int spec_symb;       // symbol specjalny wplywajacy na obliczenia linii, np WILD

		bool win_ture;     // czy dana tura ma byc wygrana
		int what_scen;    // ktory aktualnie scenariusz jest realizowany, 0 = zaden
		bool knock_down; // flaga wymuszajaca losowania jakiejkolwiek linii, 
						// uzywana w sytuacji wystapienia rownoczesnie zdarzen:
					   // wyczerpanie sie kredytow,  wskaznik 'game.score.min = 0'
					  // i mala pula pozostalych prawdopodobienstw w obiektach 'Probability', 'Win' i 'Symbol'.
					 // bez tego wymuszenia w skrajnych przypadkach losowanie czasami moglo zajac nawet kilka sekud.
					// takie zdarzenie pojawia³o sie oko³o 1 raz na 50 - 100 mln losowan

		virtual void Read_File();
		void Find_this_Line(std::string &text);
		void Read_Line(int &value);
		void Read_Line(Items &val, int counter);
		void Read_Scenario(Scenario &scenario);
		void Set_Probability();

		int Calculate_Bet();
		bool Subtract_Bet(int bet_val);
		int Create_Slot();
		void No_Solution(int counter);

		void Tour_Summary(int rtp);
		int Calculate_RTP();

		virtual void Establish_Parameters();
		virtual void Execute_Scenario(); 
		virtual float Scenario_Multipler();
		virtual void Convert_Symbols();

	public:
		Slot_Base();

		int Draw();

		Game game;         // parametry gry dla wszystkich slotow
		int result[5][7]; // tablica wynikow
		int back[5][7];  // tablica przed pojawieniem sie znakow specjalnych
		bool back_on;   // flaga zapisu na tablicy 'back'
	};
}


