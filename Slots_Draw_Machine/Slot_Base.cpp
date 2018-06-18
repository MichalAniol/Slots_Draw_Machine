#include "stdafx.h"

#include "Slot_Base.h"

namespace Slots_Draw_Machine
{
	int Score::goal; // inicjalizacja obiektow static
	int Score::reduction;
	int Score::max;
	int Score::min;
	int Score::cycle;
	int Score::counter;
	int Shield::when_game_start;
	int Shield::when_begin_today;
	int Shield::annul_after;
	int Shield::after_lose;
	bool Shield::must_win;
	int Mega_Wins::length;
	int Mega_Wins::noise;
	int Mega_Wins::ture_length;
	int Mega_Wins::ture_noise;
	int Game::credits;
#if TESTS == ON // testy powyzej kilku mln potrzebuja typu long
	long Game::experience;
	long Game::all_games;
	long Game::all_today_games;
#else
	int Game::experience;
	int Game::all_games;
	int Game::all_today_games;
#endif
	bool Game::can_lose;
	int Game::free_spins;
	int Game::raise;

	Slot_Base::Slot_Base() :
		board(symb),
		lines(symb, board),
		spec(design, symb, board, param, game, rtp),
		design(lines, symb, board, param, game, score, shield, mega_wins),
		multi(design, param, game, shield, rtp,	mega_wins, score)
	{
		srand(int(time(NULL)));

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

#if TESTS == ON
		// do zliczania restartow prawdopodobienstwa przy testach szybkosci wykonywania losowan
		drop_counter[0] = 0; // :
		drop_counter[1] = 0; // a
		drop_counter[2] = 0; // b
		drop_counter[3] = 0; // c
		drop_counter[4] = 0; // #
#endif

#if SHOW_READ_VAL == ON
		std::cout << "------------  SLOT BASE  --------------" << std::endl;
		std::cout << score.goal << std::endl;
		std::cout << score.reduction << std::endl;
		std::cout << score.max << std::endl;
		std::cout << score.min << std::endl;
		std::cout << score.cycle << std::endl;

		std::cout << shield.when_game_start << std::endl;
		std::cout << shield.when_begin_today << std::endl;
		std::cout << shield.annul_after << std::endl;
		std::cout << shield.after_lose << std::endl;

		std::cout << mega_wins.length << std::endl;
		std::cout << mega_wins.noise << std::endl;
		std::cout << mega_wins.ture_length << std::endl;
		std::cout << mega_wins.ture_noise << std::endl << std::endl;
#endif
	}

	int Slot_Base::Draw()
	{
		// przeprowadza caly proces losowan i przedstawienia rezultatow na tablicy wynikow
		// zwraca wynik rtp lub -1 jesli nie mozna przeprowadzic losowania

		int const NONE = 0;
		this->rtp.bonus = NONE; // kasowanie wylosowanych w poprzedniej turze bonusow;

		int bet_val = Calculate_Bet(); // oblicza rtp dla tej tury

		bool const NOT_ENOUGH_CREDITS = false;
		if (Subtract_Bet(bet_val) == NOT_ENOUGH_CREDITS) // jesli nie mozna naliczyc zakladu
		{
			int const CANT_DRAW = -1;
			return CANT_DRAW; // zwroc -1 (brak losowania)
		}

		win_ture = design.Get_Win_Check() // sprawdza czy w tej turze pojawia sie wygrana z linii
			|| shield.must_win; // lub wygrana jest wymuszona z powodu zbyt malej ilosci kredytow

		what_scen = scenario.Hit_form_Scenario(); // czy wystepuje scenariusz w tej turze

		rtp.max = int(bet_val * multi.average_bet * multi.Major()); // maksymalna wygrana z linii w tej grze 

		rtp.scen = int(rtp.max * Scenario_Multipler()); // maksymalna wygrana z doliczeniem scenariuszy

		int rtp;
		rtp = Create_Slot(); // buduje na tablicy wynikow losowania i zraca rtp dla tego rozwiazania

		design.Set_Win_Mark_Used(); // zaznacz uzycie wygranej

		Tour_Summary(rtp); // podlicz ture

		Convert_Symbols(); // zmien symbole na wlasciwe dla gry

		return rtp;
	}


#if SHOW_READ_VAL == ON
	void Slot_Base::Print_Scenario(Scenario scenario)
	{
		int const FIRST_ITEM = 0;
		for (int i = FIRST_ITEM; i < scenario.num_of_cycles; ++i) std::cout << scenario.cycle[i] << " ";
		std::cout << std::endl;

		for (int i = FIRST_ITEM; i < scenario.num_of_cycles; ++i) std::cout << scenario.cycle_active[i] << " ";
		std::cout << std::endl;
	}
#endif

