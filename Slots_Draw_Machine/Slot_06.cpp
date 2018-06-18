#include "stdafx.h"
#include "Slot_06.h"

namespace Slots_Draw_Machine
{
	Slot_06::Slot_06()
	{
		Establish_Parameters();    // wczytanie linii i symboli

		Items dimensions; // obiekt do przeslania ksztaltu slotu
		int const SLOT_WIDE = 5;
		dimensions.counter = SLOT_WIDE; // wprowadzanie danych: szerokosc ringu
		dimensions.val[0] = 3; // wysokosci ringow
		dimensions.val[1] = 4;
		dimensions.val[2] = 5;
		dimensions.val[3] = 4;
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

		multi.average_bet = 3.35f;                       // przecietna wartosc mnoznika 

		scenario.Add_Scenario(freespin_3_start, freespin_3); // dodanie scenariuszy do sceanrio holder
		scenario.Add_Scenario(wild_start, wild);       
		scenario.Add_Scenario(freespin_1or2_start, freespin_1or2);
		scenario.Activation(game.slot_all_games); // uruchomienie scenariuszy

		int const START_POSITION = 0;
		param.cycle.counter = START_POSITION;  // zerowanie licznika cykli
		game.free_spins = START_POSITION;     // zerowanie licznika freespinow
		score.counter = START_POSITION;      // zerowanoie licznika zmiany 'score goal'
		param.cycle.raise_on = false;       // opuszczenie flagi cyklu specjalnego

		rtp.multip.go_up = float(100 + param.cycle.raise) / 100; // zamiana procentow na mnoznik
		rtp.multip.go_down = float(100 - param.cycle.reduce) / 100;

		int const WILD = 11;          // numer pod ktorym wystepuje symbol 'wild' na tablicy wynikow
		spec_symb = WILD;            // przeslanie znaku specjalnego

		Set_Probability();         // przeslanie wczytanych danych do obiektow tworzacych prawdopodobienstwo
	}

	void Slot_06::Establish_Parameters()
	{
		// wprowadzenie danych do list symboli i linii

		int const SYMB_RTP[9][3] = { // wczytywanie rtp symboli
		{ 25, 30, 60 }, // Maat
		{ 20, 25, 40 }, // Hathor
		{ 12, 16, 25 }, // Khafre
		{ 10, 12, 20 }, // Scorpion
		{ 8, 10, 20 },  // Jackal
		{ 7, 8, 18 },   // Sun
		{ 5, 6, 14 },   // 777
		{ 4, 5, 10 },   // 77
		{ 3, 5, 8 } };  // 7
		int const FIRST_SYMBOL = 0;
		for (int i = FIRST_SYMBOL; i < ARRAY_LENGTH(SYMB_RTP); ++i)
		{
			symb.Add_RTP(SYMB_RTP[i][0], SYMB_RTP[i][1], SYMB_RTP[i][2]);
		}

		int const RTP_FOR_LINE_ASSEMBLED_ONLY_WILD_SYMOLS = 100; // wild x5, wartosc wymyslona 
		symb.Add_RTP_special(RTP_FOR_LINE_ASSEMBLED_ONLY_WILD_SYMOLS); // Pharaoh, Wild (9)

		int const LINE[50][5] = { // wczytywanie ksztaltu linii
		{ 1, 1, 1, 1, 1 }, // 1
		{ 0, 0, 0, 0, 0 }, // 2
		{ 2, 2, 2, 2, 2 }, // 3
		{ 0, 1, 2, 1, 0 }, // 4
		{ 1, 2, 3, 2, 1 }, // 5
		{ 2, 3, 4, 3, 2 }, // 6
		{ 0, 0, 1, 0, 0 }, // 7
		{ 1, 1, 2, 1, 1 }, // 8
		{ 2, 2, 3, 2, 2 }, // 9
		{ 0, 1, 1, 1, 0 }, // 10
		{ 1, 2, 2, 2, 1 }, // 11
		{ 2, 3, 3, 3, 2 }, // 12
		{ 0, 0, 1, 1, 0 }, // 13
		{ 1, 1, 2, 2, 1 }, // 14
		{ 2, 2, 3, 3, 2 }, // 15
		{ 0, 1, 1, 0, 0 }, // 16
		{ 1, 2, 2, 1, 1 }, // 17
		{ 2, 3, 3, 2, 2 }, // 18
		{ 1, 1, 1, 0, 0 }, // 19
		{ 1, 2, 3, 3, 2 }, // 20
		{ 2, 1, 0, 1, 2 }, // 21
		{ 0, 2, 4, 2, 0 }, // 22
		{ 2, 2, 2, 1, 0 }, // 23
		{ 0, 1, 2, 2, 2 }, // 24
		{ 0, 3, 0, 3, 0 }, // 25
		{ 2, 0, 4, 0, 2 }, // 26
		{ 1, 0, 2, 0, 1 }, // 27
		{ 2, 1, 3, 1, 2 }, // 28
		{ 0, 2, 1, 2, 0 }, // 29
		{ 1, 3, 2, 3, 1 }, // 30
		{ 1, 0, 4, 0, 1 }, // 31
		{ 2, 1, 4, 1, 2 }, // 32
		{ 2, 2, 4, 2, 2 }, // 33
		{ 1, 3, 0, 3, 1 }, // 34
		{ 0, 2, 0, 2, 0 }, // 35
		{ 0, 1, 0, 1, 0 }, // 36
		{ 1, 0, 2, 3, 1 }, // 37
		{ 1, 3, 2, 0, 1 }, // 38
		{ 1, 3, 4, 3, 1 }, // 39
		{ 1, 0, 0, 0, 1 }, // 40
		{ 1, 2, 4, 2, 1 }, // 41
		{ 1, 1, 0, 1, 1 }, // 42
		{ 0, 1, 3, 1, 0 }, // 43
		{ 2, 2, 1, 2, 2 }, // 44
		{ 2, 0, 3, 0, 2 }, // 45
		{ 0, 3, 1, 3, 0 }, // 46
		{ 0, 3, 2, 3, 0 }, // 47
		{ 2, 0, 2, 0, 2 }, // 48
		{ 0, 3, 4, 3, 0 }, // 49
		{ 2, 0, 0, 0, 2 } }; // 50
		int const FIRST_LINE = 0;
		for (int i = FIRST_LINE; i < ARRAY_LENGTH(LINE); ++i)
		{
			lines.Add(LINE[i][0], LINE[i][1], LINE[i][2], LINE[i][3], LINE[i][4]);
		}
	}

