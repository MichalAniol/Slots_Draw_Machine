// Test_Komponetu.cpp: definiuje punkt wejścia dla aplikacji konsolowej.
//

#include "stdafx.h"
#include "../../Slots_Draw_Machine/Lines.cpp"
#include "../../Slots_Draw_Machine/Lines_Designer.cpp"
#include "../../Slots_Draw_Machine/Preprocesor.cpp"
#include "../../Slots_Draw_Machine/Probability.cpp"
#include "../../Slots_Draw_Machine/Result_Board.cpp"
#include "../../Slots_Draw_Machine/Scenario.cpp"
#include "../../Slots_Draw_Machine/Scenario_Holder.cpp"
#include "../../Slots_Draw_Machine/Multipler.cpp"
#include "../../Slots_Draw_Machine/Slot_01.cpp"
#include "../../Slots_Draw_Machine/Slot_02.cpp"
#include "../../Slots_Draw_Machine/Slot_03.cpp"
#include "../../Slots_Draw_Machine/Slot_04.cpp"
#include "../../Slots_Draw_Machine/Slot_05.cpp"
#include "../../Slots_Draw_Machine/Slot_06.cpp"
#include "../../Slots_Draw_Machine/Slot_Base.cpp"
#include "../../Slots_Draw_Machine/Specials.cpp"
#include "../../Slots_Draw_Machine/Symbols.cpp"
#include "../../Slots_Draw_Machine/Win_Cycle.cpp"

using namespace std;


void Performation_Time(int bet_num, int slot_num);
void Show_Draws(int bet_num, int slot_num);
void Show_Board(int bet_num, int slot_num);
void Set_Bet(int max_bet_choose, bool show, int slot_num);

Slots_Draw_Machine::Slot_Base *slot[6];


int main()
{
	slot[0] = new Slots_Draw_Machine::Slot_01;
	slot[1] = new Slots_Draw_Machine::Slot_02;
	slot[2] = new Slots_Draw_Machine::Slot_03;
	slot[3] = new Slots_Draw_Machine::Slot_04;
	slot[4] = new Slots_Draw_Machine::Slot_05;
	slot[5] = new Slots_Draw_Machine::Slot_06;

	int const SLOT_NUM = 2; // <------------ WYBOR SLOTU DO TESTOW

	// slot 1: 0-3 // ilosc rodzajow zakladow dla danego slotu
	// slot 2: 0-5
	// slot 3: 0-7
	// slot 4: 0-6
	// slot 5: 0-6
	// slot 6: 0-9
	int const NUM_OF_BET = 1; // <---------- WYBOR MAKSYMALNEJ WYSOKOSCI ZAKLADU DLA TESTOW


	enum test_chose
	{
		test_wydajnosciowy = 1,
		test_rtp = 2,
		test_tablicy_wynikow = 3
	};
	//***************************************
	//***************************************

	test_chose what_test = // <------------- WTBOR RODZAJU TESTU
		test_wydajnosciowy;

	//***************************************
	//***************************************

	{
		// zestaw parametrow ktore musza zostac nadane slotowi przed losowaniami
		// i powinny zostac zapamietanie w save'je

		slot[SLOT_NUM]->game.slot_all_games = 0; // dla kazdego slotu intywidualnie
		slot[SLOT_NUM]->game.slot_today_games = 0;

		slot[SLOT_NUM]->game.credits = 500000; // raz dla wszystkich slotow
		slot[SLOT_NUM]->game.experience = 0;
		slot[SLOT_NUM]->game.all_games = 0;
		slot[SLOT_NUM]->game.all_today_games = 0;

		slot[SLOT_NUM]->game.raise = 0;
		slot[SLOT_NUM]->game.free_spins = 0;

		slot[SLOT_NUM]->game.can_lose = false; // opuszczenie flagi wymuszajacej przegrana
	}




	switch (what_test)
	{
	case 1: 
		Performation_Time(NUM_OF_BET, SLOT_NUM);
		break;
	case 2: 
		Show_Draws(NUM_OF_BET, SLOT_NUM);
		break;
	case 3: 
		Show_Board(NUM_OF_BET, SLOT_NUM);
		break;
	default:
		cout << "Nie wybrano testu !!!\n\n\n";
		break;
	}
	
	for (;;)

    return 0;
}

