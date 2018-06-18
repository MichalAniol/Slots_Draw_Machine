#include "stdafx.h"
#include "Lines_Designer.h"

namespace Slots_Draw_Machine
{
	Lines_Designer::Lines_Designer(
		Lines &lines,
		Symbols &symb,
		Result_Board &board,
		Slot_Parameter &param,
		Game &game,
		Score &score,
		Shield &shield,
		Mega_Wins &mega_wins
	) :
		LAST_MEM(ARRAY_LENGTH(mem.line) - 1)
	{
		this->lines = &lines; // wskazniki do komunikowania sie z obiektami
		this->symb = &symb;
		this->board = &board;
		this->param = &param;
		this->game = &game;
		this->score = &score;
		this->shield = &shield;
		this->mega_wins = &mega_wins;
	}

	int Lines_Designer::Nearest_No_Cross(int base_line, int search_around)
	{
		// szukanie drugiej nie pokrywajcej sie linii

		const int FIRST_SHAPE = 0;
		const int LAST_SHAPE = lines->Get_Num_Lines() - 1;

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		TEST_RANGE(base_line, FIRST_SHAPE, LAST_SHAPE)
		TEST_RANGE(search_around, FIRST_SHAPE, LAST_SHAPE)
#endif

		bool no_cross = !lines->Check_Crossing(base_line, search_around); // jesli wskazne linie w argumentach nie przecinaja sie 
		if (no_cross == true) return search_around;					     // od razu zwroc wartosc drugiej linii

		int const NEXT_ONE = 1;
		int look_up = search_around + NEXT_ONE; // numery argumentow do przeszukania
		int look_down = search_around - NEXT_ONE;

		int half_of_board = (lines->Get_Num_Lines() / 2) + 1;
		for (int counter = 0; counter < half_of_board; ++counter) // szukaj w gore i w dol zarazem
		{
			if (look_up > LAST_SHAPE) look_up = FIRST_SHAPE; // jesli przekroczy zakres wroc na poczatek
			if (look_down < FIRST_SHAPE) look_down = LAST_SHAPE; // jesli zejdzie ponizej zakresu wroc na koniec

			bool no_cross_up = !lines->Check_Crossing(base_line, look_up);
			if (no_cross_up == true) return look_up; // jesli sie nie przecianja zwroc te linie

			bool no_cross_down = !lines->Check_Crossing(base_line, look_down);
			if (no_cross_down == true) return look_down; // jesli sie nie przecianja zwroc te linie

			++look_up; // przejdz do nastepnych elementow listy
			--look_down;
		}

		const int FIND_NONE = -1;
		return FIND_NONE; // nie znalazl nieprzecinajacych sie
	}

	Items Lines_Designer::Nearest_No_Cross(int line_1st, int line_2nd, int line_3rd)
	{
		// szukanie trzech niepokrywajacych sie linii

		const int FIRST_SHAPE = 0;
		const int LAST_SHAPE = lines->Get_Num_Lines() - 1;

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		TEST_RANGE(line_1st, FIRST_SHAPE, LAST_SHAPE)
		TEST_RANGE(line_2nd, FIRST_SHAPE, LAST_SHAPE)
		TEST_RANGE(line_3rd, FIRST_SHAPE, LAST_SHAPE)
#endif

			Items result;
		int const THREE_LINES = 3;
		result.counter = THREE_LINES;

		int base_line_remember = line_1st;
		while (lines->Check_Crossing(line_1st, line_2nd, line_3rd)) // sprawdzaj kolejne warianty ulozenia linii az znadziesz 3 nie pokrywajace sie
		{
			++line_3rd; // przesuniecie trzeciego licznika o jeden
			if (line_3rd > LAST_SHAPE)
			{
				line_3rd = FIRST_SHAPE; // jesli trzeci przekroczy zakres 
				++line_2nd; // przesuniecie drugiego licznika o jeden
				if (line_2nd > LAST_SHAPE)
				{
					line_2nd = FIRST_SHAPE; // jesli drugi przekroczy zakres 
					++line_1st; // przesuniecie pierwszego licznika o jeden
					if (line_1st > LAST_SHAPE)
					{
						line_1st = FIRST_SHAPE; // jesli pierwszy przekroczy zakres 
						if (line_1st == base_line_remember) // jesli przejrzy juz wszystkie mozliwosci...
						{
							const int FIND_NONE = -1;
							result.val[0] = FIND_NONE; // ... i nie znajdziesz 
							result.val[1] = FIND_NONE;
							result.val[2] = FIND_NONE;

							return result;
						}
					}
				}
			}
		}

		result.val[0] = line_1st; // wpisanie warosci wyliczonych linii
		result.val[1] = line_2nd;
		result.val[2] = line_3rd;

		return result;
	}