	void Slot_Base::Read_File()
	{
		// wczytuje bazowe parametry slotu

		std::string slot_data_begin = "------------  SLOT BASE  --------------";
		Find_this_Line(slot_data_begin);

		Read_Line(score.goal);
		Read_Line(score.reduction);
		Read_Line(score.max);
		Read_Line(score.min);
		Read_Line(score.cycle);

		Read_Line(shield.when_game_start);
		Read_Line(shield.when_begin_today);
		Read_Line(shield.annul_after);
		Read_Line(shield.after_lose);

		Read_Line(mega_wins.length);
		Read_Line(mega_wins.noise);
		Read_Line(mega_wins.ture_length);
		Read_Line(mega_wins.ture_noise);
	}

	void Slot_Base::Find_this_Line(std::string &text)
	{
		// szuka linii w pliku parametrow

		do
		{
			getline(file, line);

		} while (text != line)
			;
	}

	void Slot_Base::Read_Line(int &value)
	{
		// pierwszy parametr w linii i przechodzi do kolejnej

		file >> value;

		getline(file, line);
	}

	void Slot_Base::Read_Line(Items &val, int counter)
	{
		// wczytuje okreslona ilosc parametrow w linii i przechodzi do nastepnej

		for (int i = 0; i < counter; ++i)
		{
			file >> val.val[i];
		}

		val.counter = counter;

		getline(file, line);
	}

	void Slot_Base::Read_Scenario(Scenario &scenario)
	{
		// wczytuje scenariusz z aktualnej linii do wskazanego obiektu scenariusza i przechodzi do nastepnej linii

		int const FIRST_CYCLE = 0;
		int const LAST_CYCLE = 20;
		int temp[20];
		int value;
		for (int i = FIRST_CYCLE; i < LAST_CYCLE; ++i)
		{
			file >> temp[i];
			if (temp[i] == -1) break;
		}
		getline(file, line);
		for (int i = FIRST_CYCLE; i < LAST_CYCLE; ++i)
		{
			file >> value;
			if (value == -1 || temp[i] == -1) break;

			scenario.Add_Cycle(temp[i], value);
		}
	}

	void Slot_Base::Set_Probability()
	{
		// ustwawienie parametrow prawdopodobienstwa w obiektach za nie odopwiadajacych
		// w tej chwili tylko nadaje parametry, ale miala to byc funkcja zmieniajaca parametry 
		// w dostosowujace sie do zdarzen w slocie
		// jak na razie taka funkcja nie jest potrzebna, moglem ja podzielic i umiescic w konstruktorach,
		// jednak zostawilem ja na wszelki wypadek, gdyby jednak okazala sie kiedys potrzebna

		int cycyle_size; // ustawienie prawdopodobienstwa sredniej czestotliwosci wygranej
		cycyle_size = param.cycle.win;

		int coverage; // ustawienie prawdopodobienstwa pokrywajacych sie linii
		coverage = param.lines_cover;

		Items num_cover = param.num_cover; // ustawienie prawdopodobienstwa ilosci linii w przedziale od 1 do 5
		Items num_not_cover = param.num_not_cover; // ustawienie prawdopodobienstwa ilosci linii w przedziale od 1 do 5

		int bet_num; // ustawienie prawdopodobienstwa wystapienia dlugosci linii dla danego zakladu
		bet_num = param.bet.choose;
		Items bet_x_length = param.bet.x_length[bet_num];

		design.Change_Probability( // przeslanie ustawien do designera
			cycyle_size,
			coverage,
			num_cover,
			num_not_cover,
			bet_x_length);

		int spread;
		spread = param.sybols_spread;
		symb.Set_Spread(spread); // ustawienie prawdopodobienstwa trafienia danego symbolu
	}