	void Slot_06::Read_File()
	{
		// wczytanie parametrow slotu z pliku txt

		std::string slot_data_begin = "------------  SLOT 06  --------------";
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

		Read_Scenario(wild_start);
		Read_Scenario(wild);

		Read_Scenario(freespin_1or2_start);
		Read_Scenario(freespin_1or2);
		Read_Scenario(freespin_3_start);
		Read_Scenario(freespin_3);

#if SHOW_READ_VAL == ON
		// wypisuje wczytane dane na konsoli w celu sprawdzenia, czy dane zostaly poprawnie wczytane

		std::cout << "------------  SLOT 06  --------------" << std::endl;
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

		Print_Scenario(wild_start);
		Print_Scenario(wild);

		Print_Scenario(freespin_1or2_start);
		Print_Scenario(freespin_1or2);
		Print_Scenario(freespin_3_start);
		Print_Scenario(freespin_3);
#endif
	}

	void Slot_06::Execute_Scenario()
	{
		// wrysowanie scenariuszy na tablicy wynikow

		int const FREE_SPINS = 10; // numer pod ktorym wystepuje symbol 'free spins' na tablicy wynikow
		int const WILD = 11; // numer pod ktorym wystepuje symbol 'wild' na tablicy wynikow

		switch (what_scen)
		{
			case 1: // freespin_3
			{
				int const THREE_ITEMS = 3;
				spec.Free_Spins(THREE_ITEMS, FREE_SPINS);

			}; break;

			case 2: // wild
			{
				spec.Expand_Wild(WILD);

			}; break;

			case 3: // freespin_1or2
			{
				int one_or_two;
				one_or_two = (rand() % 1) + 1;

				spec.Free_Spins(one_or_two, FREE_SPINS);

			}; break;
		}
	}

	float Slot_06::Scenario_Multipler()
	{
		// ustala mnoznik w momencie wystapienia scenariusza

		float const DEFALUT_VAL = 1;

		switch (what_scen)
		{
			case 2: // wild
			{
				float const WILD_MULTI = 3.5f;
				return WILD_MULTI;

			}; break;

			default:
			{
				return DEFALUT_VAL; // dla freespin_1or2 i freespin_3 mnoznik sie nie zwieksza

			}; break;
		}
	}

	void Slot_06::Convert_Symbols()
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
				case 1: result[x][y] = 8; break;   // Maat 
				case 2: result[x][y] = 7; break;   // Hathor 
				case 3: result[x][y] = 6; break;   // Khafre
				case 4: result[x][y] = 5; break;   // Scorpion
				case 5: result[x][y] = 4; break;   // Jackal  
				case 6: result[x][y] = 3; break;   // Sun
				case 7: result[x][y] = 2; break;   // 777 
				case 8: result[x][y] = 1; break;   // 77 
				case 9: result[x][y] = 0; break;   // 7 
				case 10: result[x][y] = 9; break;  // Cleopatra (Free spins)
				case 11: result[x][y] = 10; break; // Hierohliphs (Wild)

				default: result[x][y] = -1; break; // empty
				}
			}
		}

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
					case 1: back[x][y] = 8; break;   // Maat 
					case 2: back[x][y] = 7; break;   // Hathor 
					case 3: back[x][y] = 6; break;   // Khafre
					case 4: back[x][y] = 5; break;   // Scorpion
					case 5: back[x][y] = 4; break;   // Jackal  
					case 6: back[x][y] = 3; break;   // Sun
					case 7: back[x][y] = 2; break;   // 777 
					case 8: back[x][y] = 1; break;   // 77 
					case 9: back[x][y] = 0; break;   // 7 
					case 10: back[x][y] = 9; break;  // Cleopatra (Free spins)
					case 11: back[x][y] = 10; break; // Hierohliphs (Wild)

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