	void Lines_Designer::Design_No_Cross(int counter)
	{
		// projektuje podana ilosc linii bez powtorzen nie pokrywajacych sie
		// tylko pierwsza linia jest uzywana z tablicy uzyc wszystich linii
		// wyniki sa zapisywane w 'mem'ach

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_DRAW = 1;
		const int MAX_DRAWS = 3;
		TEST_RANGE(counter, FIRST_DRAW, MAX_DRAWS)
#endif

		int const FIRST_ONE = 1;
		Design_Cross(FIRST_ONE); // wylosuj pierwsza linie 

		switch (counter)
		{
		case 1: break; // w przypadku wyszukania jednej linii

		case 2: // w przypadku wyszukania dwoch linii
		{
			int first = mem.line[0];
			int second = lines->Random_Not_Used(); // losowanie linii

			mem.line[1] = Nearest_No_Cross(first, second); // znalezienie naljblizszej niekrzyzujacej sie
		}; break;

		case 3: // w przypadku wyszukania trzech linii
		{
			int first = mem.line[0];
			int second = lines->Random_Not_Used(); // losowanie linii
			int third = lines->Random_Not_Used();

			Items result;
			result = Nearest_No_Cross(first, second, third); // znalezienie naljblizszych niekrzyzujacych sie

			mem.line[0] = result.val[0]; // zapisz wynik
			mem.line[1] = result.val[1];
			mem.line[2] = result.val[2];
		}; break;
		}
	}

	bool Lines_Designer::Find_No_Cross(int min_rtp, int max_rtp)
	{
		// znajduje niepokrywajace sie linie 

		mem.size = num_not_cover.Draw_Scope() + 1; // ilosc linii z prawdopodobienstwaa
		Items few_symb;
		do {
			int const NEW_CALCULATE = 0;
			rtp_for_tour = NEW_CALCULATE;
			Design_No_Cross(mem.size); // dopasowanie linii
			few_symb = symb->Get_A_Few_Random(mem.size); // dopasowanie symboli

			int const START_POSITION = 0;
			int counter = START_POSITION; // licznik prob dopasowania

			int const FIRST_DRAW = 0;
			for (int i = FIRST_DRAW; i < mem.size; ++i)
			{
				mem.length[i] = line_length.Draw_Scope(); // losowanie dlugosci
				mem.symb[i] = few_symb.val[i]; // zapisywanie symboli

				int check_symb = mem.symb[i]; // sybol 1 na tablicy rtp jest na pozycji 0
				int bet_val = param->bet.value.val[param->bet.choose]; // wartosc zakladu
				int rtp_for_bet = symb->Get_RTP(check_symb, mem.length[i]); // skladowe do obliczenia rtp

				int const RTP_DIVISOR = 10;
				rtp_for_tour += (rtp_for_bet * bet_val) / RTP_DIVISOR; // oblicza rtp

				++counter;
				int const MAX_REPETITIONS = 50;
				if (counter == MAX_REPETITIONS)
				{
					return false; // jesli licznik prob sie skonczy
				}
			}
		} while (rtp_for_tour < min_rtp || rtp_for_tour > max_rtp) // dopoki wynik nie miesci sie w zakresie
			;

		return true; // potwierdzenie znalezienia wyniku, wynik jest w mem'ach
	}

