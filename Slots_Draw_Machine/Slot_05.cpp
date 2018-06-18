#include "stdafx.h"
#include "Slot_05.h"

namespace Slots_Draw_Machine
{
	Slot_05::Slot_05()
	{
		Establish_Parameters();    // wczytanie linii i symboli

		Items dimensions; // obiekt do przeslania ksztaltu slotu
		int const SLOT_WIDE = 5;
		dimensions.counter = SLOT_WIDE; // wprowadzanie danych: szerokosc ringu
		dimensions.val[0] = 3; // wysokosci ringow
		dimensions.val[1] = 3;
		dimensions.val[2] = 3;
		dimensions.val[3] = 3;
		dimensions.val[4] = 3;
		board.Set_Shape(dimensions); // nadawanie ksztaltu slotowi w tablicy wynikow 

		file.open("Parameters.cfg", std::ios::in);  // wczytanie parametrow z pliku konfiguracyjnego
#if TESTS == ON
		if (file.good() == false)
		{
			file.open("../../Slots_Draw_Machine/Parameters.cfg", std::ios::in);  // wczytanie parametrow z pliku konfiguracyjnego
		}
#endif
		if (file.good() == true)
		{
			Read_File();

			file.close();
		}
		multi.average_bet = 4.35f;                              // przecietna wartosc mnoznika 

		scenario.Add_Scenario(freespin_3_start, freespin_3); // dodanie scenariuszy do sceanrio holder
		scenario.Add_Scenario(bonusgame_3_start, bonusgame_3);
		scenario.Add_Scenario(freespin_1or2_start, freespin_1or2);
		scenario.Add_Scenario(bonusgame_1or2_start, bonusgame_1or2);
		scenario.Add_Scenario(wadjet_start, wadjet);
		scenario.Activation(game.slot_all_games); // uruchomienie scenariuszy

		int const START_POSITION = 0;
		param.cycle.counter = START_POSITION;  // zerowanie licznika cykli
		game.free_spins = START_POSITION;     // zerowanie licznika freespinow
		score.counter = START_POSITION;      // zerowanoie licznika zmiany 'score goal'
		param.cycle.raise_on = false;       // opuszczenie flagi cyklu specjalnego

		rtp.multip.go_up = float(100 + param.cycle.raise) / 100; // zamiana procentow na mnoznik
		rtp.multip.go_down = float(100 - param.cycle.reduce) / 100;

		Set_Probability();          // przeslanie wczytanych danych do obiektow tworzacych prawdopodobienstwo
	}

	void Slot_05::Establish_Parameters()
	{
		// wprowadzenie danych do list symboli i linii

		int const SYMB_RTP[9][3] = { // wczytywanie rtp symboli
		{ 50, 100, 500 }, // Menes  (1)
		{ 40, 80, 400 },  // Iryhor (2)
		{ 30, 60, 300 },  // Amon   (3)
		{ 20, 50, 250 },  // Falcon (4)
		{ 15, 40, 200 },  // Eye    (5)
		{ 12, 30, 150 },  // Ship   (6)
		{ 10, 20, 100 },  // Sun    (7)
		{ 5, 10, 50 },    // Ring   (8)
		{ 2, 5, 25 } };   // Wadjet (9)
		int const FIRST_SYMBOL = 0;
		for (int i = FIRST_SYMBOL; i < ARRAY_LENGTH(SYMB_RTP); ++i)
		{
			symb.Add_RTP(SYMB_RTP[i][0], SYMB_RTP[i][1], SYMB_RTP[i][2]);
		}

		int const LINE[25][5] = { // wczytywanie ksztaltu linii
		{ 1, 1, 1, 1, 1 },   //1
		{ 0, 0, 0, 0, 0 },   //2
		{ 2, 2, 2, 2, 2 },   //3
		{ 0, 1, 2, 1, 0 },   //4
		{ 2, 1, 0, 1, 2 },   //5
		{ 1, 0, 0, 0, 1 },   //6
		{ 1, 2, 2, 2, 1 },   //7
		{ 0, 0, 1, 2, 2 },   //8
		{ 2, 2, 1, 0, 0 },   //9
		{ 1, 2, 1, 0, 1 },   //10
		{ 1, 0, 1, 2, 1 },   //11
		{ 0, 1, 1, 1, 0 },   //12
		{ 2, 1, 1, 1, 2 },   //13
		{ 0, 1, 0, 1, 0 },   //14
		{ 2, 1, 2, 1, 2 },   //15
		{ 1, 1, 0, 1, 1 },   //16
		{ 1, 1, 2, 1, 1 },   //17
		{ 0, 0, 2, 0, 0 },   //18
		{ 2, 2, 0, 2, 2 },   //19
		{ 0, 2, 2, 2, 0 },   //20
		{ 2, 0, 0, 0, 2 },   //21
		{ 1, 2, 0, 2, 1 },   //22
		{ 1, 0, 2, 0, 1 },   //23
		{ 0, 2, 0, 2, 0 },   //24
		{ 2, 0, 2, 0, 2 } }; //25
		int const FIRST_LINE = 0;
		for (int i = FIRST_LINE; i < ARRAY_LENGTH(LINE); ++i)
		{
			lines.Add(LINE[i][0], LINE[i][1], LINE[i][2], LINE[i][3], LINE[i][4]);
		}
	}