void Performation_Time(int bet_num, int slot_num)
{

	long counter = 0;

	int const NUM_LOW = 2;
	float const SCOPE_LOW = 1;

	int const NUM_MIDDLE = 20;
	float const SCOPE_MIDDLE = 0.1f;

	int const NUM_HIGH = 8;
	float const SCOPE_HIGH = 1;

	int const NUM_HIGHEST = 5;
	float const SCOPE_HIGHEST = 5;

	int take[NUM_LOW + NUM_MIDDLE + NUM_HIGH + NUM_HIGHEST + 1];

	for (int i = 0; i < ARRAY_LENGTH(take); ++i)
	{
		take[i] = 0;
	}

	clock_t time;
	clock_t last_time;
	float how_long_mem = 0;
	float how_long;

	last_time = clock();
	int rtp;

	for ( ; ; )
	{
		for ( ; ; )
		{
			Set_Bet(bet_num, false, slot_num);

			int cycle = 1000;
			if (counter % cycle == 0)
			{
				time = clock();
				how_long = float((time - last_time) / 10.0f);
				if (how_long > how_long_mem)
				{
					cout << "#> " << how_long << " <#" << endl;
					how_long_mem = how_long;
				}

				float scope = 0;
				int num = 0;

				for (int i = 0; i < NUM_LOW; ++i)
				{
					if (how_long >= scope && how_long < scope + SCOPE_LOW) ++take[num];
					scope += SCOPE_LOW;
					++num;
				}

				for (int i = 0; i < NUM_MIDDLE; ++i)
				{
					if (how_long >= scope && how_long < scope + SCOPE_MIDDLE) ++take[num];
					scope += SCOPE_MIDDLE;
					++num;
				}

				for (int i = 0; i < NUM_HIGH; ++i)
				{
					if (how_long >= scope && how_long < scope + SCOPE_HIGH) ++take[num];
					scope += SCOPE_HIGH;
					++num;
				}

				for (int i = 0; i < NUM_HIGHEST; ++i)
				{
					if (how_long >= scope && how_long < scope + SCOPE_HIGHEST) ++take[num];
					scope += SCOPE_HIGHEST;
					++num;
				}

				if (how_long >= scope) ++take[ARRAY_LENGTH(take) - 1]; // ostatnia komorka

				last_time = time;
			}



			++counter;
			if (counter % 1000 == 0)
			{
				cout << ".";
			}

			if (counter % 100000 == 0)
			{
				cout << "\n\n------------ dla: " << double(counter) / 1000000 << " mln losowan ----------\n";
				float  scope = 0;
				int num = 0;
				for (int i = 0; i < NUM_LOW; ++i)
				{
					cout << scope << " - " << scope + SCOPE_LOW << ": " << take[num] * 10 << " tys. == " <<
						float(take[num]) / float(counter / 100) * 1000.0f << "%" << endl;
					scope += SCOPE_LOW;
					++num;
				}

				for (int i = 0; i < NUM_MIDDLE; ++i)
				{
					cout << scope << " - " << scope + SCOPE_MIDDLE << ": " << take[num] * 10 << " tys. == " <<
						float(take[num]) / float(counter / 100) * 1000.0f << "%" << endl;
					scope += SCOPE_MIDDLE;
					++num;
				}

				for (int i = 0; i < NUM_HIGH; ++i)
				{
					cout << scope << " - " << scope + SCOPE_HIGH << ": " << take[num] * 10 << " tys. == " <<
						float(take[num]) / float(counter / 100) * 1000.0f << "%" << endl;
					scope += SCOPE_HIGH;
					++num;
				}

				for (int i = 0; i < NUM_HIGHEST; ++i)
				{
					cout << scope << " - " << scope + SCOPE_HIGHEST << ": " << take[num] * 10 << " tys. == " <<
						float(take[num]) / float(counter / 100) * 1000.0f << "%" << endl;
					scope += SCOPE_HIGHEST;
					++num;
				}

				cout << scope << " - nieskonczonosc" << ": " << take[ARRAY_LENGTH(take) - 1] << " == " <<
					float(take[ARRAY_LENGTH(take) - 1]) / float(counter / 1000) * 100.0f << "%" << endl;

				cout << "\nilosc nieudanych losowan i rodzaje przesuniec prawdopodobienstw:\n";
				cout << ". = 1000 losowan\n";
				cout << ": - " << slot[slot_num]->drop_counter[0] << " - podnies oczekiwany wynik o 10%" << endl;
				cout << "a - " << slot[slot_num]->drop_counter[1] << " - podnies oczekiwany wynik o kolejne 20% i zresetuj prawdopodobienstwo, kore ma najmniejsza pole mozliwych losowan" << endl;
				cout << "b - " << slot[slot_num]->drop_counter[2] << " - podnies oczekiwany wynik o kolejne 30% i zresetuj kolejne prawdopodobienstwo, kore ma najmniejsza pole mozliwych losowan" << endl;
				cout << "c - " << slot[slot_num]->drop_counter[3] << " - podnies oczekiwany wynik o kolejne 50% i zresetuj wszystkie prawdopodobienstwa" << endl;
				cout << "# - " << slot[slot_num]->drop_counter[4] << " - wlacz 'knock_down' i wylosjuj kilka lini i tym samym symbolu poza prawdopodobienstwami" << endl;

				cout << "--------------------------------------------\n";
			}

			rtp = slot[slot_num]->Draw();
			int const NO_DRAW = -1;
			if (rtp == NO_DRAW)
			{
				break;
			}
		}

		cout << "Skonczyly sie kredyty !!!" << endl;

		int const CHARGE_A_BIT = 1000000;
		slot[slot_num]->game.credits += CHARGE_A_BIT;

		cout << "Doladowanie kredytow o 100 tys !!!" << endl;
	}
}