	int Slot_Base::Calculate_Bet()
	{
		// ustala najmniejsze rtp w celu unikniecia przegranej w przypadku malej ilosci kredytow
		// pod warunkiem, ze przegrana nie zostala wymuszona

		int bet_val = param.bet.value.val[param.bet.choose]; // pobiera wartosc zakladu dla tej tury

		int const DEFAULT = 0;
		rtp.min = DEFAULT;

		int const FIRST_BET = 0;
		int lowest_bet = param.bet.value.val[FIRST_BET]; // najmniejsza wartosc zakladu na tym slocie

		bool less_for_play = (game.credits - bet_val) < lowest_bet; // jesli przy kolejnym zakladzie nie starczy na kolejny najnizszy zaklad

		shield.must_win =
			less_for_play == true && // jesli nie wystarcza kredytow na nastepna gre
			game.can_lose == false; // i jesli nie wlaczono przegrywania

		if (shield.must_win == true)
		{
			rtp.min = lowest_bet; // ustaw rtp na najmniejsza warosc zakladu
		}

		return bet_val;
	}

	bool Slot_Base::Subtract_Bet(int bet_val)
	{
		// odejmuje wysokosc zakladu od posiadanych kredytow

		int const NO_FREE_TOURS = 0;
		if (game.free_spins == NO_FREE_TOURS) // jesli nie jest to darmowa tura 
		{
			if (game.credits - bet_val < 0)
			{
				multi.Rise_Start(shield.after_lose); // aktywowanie zwiekszonych wygranych przez okrslona ilosc tur
				
				game.raise = shield.after_lose; // przekazanie do parametrow aby pamietano mimo wyjscia z gry
				
				game.can_lose = false; // wylacz mozliwosc stracenia wszystkich kredytow

				return false; // nie starczylo kredytow na zaklad
			}

			game.credits -= bet_val; // zaplata za gre
		}
		else
		{
			--game.free_spins; // zmniejszenie licznika pozostalych darmowych tur
		}

		return true; // odjeto
	}

	int Slot_Base::Create_Slot()
	{
		// buduje na tablicy wynikow losowania w zakresie min i max rtp i zraca rtp dla tej tury

		int rtp_val;
		int const START_POSITION = 0;
		int counter = START_POSITION; // licznik prob znalezienia rtp
		do
		{
			board.Clear(); // czyszczenie tablicy wynikow

			bool fill_up_done;
			do
			{
				if (knock_down == true) // opcja uruchamiana gdy bardzo dlugo nie chce nic wylosowac
				{
					design.Draw_Any();
				}
				else
				{
					if (win_ture == true // lub wylosowalo wygrana
						|| shield.must_win == true)  // lub wymusza wygrana
					{
						design.Draw(rtp.min, rtp.max); // projektowanie linii
					}
					else
					{
						design.Draw_No_Hits(); // symuluje bliskie trafienia
					}
				}

				fill_up_done = board.Fill_Up(); // dopisz losowe symbole w puste miejsca na tablicy

			} while (fill_up_done == false) // jesli nie udalo sie dopisac losowych symboli, to losuj jeszcze raz
				;

			int const NO_SCENARIO = 0;
			if (what_scen > NO_SCENARIO) // jesli jest aktywny jakis scenariusz
			{
				Execute_Scenario(); // dorysowanie wystepujacego scenariusza
			}

			lines.Find_Lines(spec_symb); // wyszukiwanie linii na tablicy wynikow

			rtp_val = Calculate_RTP(); // obliczanie rtp na podstawie znalezionych liniii

			++counter; // zwieksz licznik wyszukiwan
			No_Solution(counter); // jesli losowanie sie nie powiodlo

		} while (rtp_val < rtp.min
			|| rtp_val > rtp.scen) // tak dlugo az wynik zmiesci sie w ramach rtp
			;

		design.Mark_Use_All_Probabilities(); // zaznaczenie uzyc

		return rtp_val + rtp.bonus;
	}

