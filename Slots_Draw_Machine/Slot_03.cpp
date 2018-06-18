#include "stdafx.h"
#include "Slot_03.h"

namespace Slots_Draw_Machine
{
	Slot_03::Slot_03()
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
		multi.average_bet = 1.2f;                          // przecietna wartosc mnoznika 

		scenario.Add_Scenario(wild_start, wild);         // dodanie scenariuszy do sceanrio holder
		scenario.Add_Scenario(mystery_start, mystery);
		scenario.Add_Scenario(bonusgame_3_start, bonusgame_3);
		scenario.Add_Scenario(bonusgame_1or2_start, bonusgame_1or2);
		scenario.Add_Scenario(pic_anubis_start, pic_anubis);
		scenario.Add_Scenario(pic_isis_start, pic_isis);
		scenario.Activation(game.slot_all_games); // uruchomienie scenariuszy

		int const START_POSITION = 0;
		param.cycle.counter = START_POSITION;  // zerowanie licznika cykli
		game.free_spins = START_POSITION;     // zerowanie licznika freespinow
		score.counter = START_POSITION;      // zerowanoie licznika zmiany 'score goal'
		param.cycle.raise_on = false;       // opuszczenie flagi cyklu specjalnego

		rtp.multip.go_up = float(100 + param.cycle.raise) / 100; // zamiana procentow na mnoznik
		rtp.multip.go_down = float(100 - param.cycle.reduce) / 100;

		int const WILD = 10;          // numer pod ktorym wystepuje symbol 'wild' na tablicy wynikow
		spec_symb = WILD;           // przeslanie znaku specjalnego