	void Slot_05::Read_File()
	{
		// wczytanie parametrow slotu z pliku txt

		std::string slot_data_begin = "------------  SLOT 05  --------------";
		Find_this_Line(slot_data_begin);

		Read_Line(param.cycle.win);
		Read_Line(param.bet.num);
		Read_Line(param.exp.open_lev);
		Read_Line(param.exp.for_next_lev);
		Read_Line(param.exp.for_play);
		Read_Line(param.exp.for_bet, param.bet.num);
		Read_Line(param.bet.value, param.bet.num);
		Read_Line(param.bet.choose);
		Read_Line(param.cycle.everyday_start);

		int const FIRST_ITEM = 0;
		for (int i = FIRST_ITEM; i < param.bet.num; ++i)
		{
			int const NUM_ITEM = 3;
			Read_Line(param.bet.x_length[i], NUM_ITEM);
		}

		Read_Line(param.num_not_cover, 3);
		Read_Line(param.num_cover, 5);
		Read_Line(param.lines_cover);
		Read_Line(param.sybols_spread);

		Read_Line(param.cycle.raise);
		Read_Line(param.cycle.reduce);

		Read_Scenario(freespin_1or2_start);
		Read_Scenario(freespin_1or2);
		Read_Scenario(freespin_3_start);
		Read_Scenario(freespin_3);

		Read_Scenario(bonusgame_1or2_start);
		Read_Scenario(bonusgame_1or2);
		Read_Scenario(bonusgame_3_start);
		Read_Scenario(bonusgame_3);

		Read_Scenario(wadjet_start);
		Read_Scenario(wadjet);

#if SHOW_READ_VAL == ON
		// wypisuje wczytane dane na konsoli w celu sprawdzenia, czy dane zostaly poprawnie wczytane

		std::cout << "------------  SLOT 05  --------------" << std::endl;
		std::cout << param.cycle.win << std::endl;
		std::cout << param.bet.num << std::endl;
		std::cout << param.exp.open_lev << std::endl;
		std::cout << param.exp.for_next_lev << std::endl;
		std::cout << param.exp.for_play << std::endl;

		for (int i = 0; i < param.bet.num; i++) std::cout << param.exp.for_bet.val[i] << " "; std::cout << std::endl;
		for (int i = 0; i < param.bet.num; i++) std::cout << param.bet.value.val[i] << " "; std::cout << std::endl;

		std::cout << param.bet.choose << std::endl;
		std::cout << param.cycle.everyday_start << std::endl;

		for (int i = FIRST_ITEM; i < param.bet.num; ++i)
			std::cout << param.bet.x_length[i].val[0] << " " << param.bet.x_length[i].val[1] << " " << param.bet.x_length[i].val[2] << std::endl;

		std::cout << param.num_not_cover.val[0] << " " << param.num_not_cover.val[1] << " " << param.num_not_cover.val[2] << std::endl;
		std::cout << param.num_cover.val[0] << " " << param.num_cover.val[1] << " " << param.num_cover.val[2] << " "
			<< param.num_cover.val[3] << " " << param.num_cover.val[4] << std::endl;
		std::cout << param.lines_cover << std::endl;
		std::cout << param.sybols_spread << std::endl;
		std::cout << param.cycle.raise << std::endl;
		std::cout << param.cycle.reduce << std::endl;

		Print_Scenario(freespin_1or2_start);
		Print_Scenario(freespin_1or2);
		Print_Scenario(freespin_3_start);
		Print_Scenario(freespin_3);

		Print_Scenario(bonusgame_1or2_start);
		Print_Scenario(bonusgame_1or2);
		Print_Scenario(bonusgame_3_start);
		Print_Scenario(bonusgame_3);

		Print_Scenario(wadjet_start);
		Print_Scenario(wadjet);
#endif
	}