	void Slot_Base::No_Solution(int counter)
	{
		// gdy przez okreslona ilosc powtorzen nie daje sie znalezc ukladu linii
		// dajacych wynik w podanym zakresie min i max rtp
		// uruchamia sie w sytuacji wystapienia rownoczesnie zdarzen:
		// wyczerpanie sie kredytow,  wskaznik 'game.score.min = 0'
		// i mala pula pozostalych prawdopodobienstw w obiektach 'Probability', 'Win' i 'Symbol'.

		if (counter == 50)
		{
			float const RAISE_A_BIT = 1.1f;
			rtp.max = int(rtp.max * (1 + ((RAISE_A_BIT - 1) / 2)));
			rtp.scen = int(rtp.scen * RAISE_A_BIT);

#if TESTS == ON
			std::cout << ":";
			drop_counter[0]++;
#endif
		}
		if (counter == 60)
		{
			float const RAISE_A_BIT = 1.2f;
			rtp.max = int(rtp.max * (1.0f + ((RAISE_A_BIT - 1.0f) / 2.0f)));
			rtp.scen = int(rtp.scen * RAISE_A_BIT);

#if TESTS == ON
			std::cout << "a";
			drop_counter[1]++;
#endif
		}
		if (counter == 70)
		{
			float const RAISE_A_BIT = 1.3f;
			rtp.max = int(rtp.max * (1.0f + ((RAISE_A_BIT - 1.0f) / 2.0f)));
			rtp.scen = int(rtp.scen * RAISE_A_BIT);
			design.Reset_Least();

#if TESTS == ON
			std::cout << "b";
			drop_counter[2]++;
#endif
		}
		if (counter == 80)
		{
			float const RAISE_A_BIT = 1.5f;
			rtp.max = int(rtp.max * (1.0f + ((RAISE_A_BIT - 1.0f) / 2.0f)));
			rtp.scen = int(rtp.scen * RAISE_A_BIT);
			design.Restart_All();

#if TESTS == ON
			std::cout << "c";
			drop_counter[3]++;
#endif
		}
		if (counter > 100)
		{
			float const RAISE_A_BIT = 1.2f;
			rtp.max = int(rtp.max * (1.0f + ((RAISE_A_BIT - 1.0f) / 2.0f)));
			rtp.scen = int(rtp.scen * RAISE_A_BIT);
			knock_down = true;

#if TESTS == ON
			std::cout << "#";
			drop_counter[4]++;
#endif
		}
	}

	void Slot_Base::Tour_Summary(int rtp)
	{
		// finalizacja tury

		knock_down = false; // wylaczenie skrocenia losowania jesli nastapilo

		int const BANKRUT_OFF = 0;
		if (shield.annul_after > BANKRUT_OFF // jesli prog przegranej jest aktywowany
			&& game.experience > shield.annul_after) // jesli doswiadczenie przekroczy okreslony prog
		{
			game.can_lose = true; // wlacz mozliwosc stracenia wszystkich kredytow

			int const BANKRUT_USED = -2;
			shield.annul_after = BANKRUT_USED; // wylaczenie mozliwosci ponownego przegrania
		}

		game.credits += rtp; // naliczenie wygranej

		int exp_for_tour = param.exp.for_bet.val[param.bet.choose] + param.exp.for_play;
		game.experience += exp_for_tour; // naliczenie expa

		int const NONE = 0;
		if (game.raise > NONE) // zmniejszenie licznika: zwiekszonych wygranych po utracie kredytow
		{
			--game.raise;
		}

		++game.all_games; // zwiekszenie licznikow
		++game.all_today_games;
		++game.slot_all_games;
		++game.slot_today_games;
	}

	int Slot_Base::Calculate_RTP()
	{
		// oblicza rtp na podstawie odnalezionych linii na tablicy wynikow

		int result = 0;

		for (int i = 0; i < lines.Get_Active_Num(); ++i)
		{
			int symbol = lines.Get_Active_Symb(i); // pobranie symbolu

			int length = lines.Get_Active_Length(i); // pobranie dlugosci

			int rtp_for_line = symb.Get_RTP(symbol, length); // sprawdzenie rtp
			result += rtp_for_line;
		}

		int bet_val = param.bet.value.val[param.bet.choose]; // pobranie wartosci zakladu
		result = result * bet_val / 10;	// przemnozenie rtp przez wysokosc zakladu

		return result;
	}




	void Slot_Base::Establish_Parameters() {}

	void Slot_Base::Execute_Scenario() {}

	float Slot_Base::Scenario_Multipler() { return 0; }

	void Slot_Base::Convert_Symbols() {}
}