void Show_Draws(int bet_num, int slot_num)
{
	int rtp;
	for (; ; )
	{
		for (; ; )
		{
			Set_Bet(bet_num, true, slot_num);

			rtp = slot[slot_num]->Draw();
			int const NO_DRAW = -1;
			if (rtp == NO_DRAW)
			{
				break;
			}

			cout
				<< "game: " << slot[slot_num]->game.all_games << ", "
				<< "RTP: " << rtp << ", "
				<< "credits: " << slot[slot_num]->game.credits << ", "
				<< "exp: " << slot[slot_num]->game.experience;


			//if (rtp > 0) cout << "8"; else cout << " ";

			if (slot[slot_num]->rtp.bonus > 0) cout << ", >>> BONUS: " << slot[slot_num]->rtp.bonus << " <<<";
			if (slot[slot_num]->game.free_spins > 0) cout << ", ]FS:" << slot[slot_num]->game.free_spins << "[";
			if (rtp > 0) cout << "  <---------- WIN !!!";

			cout << endl;

		//	int const NO_CREDITS = 0;
		//	TEST_RANGE(slot.game.credits, NO_CREDITS, 15000000);
		}

		cout << "Skonczyly sie kredyty !!!" << endl;

		for (int i = 0; i < 1000; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				for (int k = 0; k < 1000; k++)
				{
				}
			}
		}

		int const CHARGE_A_BIT = 100000;
		slot[slot_num]->game.credits += CHARGE_A_BIT;

		cout << "Doladowanie kredytow o 100 tys !!!" << endl;

		for (int i = 0; i < 1000; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				for (int k = 0; k < 1000; k++)
				{
				}
			}
		}
	}
}