	void Lines_Designer::Design_Cross(int counter)
	{
		// projektuje podana ilosc linii bez powtorzen mogacych sie pokrywac
		// tylko pierwsza linia jest uzywana z tablicy uzyc wszystich linii
		// wyniki sa zapisywane w 'mem'ach

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
	int const FIRST_DRAW = 1;
	int const LAST_MEM = lines->Get_Num_Lines();
	TEST_RANGE(counter, FIRST_DRAW, LAST_MEM)
#endif

		mem.line[0] = lines->Random_Not_Used(); // losowanie pierwszej linii z tablicy used[]

		int const SECOND_DRAW = 1;
		int draw = SECOND_DRAW; // ustawienie losowania drugiej linii
		bool has_draw;

		while (draw < counter)
		{
			do {
				mem.line[draw] = rand() % lines->Get_Num_Lines(); // losowanie kolejnej linii

				has_draw = false;
				int const FIRST_DRAW = 0;
				for (int draw_before = FIRST_DRAW; draw_before < draw; ++draw_before) // sprawdznie czy zostala juz trafona w tym losowaniu
				{
					if (mem.line[draw] == mem.line[draw_before])
					{
						has_draw = true; // jesli tak to zaznacz
					}
				}

			} while (has_draw == true) // jesli juz by³a to wylosuj nowa
				;

			++draw; // przejscie do kolejnego losowania
		}
	}

	bool Lines_Designer::Find_Cross(int min_rtp, int max_rtp)
	{
		// znajduje pokrywajace sie linie

		mem.size = num_cover.Draw_Scope() + 1; // ilosc linii do znalezienia

		int const START_POSITION = 0;
		int counter = START_POSITION; // licznik prob dopasowania
		int draw_symbol;
		do {
			int const NEW_CALCULATE = 0;
			rtp_for_tour = NEW_CALCULATE;

			Design_Cross(mem.size);
			draw_symbol = symb->Get_Random(); // losowanie pierwszej linii

			int const FIRST_DRAW = 0;
			for (int i = FIRST_DRAW; i < mem.size; ++i) // 
			{
				mem.length[i] = line_length.Draw_Scope(); // losowanie dlugosci
				mem.symb[i] = draw_symbol; // zapamietanie symbolu

				int bet_val = param->bet.value.val[param->bet.choose]; // wartosc zakladu

				int const RTP_DIVISOR = 10;
				rtp_for_tour += (symb->Get_RTP(mem.symb[i], mem.length[i]) * bet_val) / RTP_DIVISOR; // obliczanie rtp
			}

			++counter;
			int const MAX_REPETITIONS = 50;
			if (counter == MAX_REPETITIONS)
			{
				return false; // konczy szukanie jesli przekroczy licznik
			}

		} while (rtp_for_tour < min_rtp || rtp_for_tour > max_rtp) // dopoki wynik nie miesci sie w zakresie
			;

		return true; // potwierdzenie znalezienia wyniku, wynik jest w mem'ach
	}

	void Lines_Designer::Change_Probability(
		int win,
		int cover,
		Items &num_cover,
		Items &num_not_cover,
		Items &line_length)
	{
		// zmienia warosci prawdopodobienstwa

		this->win.next_size = win;
		this->win.Change_Size();

		this->cover.Set_Scope(cover);

		this->num_cover.Set_Scope(num_cover);
		this->num_not_cover.Set_Scope(num_not_cover);
		this->line_length.Set_Scope(line_length);
	}

