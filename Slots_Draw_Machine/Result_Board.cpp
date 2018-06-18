#include "stdafx.h"
#include "Result_Board.h"

namespace Slots_Draw_Machine
{
	Result_Board::Result_Board(Symbols &symb)
	{
		int const START_POSITION = 0;
		length = START_POSITION;

		this->symb = &symb;

		backside_on = false; // zeby wyczyscic tylna tablice wynikow

		Clear();
	}

	void Result_Board::Note_from_Posit(Position &pos)
	{
		// zaznacza symbol z pozycji w obiekcie 'exam'

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int BOARD_START = 0;
		ASSERT(length == BOARD_START, "Nie ustawiono ksztaltu tablicy wynikowej!!!")
#endif

		if (On_Shape(pos) == true) // jesli pozycja znajduje sie w ksztalcie tablicy wynikowej
		{
			int symb_num = Get_Symbol(pos); // to sprawdz symbol z pozycji

			if (exam.used[symb_num] == false) // jesli nie byl jeszcze zaznaczony
			{
				exam.used[symb_num] = true; // to zaznacz
				--exam.left; // i zmniejsz licznik pozostalych nieodnalezionych
			}
		}
	}

	void Result_Board::Clear_Used_Symb()
	{
		// czysci tablice uzyc w obiekcie 'exam'

		int const FIRST_ITEM = 0; 
		exam.used[FIRST_ITEM] = true; // zacznaczenie pustego symbolu jako uzyty, aby nie wplywal na sprawdzenia

		int const FIRST_EMPTY = 1; // symbol 0 oznacza pusta komorke
		for (int i = FIRST_EMPTY; i < symb->Get_Size(); ++i) // czyszczenie tablicy
		{
			exam.used[i] = false;
		}

		exam.left = symb->Get_Size() - FIRST_EMPTY; // reset ilosci pozostalych symboli
	}

	void Result_Board::Exam_Cells_Around(Position &pos)
	{
		// sprawdza zawartosc wszystkich sasiadujacych komorek po bokach i skosie

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int BOARD_START = 0;
		ASSERT(length == BOARD_START, "Nie ustawiono ksztaltu tablicy wynikowej!!!")
#endif

		Clear_Used_Symb(); // wyczyszczenie listy uzyc

		Position check;
		int const MOVE_ONE_CELL = 1;

		// LEWY RING
		int tested_ring = pos.x - MOVE_ONE_CELL;
		if (tested_ring >= 0) // jesli testowany ring nie wychodzi poza tablice wynikow
		{
			check.x = tested_ring; // ustaw pozycje do sprawdzenia na ring z lewej

			int const START_RING = 0; // poczatek ringu
			int end_of_ring = height.val[tested_ring]; // koniec ringu

			for (int position_y = START_RING; position_y < end_of_ring; ++position_y) // sprawdzenie calego ringu 
			{
				check.y = position_y;
				Note_from_Posit(check);
			}
		}

		// SRODEK
		check.x = pos.x; //  ustaw pozycje do sprawdzenia na ring, na ktorym znajduje sie sprawdzana komorka
		check.y = pos.y - MOVE_ONE_CELL;
		Note_from_Posit(check); // sprawdz symbol powyzej
		check.y = pos.y + MOVE_ONE_CELL;
		Note_from_Posit(check); // sprawdz symbol ponizej

		// PRAWY RING
		tested_ring = pos.x + MOVE_ONE_CELL;
		check.x = tested_ring; // ustaw pozycje do sprawdzenia na ring z prawej

		if (tested_ring < length) // jesli sprawdzana komorka nie jest ostatnia na szeroksi slotu
		{
			check.x = tested_ring; // ustaw pozycje do sprawdzenia na ring z lewej

			int const START_RING = 0; // poczatek ringu
			int end_of_ring = height.val[tested_ring]; // koniec ringu

			for (int position_y = START_RING; position_y < end_of_ring; ++position_y) // sprawdzenie calego ringu 
			{
				check.y = position_y;
				Note_from_Posit(check);
			}
		}
	}

	int Result_Board::Get_Rand_from_Exam()
	{
		// losuje symbol z pozostalej puli z tablicy 'exam'

		int const ONE_FOR_RAND = 1;
		int const LAST_SYMB = symb->Get_Size();

		int const NO_SYMBOL = -1; // na wypadek gdyby nie bylo mozliwe podanie zadnego symbolu
		int look_for_symb = NO_SYMBOL;

		int const NONE = 0;
		if (exam.left != NONE) // jesli pozostaly jakies symbole do losowania
		{
			bool occupied;
			do {

				int const FIRST_SYMB = 1;
				look_for_symb = (rand() % (LAST_SYMB - FIRST_SYMB)) + FIRST_SYMB; // losowanie od 1 do ostatniego

				occupied = exam.used[look_for_symb];

			} while (occupied == true) // czy symbol wystepuje na liscie 'exam'
				;
		}

		return look_for_symb;
	}