	void Slot_05::Execute_Scenario()
	{
		// wrysowanie scenariuszy na tablicy wynikow

		int const FREE_SPINS = 10; // numer pod ktorym wystepuje symbol 'free spins' na tablicy wynikow
		int const BONUS_GAME = 11; // numer pod ktorym wystepuje symbol 'bonus game' na tablicy wynikow

		int const WADJET = 9; // z tabeli symboli

		switch (what_scen)
		{
			case 1: // freespin_3
			{
				int const THREE_ITEMS = 3;
				spec.Free_Spins(THREE_ITEMS, FREE_SPINS);

			}; break;

			case 2: // bonusgame_3
			{
				int const THREE_ITEMS = 3;
				spec.Bonus_Game(THREE_ITEMS, BONUS_GAME);

			}; break;

			case 3: // freespin_1or2
			{
				int one_or_two;
				one_or_two = (rand() % 1) + 1;
				spec.Free_Spins(one_or_two, FREE_SPINS);

			}; break;

			case 4: // bonusgame_1or2
			{
				int one_or_two;
				one_or_two = (rand() % 1) + 1;
				spec.Bonus_Game(one_or_two, BONUS_GAME);

			}; break;

			case 5: // bonusgame_1or2
			{
				spec.Wadjet(WADJET);

			}; break;
		}
	}

	float Slot_05::Scenario_Multipler()
	{
		// ustala mnoznik w momencie wystapienia scenariusza

		int const WADJET_SCENARIO = 5;
		if (what_scen == WADJET_SCENARIO)
		{
			float const WADJET_VAL = 1.6f;
			return WADJET_VAL;
		}

		float const DEFALUT_VAL = 1;
		return DEFALUT_VAL; 
	}

	void Slot_05::Convert_Symbols()
	{
		// dla ulatwienia obliczen przyjalem inna numeracje symboli
		// ta funkcja zamienia symbole na tablicy wynikow na wlasciwe dla gry
		// i zapisuje je na tablicy 

		int const START_BOARD = 0;
		int const END_X_BOARD = ARRAY_LENGTH(board.result);
		int const END_Y_BOARD = ARRAY_SECOND(board.result);

		for (int y = START_BOARD; y < END_Y_BOARD; ++y) // symbole na frontowej tablicy wynikow 
		{
			for (int x = START_BOARD; x < END_X_BOARD; ++x)
			{
				int symbol = board.result[x][y];

				switch (symbol)
				{ 
				case 1: result[x][y] = 8; break;   // Menes 
				case 2: result[x][y] = 7; break;   // Iryhor 
				case 3: result[x][y] = 6; break;   // Amon
				case 4: result[x][y] = 5; break;   // Falcon
				case 5: result[x][y] = 4; break;   // Eye  
				case 6: result[x][y] = 3; break;   // Ship
				case 7: result[x][y] = 2; break;   // Sun
				case 8: result[x][y] = 1; break;   // Ring 
				case 9: result[x][y] = 0; break;   // Wadjet
				case 10: result[x][y] = 9; break;  // Residence (Bonus Game)
				case 11: result[x][y] = 10; break; // Ram (Free Spins)

				default: result[x][y] = -1; break; // empty
				}
			}
		}
		int const WILD = 10; // numer pod ktorym wystepuje symbol 'wild' na tablicy wynikow
		int const MYSTERY = 11; // numer pod ktorym wystepuje symbol 'free spins' na tablicy wynikow
		int const BONUS_GAME = 12; // numer pod ktorym wystepuje symbol 'bonus game' na tablicy wynikow

		back_on = board.backside_on; // czy tylna tablica wynikow jest aktywna

		if (back_on == true)
		{
			for (int y = START_BOARD; y < END_Y_BOARD; ++y) // symbole na tylnej tablicy wynikow
			{
				for (int x = START_BOARD; x < END_X_BOARD; ++x)
				{
					int symbol = board.backside[x][y];

					switch (symbol)
					{
					case 1: back[x][y] = 8; break;   // Menes 
					case 2: back[x][y] = 7; break;   // Iryhor 
					case 3: back[x][y] = 6; break;   // Amon
					case 4: back[x][y] = 5; break;   // Falcon
					case 5: back[x][y] = 4; break;   // Eye  
					case 6: back[x][y] = 3; break;   // Ship
					case 7: back[x][y] = 2; break;   // Sun
					case 8: back[x][y] = 1; break;   // Ring 
					case 9: back[x][y] = 0; break;   // Wadjet
					case 10: back[x][y] = 9; break;  // Residence (Bonus Game)
					case 11: back[x][y] = 10; break; // Ram (Free Spins)

					default: back[x][y] = -1; break; // empty
					}
				}
			}
		}
		else // jesli tylna tablica jest nieaktywna
		{
			for (int y = START_BOARD; y < END_Y_BOARD; ++y) // to ja wyczysc
			{
				for (int x = START_BOARD; x < END_X_BOARD; ++x)
				{
					int const EMPTY = -1;
					back[x][y] = EMPTY;
				}
			}
		}
	}
}