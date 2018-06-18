#include "stdafx.h"
#include "Lines.h"

namespace Slots_Draw_Machine
{
	Lines::Lines(Symbols &symb, Result_Board &board)
	{
		this->symb = &symb; // wskaznik do komunikacji z obiektem
		this->board = &board; // wskaznik do komunikacji z obiektem

		int const NONE = 0;
		num_lines = NONE;
		left = NONE;

		const int FIRST_ITEM = 0;
		for (int i = FIRST_ITEM; i < ARRAY_LENGTH(shape); ++i) // zerwoanie tablic
		{
			for (int j = FIRST_ITEM; j < ARRAY_SECOND(shape); ++j)
			{
				shape[i][j] = NONE; // tablica ksztaltow linii
			}

			used[i] = false; // tablica uzyc linii
		}

		active.counter = NONE;
	}

	Items Lines::Get(int the_line)
	{
		// odczytywanie ksztaltu linii

		const int FIRST_SHAPE = 0;

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int LAST_SHAPE = ARRAY_LENGTH(shape) - 1;
		TEST_RANGE(the_line, FIRST_SHAPE, LAST_SHAPE)
#endif

		Items line;
		line.counter = ARRAY_SECOND(shape);

		for (int ring = FIRST_SHAPE; ring < line.counter; ++ring) // wczyatj ring po ringu
		{
			line.val[ring] = shape[the_line][ring];
		}

		return line;
	}

	bool Lines::Check_Used(int the_line)
	{
		// sprawdzanie uzycia linii

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_SHAPE = 0;
		TEST_RANGE(the_line, FIRST_SHAPE, num_lines)
#endif

			return used[the_line];
	}

	void Lines::Mark_as_Used(int the_line)
	{
		// zaznaczenie uzycia

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_SHAPE = 0;
		TEST_RANGE(the_line, FIRST_SHAPE, num_lines)
#endif

			if (used[the_line] == false)
			{
				used[the_line] = true;

				--left; // zmniejszenie licznika pozostalych do uzycia
			}
	}

	void Lines::Check_Length(int num_line, int look_for_symb, int special)
	{
		// sprawdza jakiej dlugosci jest linia, a wynik zapisuje do obiektu 'active'

		int const FIRST_RING = 0;
		int symb_list[ARRAY_LENGTH(board->result)];
		for (int ring = FIRST_RING; ring < board->Get_Length(); ++ring) // tworzy liste symboli z linii
		{
			int positon_y = shape[num_line][ring];
			int symb_form_board = board->result[ring][positon_y];

			symb_list[ring] = symb_form_board;
		}

		int length;
		if ((symb_list[0] == look_for_symb || symb_list[0] == special)
			&& (symb_list[1] == look_for_symb || symb_list[1] == special)
			&& (symb_list[2] == look_for_symb || symb_list[2] == special)) // sprawdz czy pierwsze trzy symbole sa takie same lub specjalne
		{
			int const THREE_SYMB = 0; // dlugosc 3 na tablicy rtp znajduje sie na pozycji 0
			length = THREE_SYMB;
			int const FOURTH_RING = 3;
			if ((symb_list[3] == look_for_symb || symb_list[3] == special) // czy czwarty jest szukanym symbolem lub specjalnym
				&& board->Get_Length() > FOURTH_RING) // jesli slot jest na tyle dlugi
			{
				++length; // length = 1
				int const FIFTH_RING = 4;
				if ((symb_list[4] == look_for_symb || symb_list[4] == special) // czy piaty jest szukanym symbolem lub specjalnym
					&& board->Get_Length() > FIFTH_RING) // jesli slot jest na tyle dlugi
				{
					++length; // length = 2
				}
			}
			active.line[active.counter] = num_line; // dopisz do listy wystepujacych linii
			active.symb[active.counter] = look_for_symb; // zlozona z symboli
			active.length[active.counter] = length; // dlugosc linii
			++active.counter;
		}
	}

	void Lines::Add(int one, int two, int three)
	{
		// dodawanie linii o dlugosci trzech symboli

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int SLOT_BEGIN = 0;
		const int SLOT_HEIGHT = 6;
		TEST_RANGE(one, SLOT_BEGIN, SLOT_HEIGHT)
		TEST_RANGE(two, SLOT_BEGIN, SLOT_HEIGHT)
		TEST_RANGE(three, SLOT_BEGIN, SLOT_HEIGHT)

		const int FIRST_SHAPE = 0;
		const int LAST_SHAPE = ARRAY_LENGTH(shape) - 1;
		TEST_RANGE(num_lines, FIRST_SHAPE, LAST_SHAPE)
#endif

		shape[num_lines][0] = one; // dodawanie do listy
		shape[num_lines][1] = two;
		shape[num_lines][2] = three;

		int const EMPTY = -1;
		shape[num_lines][3] = EMPTY;
		shape[num_lines][4] = EMPTY;

		++num_lines; // zwiekszenie licznika listy
		++left;     // i pozostalych do losowania
	}