		Set_Probability();       // przeslanie wczytanych danych do obiektow tworzacych prawdopodobienstwo
	}

	void Slot_03::Establish_Parameters()
	{
		// wprowadzenie danych do list symboli i linii

		int const SYMB_RTP[9][3] = { // wczytywanie rtp symboli
		{ 10, 20, 100 }, // Isis      (1)
		{ 10, 20, 50 },  // Anubis    (2)
		{ 10, 20, 30 },  // Sphinx    (3)
		{ 5, 10, 20 },   // Ankh      (4)
		{ 5, 10, 15 },   // Mummy     (5)
		{ 3, 6, 12 },    // Statue    (6)
		{ 2, 5, 10 },    // Camel     (7)
		{ 2, 3, 6 },     // Crocodile (8)
		{ 1, 2, 5 }, };  // Bell      (9)
		int const FIRST_SYMBOL = 0;
		for (int i = FIRST_SYMBOL; i < ARRAY_LENGTH(SYMB_RTP); ++i)
		{
			symb.Add_RTP(SYMB_RTP[i][0], SYMB_RTP[i][1], SYMB_RTP[i][2]);
		}

		int const RTP_FOR_LINE_ASSEMBLED_ONLY_WILD_SYMOLS = 100; // wild x5, wartosc wymyslona 
		symb.Add_RTP_special(RTP_FOR_LINE_ASSEMBLED_ONLY_WILD_SYMOLS); // Pharaoh, Wild (10)

		int const LINE[30][5] = { // wczytywanie ksztaltu linii
		{ 1, 1, 1, 1, 1 }, //1
		{ 0, 0, 0, 0, 0 }, //2
		{ 2, 2, 2, 2, 2 }, //3
		{ 0, 0, 1, 2, 2 }, //4
		{ 2, 2, 1, 0, 0 }, //5
		{ 0, 2, 0, 2, 0 }, //6
		{ 2, 0, 2, 0, 2 }, //7
						   //	{ 1, 0, 1, 0, 1 },
		{ 0, 1, 0, 1, 0 }, //8
		{ 2, 1, 2, 1, 2 }, //9
		{ 1, 2, 1, 2, 1 }, //10
		{ 2, 1, 0, 1, 2 }, //11
		{ 0, 1, 2, 1, 0 }, //12
		{ 1, 2, 1, 0, 1 }, //13
		{ 1, 0, 1, 2, 1 }, //14
		{ 1, 1, 0, 1, 1 }, //15
		{ 1, 1, 2, 1, 1 }, //16
		{ 1, 0, 0, 0, 1 }, //17
		{ 1, 2, 2, 2, 1 }, //18
		{ 1, 0, 2, 0, 1 }, //19
		{ 1, 2, 0, 2, 1 }, //20
		{ 2, 0, 0, 0, 2 }, //21
		{ 0, 2, 2, 2, 0 }, //22
		{ 2, 1, 1, 1, 2 }, //23
		{ 0, 1, 1, 1, 0 }, //24
		{ 2, 1, 1, 1, 0 }, //25
		{ 0, 1, 1, 1, 2 }, //26
		{ 2, 2, 1, 2, 2 }, //27
		{ 0, 0, 1, 0, 0 }, //28
		{ 2, 2, 0, 2, 2 }, //29
		{ 0, 0, 2, 0, 0 } }; //30
		int const FIRST_LINE = 0;
		for (int i = FIRST_LINE; i < ARRAY_LENGTH(LINE); ++i)
		{
			lines.Add(LINE[i][0], LINE[i][1], LINE[i][2], LINE[i][3], LINE[i][4]);
		}
	}

	void Slot_03::Read_File()
	{
		// wczytanie parametrow slotu z pliku txt

		std::string slot_data_begin = "------------  SLOT 03  --------------";
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

		Read_Scenario(bonusgame_1or2_start);
		Read_Scenario(bonusgame_1or2);
		Read_Scenario(bonusgame_3_start);
		Read_Scenario(bonusgame_3);

		Read_Scenario(mystery_start);
		Read_Scenario(mystery);
		Read_Scenario(wild_start);
		Read_Scenario(wild);

		Read_Scenario(pic_anubis_start);
		Read_Scenario(pic_anubis);
		Read_Scenario(pic_isis_start);
		Read_Scenario(pic_isis);

#if SHOW_READ_VAL == ON
		// wypisuje wczytane dane na konsoli w celu sprawdzenia, czy dane zostaly poprawnie wczytane

		std::cout << "------------  SLOT 03  --------------" << std::endl;
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


		Print_Scenario(bonusgame_1or2_start);
		Print_Scenario(bonusgame_1or2);
		Print_Scenario(bonusgame_3_start);
		Print_Scenario(bonusgame_3);

		Print_Scenario(mystery_start);
		Print_Scenario(mystery);
		Print_Scenario(wild_start);
		Print_Scenario(wild);

		Print_Scenario(pic_anubis_start);
		Print_Scenario(pic_anubis);
		Print_Scenario(pic_isis_start);
		Print_Scenario(pic_isis);

#endif
	}

	void Slot_03::Execute_Scenario()
	{
		// wrysowanie scenariuszy na tablicy wynikow

		int const WILD = 10; // numer pod ktorym wystepuje symbol 'wild' na tablicy wynikow
		int const MYSTERY = 11; // numer pod ktorym wystepuje symbol 'free spins' na tablicy wynikow
		int const BONUS_GAME = 12; // numer pod ktorym wystepuje symbol 'bonus game' na tablicy wynikow

		int const ANUBIS = 1; // z tabeli symboli
		int const ISIS = 2;

		switch (what_scen)
		{
			case 1: // wild
			{
				spec.Wild(WILD);
			}; break;

			case 2: // mystery
			{
				spec.Mystery(MYSTERY);
			}; break;

			case 3: // bonusgame_3
			{
				int const THREE_ITEMS = 3;
				spec.Bonus_Game(THREE_ITEMS, BONUS_GAME);
			}; break;

			case 4: // bonusgame_1or2
			{
				int one_or_two;
				one_or_two = (rand() % 1) + 1;
				spec.Bonus_Game(one_or_two, BONUS_GAME);

			}; break;

			case 5: // pic_anubis
			{
				spec.Pictures_1_x_2_or_3(ANUBIS);

			}; break;

			case 6: // pic_isis
			{
				spec.Pictures_1_x_2_or_3(ISIS);

			}; break;
		}
	}

	float Slot_03::Scenario_Multipler()
	{
		// ustala mnoznik w momencie wystapienia scenariusza

		float const DEFALUT_VAL = 1;

		switch (what_scen)
		{
			case 0: // bez scenariusza
			{
				return DEFALUT_VAL; // zaden scenariusz nie wystepuje w tej turze

			}; break;

			case 1: // wild
			{
				float const WILD_MULTI = 3.3f;
				return WILD_MULTI;

			}; break;

			case 2: // mystery
			{
				float const MYSTERY_MULTI = 2.5f;
				return MYSTERY_MULTI;

			}; break;

			default:
			{
				return DEFALUT_VAL; // dla freespin_1or2 i freespin_3 mnoznik sie nie zwieksza

			}; break;
		}
	}

	void Slot_03::Convert_Symbols()
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
				case 1: result[x][y] = 8; break;   // Isis 
				case 2: result[x][y] = 7; break;   // Anubis 
				case 3: result[x][y] = 6; break;   // Sphinx
				case 4: result[x][y] = 5; break;   // Ankh
				case 5: result[x][y] = 4; break;   // Mummy  
				case 6: result[x][y] = 3; break;   // Statue
				case 7: result[x][y] = 2; break;   // Camel
				case 8: result[x][y] = 1; break;   // Crocodile 
				case 9: result[x][y] = 0; break;   // Bell
				case 10: result[x][y] = 9; break;  // Osiris (Wild)
				case 11: result[x][y] = 10; break; // (Mystery)
				case 12: result[x][y] = 11; break; // Chest (Bonus Game)

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
					case 1: back[x][y] = 8; break;   // Isis 
					case 2: back[x][y] = 7; break;   // Anubis 
					case 3: back[x][y] = 6; break;   // Sphinx
					case 4: back[x][y] = 5; break;   // Ankh
					case 5: back[x][y] = 4; break;   // Mummy  
					case 6: back[x][y] = 3; break;   // Statue
					case 7: back[x][y] = 2; break;   // Camel
					case 8: back[x][y] = 1; break;   // Crocodile 
					case 9: back[x][y] = 0; break;   // Bell
					case 10: back[x][y] = 9; break;  // Osiris (Wild)
					case 11: back[x][y] = 10; break; // (Mystery)
					case 12: back[x][y] = 11; break; // Chest (Bonus Game)

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