	void Lines_Designer::Draw(int min_rtp, int max_rtp)
	{
		// generuje losowania linii, ktore daja w przeliczeniu wynik mieszczacy sie w podanym rtp

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int MIN_BET = 0;
		const int MAX_BET = 50000000;
		TEST_RANGE(min_rtp, MIN_BET, max_rtp)
		TEST_RANGE(max_rtp, min_rtp, MAX_BET)
#endif

		int sensible_max_rtp = int(param->bet.value.val[param->bet.choose] * symb->Get_RTP(symb->Get_Size(), 0)); // najmniejsza mozliwa wygrana
		if (max_rtp < sensible_max_rtp) // jesli max_rtp jest za male zaby dalo sie znalezc wynik
		{
			max_rtp = sensible_max_rtp; // to je podwyzsz
		}

		int const START_POSITION = 0;
		int counter = START_POSITION; // licznik prob dopasowania
		bool is_found;
		do {
			bool coverage;
			if (shield->must_win == true) // jesli wygrana jest wymuszona 
			{
				coverage = false; // to brak pokryc, aby szybciej losowac
			}
			else
			{
				coverage = cover.Draw_Scope(); // losuj pokrycie
			}

			if (coverage == true) // jesli linie maja sie pokrywac
			{
				is_found = Find_Cross(min_rtp, max_rtp);
			}
			else // jesli linie maja sie nie pokrywac
			{
				is_found = Find_No_Cross(min_rtp, max_rtp);
			}

			++counter;
			int const MAX_REPETITIONS = 40;
			if (counter == MAX_REPETITIONS) // jesli nie mozesz znalezc dopasowania
			{
				return;
			}

		} while (is_found == false) // az znajdzie wynik mieszczacy sie w podanym zakresie rtp
			;

		Execute(); // wypisanie projektowanych linii na tablicy wynikow
	}

	void Lines_Designer::Execute()
	{
		// wypisz na tablicy wynikow to co wylosowano

		for (int i = 0; i < mem.size; i++)
		{
			int symbol = mem.symb[i];
			int line = mem.line[i];
			int length = mem.length[i];

			lines->Print_on_Board(symbol, line, length);
		}
	}

	void Lines_Designer::Draw_Any()
	{
		// losuje zestaw linii w przypadkach, gdy zadne losowania w oparciu o obiekty prawdopodobienstwa
		// nie dadza rezultatow, a losowanie musi dawac wygrana w sytacji gdy gra broni sie przez strata wszystkich kredytow

		int const FIRST_SYMB = 1;
		mem.symb[0] = rand() % (symb->Get_Size() - FIRST_SYMB) + FIRST_SYMB; // losowanie symbolu, 0 to brak sybolu, dlatego przesuniecie o 1
		mem.line[0] = rand() % lines->Get_Num_Lines(); // losowanie linii
		int const FIRST_THREE_SYMB = 2;
		mem.length[0] = rand() % (board->Get_Length() - FIRST_THREE_SYMB); // losowanie dlugosci, na tablicy wynikow rtp 3 linia jest na pozycji 0

		int const MAX_DRAWS = 2;
		int const MORE_THAN_ONE = 1;
		int draw_anything = (rand() % MAX_DRAWS) + MORE_THAN_ONE; // dorysuj 1, 2 lub 3 linie

		int const NONE = 0;
		for (int i = MORE_THAN_ONE; i < draw_anything; ++i) // jesli wylosowano wiecej niz 1 linie, to dorysuj
		{
			do
			{
				mem.line[i] = rand() % lines->Get_Num_Lines(); // losowanie linii

			} while (mem.line[i] == mem.line[0]) // zeby byl rozny od poprzednio wylosowanego
				;

			mem.symb[i] = mem.symb[0]; // dla wszystkich wylosowanyc linii symbole maja byc takie same

			mem.length[i] = rand() % (board->Get_Length() - FIRST_THREE_SYMB); // losowanie dlugosci 

		}

		int const FOR_RIGHT_SIZE = 1;
		mem.size = draw_anything + FOR_RIGHT_SIZE; // ilosc wylosowanych linii, ilosc musi byc wieksza o 1 od ostatniej pozycji 'mem-line'

		Execute();

		int const NO_ITEMS = 0;
		mem.size = NO_ITEMS; // aby nie odznaczano zadnych losowan w obiektach 'Probability'
	}