	void Lines::Add(int one, int two, int three, int four, int five)
	{
		// dodawanie linii o dlugosci pieciu symboli

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int SLOT_BEGIN = 0;
		const int SLOT_HEIGHT = 6;
		TEST_RANGE(one, SLOT_BEGIN, SLOT_HEIGHT)
		TEST_RANGE(two, SLOT_BEGIN, SLOT_HEIGHT)
		TEST_RANGE(three, SLOT_BEGIN, SLOT_HEIGHT)
		TEST_RANGE(four, SLOT_BEGIN, SLOT_HEIGHT)
		TEST_RANGE(five, SLOT_BEGIN, SLOT_HEIGHT)

		const int FIRST_SHAPE = 0;
		const int LAST_SHAPE = ARRAY_LENGTH(shape) - 1;
		TEST_RANGE(num_lines, FIRST_SHAPE, LAST_SHAPE)
#endif

		shape[num_lines][0] = one; // dodawanie do listy
		shape[num_lines][1] = two;
		shape[num_lines][2] = three;
		shape[num_lines][3] = four;
		shape[num_lines][4] = five;

		++num_lines; // zwiekszanie licznika listy
		++left;     // i pozostalych do losowania
	}

	bool Lines::Check_Crossing(int line_1st, int line_2nd)
	{
		// sprawdzanie czy linie sie przecinaja (dla dwoch linii)

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_SHAPE = 0;
		TEST_RANGE(line_1st, FIRST_SHAPE, num_lines)
		TEST_RANGE(line_2nd, FIRST_SHAPE, num_lines)
#endif

			if (line_1st == line_2nd) // jesli to sa takie same linie
			{
				return true;
			}

		bool lines_cross = false; // opuszczona flaga przeciecia 

		int const FIRST_RING = 0;
		for (int ring = FIRST_RING; ring < board->Get_Length(); ++ring) // porownaj poszczegolne symbole na liniach
		{
			if (shape[line_1st][ring] == shape[line_2nd][ring]) // jesli symbole sa takie same
			{
				lines_cross = true; // to podnies flage przecinania sie linii
			}
		}

		return lines_cross;
	}

	bool Lines::Check_Crossing(int line_1st, int line_2nd, int line_3rd)
	{
		// sprawdzenie czy linie sie przecinaja (dla trzech linii)

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_SHAPE = 0;
		TEST_RANGE(line_1st, FIRST_SHAPE, num_lines)
		TEST_RANGE(line_2nd, FIRST_SHAPE, num_lines)
		TEST_RANGE(line_3rd, FIRST_SHAPE, num_lines)
#endif

			if (line_1st == line_2nd || // jesli to sa takie same linie
				line_1st == line_3rd ||
				line_2nd == line_3rd)
			{
				return true;
			}

		bool items_cross = false; // opuszczona flaga przeciecia 
		int const FIRST_RING = 0;
		for (int ring = FIRST_RING; ring < board->Get_Length(); ++ring) // porownaj poszczegolne symbole na liniach
		{
			if (shape[line_1st][ring] == shape[line_2nd][ring] || // jesli symbole sa takie same
				shape[line_1st][ring] == shape[line_3rd][ring] ||
				shape[line_2nd][ring] == shape[line_3rd][ring])
			{
				items_cross = true; // to podnies flage przecinania sie linii
			}
		}
		return items_cross;
	}

	void Lines::Find_Lines(int special)
	{
		// wyszukuje linie na tablicy wynikow, sprawdza z jakiego symbolu sie skladaja

		active.counter = 0; // wyzerownie licznika znalezionych linii

		int const FIRST_LINE = 0;
		for (int num_line = FIRST_LINE; num_line < num_lines; ++num_line) // sprawdz wszystkie linie po koleji
		{
			int look_for_symb;
			int count_special = 0; // licznik wystapien symbolu specjalnego w przeszukiwanej linii
			int const FIRST_RING = 0;
			for (int ring = FIRST_RING; ring < board->Get_Length(); ++ring) // sprawdz symbole na linii, az znajdziesz...(1)
			{
				int y_positon = shape[num_line][ring];             // miejsce kolejnego elementu linii na ringu
				int symb_for_pos = board->result[ring][y_positon]; // wartosc tego miejsca
				if (symb_for_pos == special)                     // jesli symbol w linii jest symbolem specjalnym
				{
					++count_special; // to nalicz kolejny
					continue;       // i sprawdzaj nastepny ring
				}

				look_for_symb = symb_for_pos; // (1)...az znajdziesz zwykly symbol na ktoryms ringu linii
				break;
			}

			if (count_special == board->Get_Length())  // jesli linia sklada sie tyko z symboli specjalnych
			{
				active.line[active.counter] = num_line; // dopisz do listy wystepujacych linii linie cala zlozona z symboli specjalnych
				active.symb[active.counter] = special; // cala zlozona z symboli specjalnych
				int const LENGTH_ON_TAB = 3;      // dlugosc linii 3 odpowiada 0 pozycji w tablicy rtp
				active.length[active.counter] = board->Get_Length() - LENGTH_ON_TAB; // o maksymalnej dlugosci

				++active.counter;
				continue; // sprawdzaj nastepna linie
			}

			Check_Length(num_line, look_for_symb, special); // sprawdza jakiej dlugosci jest linia
		}
	}