	void Result_Board::Set_Shape(Items &size)
	{
		// nadawanie ksztaltu

		length = size.counter;

		const int BOARD_START = 0;
		for (int i = BOARD_START; i < length; ++i)
		{
			height.val[i] = size.val[i];
		}
	}


	void Result_Board::Clear()
	{
		// zerowanie tablicy

		const int BOARD_START = 0;
		const int BOARD_WIDE = ARRAY_LENGTH(result);
		const int BORAD_HEIGHT = ARRAY_SECOND(result);
		for (int i = BOARD_START; i < BOARD_WIDE; ++i) // czyszczenie frontowej
		{
			for (int j = BOARD_START; j < BORAD_HEIGHT; ++j)
			{
				result[i][j] = 0;
			}
		}

		if (backside_on == true)
		{
			for (int i = BOARD_START; i < BOARD_WIDE; ++i) // czyszczenie tylnej
			{
				for (int j = BOARD_START; j < BORAD_HEIGHT; ++j)
				{
					backside[i][j] = 0;
				}
			}

			backside_on = false; // opuszczenie flagi uzycia tylnej tablicy
		}
	}

	bool Result_Board::Fill_Up()
	{
		// dorysowuje randomowe symbole w pustych miejscach, tak aby nie powstalo wiecej linii

		Position pos;
		int symb;

		int const FIRST_ITEM = 0;
		for (int x = FIRST_ITEM; x < length; ++x)
		{
			for (int y = FIRST_ITEM; y < height.val[x]; ++y) // sprawdza kazda komorke na tablicy wynikow
			{
				pos.x = x;
				pos.y = y;

				symb = Get_Symbol(pos); // sprawdz jaki symbol jest na pozycji

				int const EMPTY = 0;
				if (symb == EMPTY) // jesli komorka jest pusta
				{
					Exam_Cells_Around(pos); // sprawdz jakie symbole znajduja sie wokol

					symb = Get_Rand_from_Exam(); // wylosuj symbol inny niz w sasiednich komorkach

					int const NO_SYMBOL = -1;
					if (symb == NO_SYMBOL) // jesli nie pozostal zaden nieuzyty symbol
					{
						return false;
					}

					Set_Symbol(pos, symb); // wrysuj wylosowany symbol
				}
			}
		}
		return true;
	}



	bool Result_Board::On_Shape(Position &pos)
	{
		// sprawdza czy pozycja znajduje sie na powierzchni ksztaltu slota

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		int const NO_SHAPE = 0;
		ASSERT(length == NO_SHAPE, "Nie ustawiono ksztaltu tablicy wynikowej!!!")
#endif

			const int BOARD_START = 0;
		const int BOARD_END = length - 1;
		bool result = true;
		if (pos.x < BOARD_START || pos.x > BOARD_END) // jesli miesci sie na szerokosci tablicy
		{
			result = false;
		}

		const int RING_START = 0;
		const int RING_END = height.val[pos.x] - 1;
		if (pos.y < RING_START || pos.y > RING_END) // jesli miesi sie na danym ringu
		{
			result = false;
		}

		return result;
	}

	int Result_Board::Get_Length()
	{
		// podaje szerokosc tablicy

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int BOARD_START = 0;
		ASSERT(length == BOARD_START, "Nie ustawiono ksztaltu tablicy wynikowej!!!")
#endif

			return length;
	}

	int Result_Board::Get_Height(int ring)
	{
#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int BOARD_START = 0;
		const int BOARD_END = length - 1;
		TEST_RANGE(ring, BOARD_START, BOARD_END)
#endif

		int ring_height = height.val[ring];

		return ring_height;
	}

	int Result_Board::Get_Symbol(Position &pos)
	{
		// sprawdznie symbolu z tablicy wynikowej

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int BOARD_START = 0;
		const int BOARD_END = length - 1;
		const int RING_START = 0;
		const int RING_END = height.val[pos.x] - 1;
		TEST_RANGE(pos.x, BOARD_START, BOARD_END)
			TEST_RANGE(pos.y, BOARD_START, RING_END)
			ASSERT(length == BOARD_START, "Nie ustawiono ksztaltu tablicy wynikowej!!!")
#endif

			return result[pos.x][pos.y];
	}

	void Result_Board::Set_Symbol(Position &pos, int symbol)
	{
		// wstawianie symbolu na tablice wynikowa

#if RANGE == ON //--- SPRAWDZENIE POPRAWNOSCI WPROWADZONYCH DANYCH
		const int BOARD_START = 0;
		const int BOARD_END = length - 1;
		const int RING_START = 0;
		const int RING_END = height.val[pos.x] - 1;
		TEST_RANGE(pos.x, BOARD_START, BOARD_END)
		TEST_RANGE(pos.y, RING_START, RING_END)
		ASSERT(length == BOARD_START, "Nie ustawiono ksztaltu tablicy wynikowej!!!")
#endif

			result[pos.x][pos.y] = symbol; // na tablicy wynikow
	}
}