	void Lines_Designer::Draw_No_Hits()
	{
		// losuje linie i symbole, aby zasymulowac bliskie trafienia, bez uzycia list prawdopodobienstwa i zaznaczania ich uzycia

		int const NO_SYMBOL = 0;
		int symb_memo = NO_SYMBOL; // aby przy dwoch losowaniach symbole sie nie powtorzyly

		int const NO_LINE = -1;
		int line_memo = NO_LINE; // aby przy dwoch losowaniach linie sie nie powtorzyly

		int const MAX_DRAWS = 3;
		int draw_anything = rand() % MAX_DRAWS; // 0 - niczego nie losowac, 1 i 2 dorysuj bliskie trafienia jednej lub dwoch linii

		int const FIRST_DRAW = 0;
		for (int i = FIRST_DRAW; i < draw_anything; i++)
		{
			int line_num;
			do
			{
				line_num = rand() % lines->Get_Num_Lines(); // losowanie linii

			} while (line_num == line_memo) // zeby byl rozny od poprzednio wylosowanego
				;
			line_memo = line_num; // zpamietywanie wylosowanej linii

			int symb_num;
			int const FIRST_SYMB = 1;
			do
			{
				symb_num = rand() % (symb->Get_Size() - FIRST_SYMB) + FIRST_SYMB; // losowanie symbolu, 0 to brak sybolu, dlatego przesuniecie o 1

			} while (symb_num == symb_memo) // zeby byl rozny od poprzednio wylosowanego
				;
			symb_memo = symb_num; // zapamietanie wylosowanego symbolu

			Execute_No_Hits(line_num, symb_num);
		}

		int const NO_ITEMS = 0;
		mem.size = NO_ITEMS; // aby nie odznaczano zadnych losowan w obiektach 'Probability'
	}

	void Lines_Designer::Execute_No_Hits(int line, int symb)
	{
		// rysuje linie z symboli wraz z pustymi miejscami, tak aby nie naliczono rtp

		bool blank[ARRAY_LENGTH(board->result)]; // tablica wskazujaca, ktore symbole maja byc puste na linii
		int const FIRST_BLANK = 0;
		for (int i = FIRST_BLANK; i < ARRAY_LENGTH(board->result); ++i)
		{
			blank[i] = false; // czyszczenie tablicy
		}

		int const THIRD_RING = 3;
		int blank_symb = rand() % THIRD_RING; // losowanie ktory symbol z trzech pierwszych wyciac
		blank[blank_symb] = true; // zaznaczenie pustego miejsca

		int const BOARD_MIN_LENGTH = 3; // najmniejsza mozliwa szerokosc slotu
		if (board->Get_Length() > BOARD_MIN_LENGTH)
		{
			int rest_rings = board->Get_Length() - BOARD_MIN_LENGTH; // ile jeszcze mozna wymazac
			int more_rings = rand() % rest_rings; // losowanie ile wymazac z pozostalych

			int const FIRST_RING = 0;
			for (int ring = FIRST_RING; ring < more_rings; ++ring) // losowanie kolejnych symboli do wyzerowania
			{
				do
				{
					blank_symb = rand() % board->Get_Length(); // losuj kolejny symbol

				} while (blank[blank_symb] == true) // az znajdziesz niezaznaczony
					;
				blank[blank_symb] = true; // zaznaczenie pustego miejsca
			}
		}

		Position pos;
		for (int ring = 0; ring < board->Get_Length(); ++ring) // wrysowanie linii
		{
			if (blank[ring] == false) // jesli nie zostalo zaznaczone, ze ma byc puste
			{
				pos.x = ring;
				pos.y = lines->Get_Symb_Pos(line, ring);
				board->Set_Symbol(pos, symb); // to wpisz symbol
			}
		}
	}