	int Lines::Random_Not_Used()
	{
		// losowanie z zakresu

		int result;
		result = rand() % num_lines;

		if (Check_Used(result)) // jesli zostala uzyta w tym cyklu, znajdz nablizsza nieuzyta
		{
			int memo_result;
			memo_result = result; // zapamietaj wylosowana

			do
			{
				++result;
				if (result > num_lines) // jesli przekroczy zakres
				{
					int const FIRST_ITEM = 0;
					result = FIRST_ITEM;
				}

				if (result == memo_result) // jesli wszystkie zostaly zuzyte, wyczysc liste uzyc
				{
					Reset_Used();
				}

			} while (Check_Used(result)) // sprawdz czy zostala zuzyta
				;
		}

		return result;
	}

	void Lines::Reset_Used()
	{
		// czyszczenie listy uzyc

		int const FIRST_ITEM = 0;
		for (int line = FIRST_ITEM; line < num_lines; ++line)
		{
			used[line] = false;
		}

		left = num_lines; // ustawienie licznika uzyc
	}

	void Lines::Print_on_Board(int symbol, int line, int length)
	{
		// wpisuje linie na tablicy wyniku

		int const LENGTH_ON_TAB = 3; // dlugosc lini 3 odpowiada 0 pozycji w tablicy rtp

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
	const int FIRST_SHAPE = 0;
	const int LAST_SHAPE = num_lines - 1;
	TEST_RANGE(line, FIRST_SHAPE, LAST_SHAPE)
	int max_length = board->Get_Length() - LENGTH_ON_TAB;
	TEST_RANGE(length, FIRST_SHAPE, max_length)
#endif

		Position pos;
		int const BOARD_START = 0;
		int real_length = length + LENGTH_ON_TAB; // ilosc symboli skladajacych sie na dlugosc linii  

		for (int ring = BOARD_START; ring < real_length; ++ring) // rysowanie symboli liini na poszczegolnych ringach na tablicy wynikow
		{
			pos.x = ring;
			pos.y = shape[line][ring];

			board->Set_Symbol(pos, symbol);
		}
	}



	int Lines::Get_Symb_Pos(int line, int ring)
	{
		// zwraca pozycje symbolu z ksztaltu linii

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		int const FIRST_SHAPE = 0;
		int const LAST_SHAPE = num_lines - 1;
		TEST_RANGE(line, FIRST_SHAPE, LAST_SHAPE)
		int const LAST_RING = ARRAY_SECOND(shape);
		TEST_RANGE(ring, FIRST_SHAPE, LAST_RING)
#endif

		return  shape[line][ring];;
	}

	int Lines::Get_Num_Lines()
	{
		// zwraca ilosc rodzajow linii wystepujacych na danym slocie

		return num_lines;
	}

	int Lines::Get_Active_Num()
	{
		// podaje ilosc wyszukanych symbolii

		return active.counter;
	}

	int Lines::Get_Active_Symb(int counter)
	{
		// podaje symbol, z ktorego sklada sie linia

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
	const int FIRST_SYMB = 0;
	const int LAST_SYMB = active.counter - 1;
	TEST_RANGE(counter, FIRST_SYMB, LAST_SYMB)
#endif

		return active.symb[counter];
	}

	int Lines::Get_Active_Line(int counter)
	{
		// podaje rodzaj linii

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_SYMB = 0;
		const int LAST_SYMB = active.counter - 1;
		TEST_RANGE(counter, FIRST_SYMB, LAST_SYMB)
#endif

		return active.line[counter];
	}

	int Lines::Get_Active_Length(int counter)
	{
		// podaje dlugosc linii

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int FIRST_SYMB = 0;
		const int LAST_SYMB = active.counter - 1;
		TEST_RANGE(counter, FIRST_SYMB, LAST_SYMB)
#endif

		return active.length[counter];
	}

	int Lines::Get_Left_Percent()
	{
		int in_percent = int(float(left) / float(num_lines) * 100.0f);

		return in_percent;
	}
}