void Show_Board(int bet_num, int slot_num)
{
	int highest_hight = 0;
	
	for (int i = 0; i < slot[slot_num]->board.Get_Length(); i++)
	{
		if (slot[slot_num]->board.Get_Height(i) > highest_hight)
		{
			highest_hight = slot[slot_num]->board.Get_Height(i);
		}
	}

	int rtp;
	for (; ; )
	{
		for (; ; )
		{
			Set_Bet(bet_num, false, slot_num);

			rtp = slot[slot_num]->Draw();
			int const NO_DRAW = -1;
			if (rtp == NO_DRAW)
			{
				break;
			}

			cout << endl; // rysuj slot
			int const FIRST_CELL = 0;
			for (int y = FIRST_CELL; y < highest_hight; ++y)
			{
				for (int x = FIRST_CELL; x < slot[slot_num]->board.Get_Length(); ++x)
				{
					cout << slot[slot_num]->result[x][y] << ".";
				}
				cout << endl;
			}

			if (slot[slot_num]->back_on == true)
			{
				cout << endl; // rysuj slot
				int const FIRST_CELL = 0;
				for (int y = FIRST_CELL; y < highest_hight; ++y)
				{
					for (int x = FIRST_CELL; x < ARRAY_LENGTH(slot[slot_num]->board.result); ++x)
					{
						cout << slot[slot_num]->back[x][y] << ".";
					}
					cout << endl;
				}
			}

			for (int i = 0; i < slot[slot_num]->lines.Get_Active_Num(); ++i) // wypisz trafione linie
			{
				cout << "symb:" << slot[slot_num]->lines.Get_Active_Symb(i)
					<< "  line:" << slot[slot_num]->lines.Get_Active_Line(i)
					<< "  long:" << slot[slot_num]->lines.Get_Active_Length(i) << endl;
			}

			cout
				<< "game: " << slot[slot_num]->game.all_games << ", " // wypisz wynik
				<< "RTP: " << rtp << ", "
				<< "credits: " << slot[slot_num]->game.credits << ", "
				<< "exp: " << slot[slot_num]->game.experience;

			if (slot[slot_num]->rtp.bonus > 0) cout << ", >>> BONUS: " << slot[slot_num]->rtp.bonus << " <<<";
			if (slot[slot_num]->game.free_spins > 0) cout << ", ]FS:" << slot[slot_num]->game.free_spins << "[";
			if (rtp > 0) cout << "  <---------- WIN !!!"; // wskarz wygrana

			cout << endl;

			//	int const NO_CREDITS = 0;
			//	TEST_RANGE(slot.game.credits, NO_CREDITS, 15000000);
		}

		cout << "Skonczyly sie kredyty !!!" << endl;

		for (int i = 0; i < 1000; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				for (int k = 0; k < 1000; k++)
				{
				}
			}
		}

		int const CHARGE_A_BIT = 100000;
		slot[slot_num]->game.credits += CHARGE_A_BIT;

		cout << "Doladowanie kredytow o 100 tys !!!" << endl;

		for (int i = 0; i < 1000; i++)
		{
			for (int j = 0; j < 1000; j++)
			{
				for (int k = 0; k < 1000; k++)
				{
				}
			}
		}
	}
}

void Set_Bet(int max_bet_choose, bool show, int slot_num)
{
	if (slot[slot_num]->game.credits < slot[slot_num]->param.bet.value.val[slot[slot_num]->param.bet.choose]) // obniz jesli musisz
	{
		do
		{
			int const FIRST_BET = 0;
			if (slot[slot_num]->param.bet.choose == FIRST_BET) break;

			--slot[slot_num]->param.bet.choose;

			if (show == true)
			{
				cout << "|###.>";
			 	cout << slot[slot_num]->param.bet.value.val[slot[slot_num]->param.bet.choose] << "<.###|";
			}

		} while (slot[slot_num]->game.credits <= slot[slot_num]->param.bet.value.val[slot[slot_num]->param.bet.choose])
			;
	}

	if (slot[slot_num]->param.bet.choose < max_bet_choose) // podwyz jesli mozesz
	{
		if (slot[slot_num]->param.bet.value.val[slot[slot_num]->param.bet.choose + 1] < slot[slot_num]->game.credits)
			++slot[slot_num]->param.bet.choose;
	}

	int const NONE = 0;
	if (slot[slot_num]->param.bet.choose < NONE) slot[slot_num]->param.bet.choose = NONE; // na wszelki wypadek
	if (slot[slot_num]->param.bet.choose > max_bet_choose) slot[slot_num]->param.bet.choose = max_bet_choose;

	//cout << slot.param.bet.value.val[slot.param.bet.chose] << "<>";

}