	void Lines_Designer::Mark_Use_All_Probabilities()
	{
		// zpamietaj ktore elemaenty zostaly uzyte, aby nie powtarzaly sie w kolejnych losowaniach

		int const FIRST_MEM = 0;
		for (int i = FIRST_MEM; i < mem.size; ++i) // wszystkie zapamietane wartosci
		{
			line_length.Mark_Used(mem.length[i]); // zaznacz jako uzyte dlugosci linii
			symb->Mark_Used(); // i symbole
		}

		int const COVER_ON = 0;
		if (num_cover.Get_Resutl() == COVER_ON)// jesli pokrywaly sie
		{
			num_cover.Mark_Used();
		}
		else // jesli nie pokrywaly sie
		{
			num_not_cover.Mark_Used();
		}

		cover.Mark_Used(); // zaznacz czy sie pokrywaly
	}

	void Lines_Designer::Reset_Least()
	{
		// restartuje obiekt, ktory ma najmniejsze pole nieuzytych losowan
		// uzywane w sytuacji, gdy w kilku lub wszystkich obiektach prawdopodobienstwa pozostala za mala ilosc
		// nieuzytych losowan i z tego powodu nie mozna spelnic oczekiwanej wysokosci wyniku rtp

		int const ALL_PROBABILITIES = 4;
		Probability *proba[ALL_PROBABILITIES];
		proba[0] = &cover;
		proba[1] = &num_cover;
		proba[2] = &num_not_cover;
		proba[3] = &line_length;

		int const FIRST_ONE = 0;
		int choose = FIRST_ONE;
		int lesser_prop = (proba)[choose]->Get_Left(); // sprawdz wartosc pierwszego 'Probability' z listy

		for (int i = choose + 1; i < ALL_PROBABILITIES; ++i) // wyszukaj najnizsza wartosc
		{
			if (proba[i]->Get_Left() < lesser_prop)
			{
				choose = i;
				lesser_prop = proba[choose]->Get_Left(); // i zapamietaj ile wynosi
			}
		}

		if (symb->Get_Left() < lines->Get_Left_Percent()) // czy mniej jest symboli czy linii
		{
			if (lesser_prop < symb->Get_Left()) // czy mniejsze jest ktores 'Probability' czy ilosc pozostalych symboli
			{
				proba[choose]->Reset_Used(); // jesli 'Probability'
			}
			else
			{
				symb->Reset_Used(); // jesli symboli
			}
		}
		else
		{
			if (lesser_prop < lines->Get_Left_Percent()) // czy mniejsze jest ktores 'Probability' czy ilosc pozostalych linii
			{
				proba[choose]->Reset_Used(); // jesli 'Propability'
			}
			else
			{
				lines->Reset_Used(); // jesli linii
			}
		}
	}

	void Lines_Designer::Restart_All()
	{
		// reset wszystkich obiektow prawdopodobienstwa 
		// uzywane w sytuacji, gdy w kilku lub wszystkich obiektach prawdopodobienstwa pozostala za mala ilosc
		// nieuzytych losowan i z tego powodu nie mozna spelnic oczekiwanej wysokosci wyniku rtp

		cover.Reset_Used();
		num_cover.Reset_Used();
		num_not_cover.Reset_Used();
		line_length.Reset_Used();

		symb->Reset_Used();

		lines->Reset_Used();
	}



	bool Lines_Designer::Get_Win_Check()
	{
		// zwraca czy w danej turze ma sie pojawic wygrana

		return win.Check();
	}

	void Lines_Designer::Set_Win_Mark_Used()
	{
		// zaznacza uzycie 

		win.Mark_Used();
	}

	int Lines_Designer::Get_Win_Next_Size()
	{
		// zwraca wielkosc cylku obiektu 'win' 

		return win.next_size;
	}

	void Lines_Designer::Set_Win_Next_Size(int size)
	{
		// ustawia wielksc cyklu obiektu 'win' 

		int const MIN_SIZE = 2;
		if (size < 2)
		{
			size = 2;
		}

		int const MAX_SIZE = 20;
		if (size > 20)
		{
			size = 20;
		}

		win.next_size = size;
	}

	void Lines_Designer::Change_Win_Size()
	{
		// wprowadza projektowana wielkosc do obiektu 'win'

		win.Change_Size();
	}
}
