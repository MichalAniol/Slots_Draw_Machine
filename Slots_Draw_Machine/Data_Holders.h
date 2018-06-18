#pragma once

#include "Preprocesor.cpp"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

namespace Slots_Draw_Machine
{
	struct Position
	{
		int x;
		int y;
	};

	struct Items
	{
		int counter;
		int val[10];
	};


	//*************************************************************
	// parametry wspolne dla wszystkich slotow

	struct Game
	{
		int static credits;              // posiadane kredyty
#if TESTS == ON                         // testy powyzej kilku mln potrzebuja typu long
		long static experience;        // doswiadczenie
		long static all_games;        // ilosc wszystkich rozegranych gier
		long static all_today_games; // ilosc wszystkich rozegranych gier w danym dniu
#else
		int static experience;       
		int static all_games;   
		int static all_today_games; 
#endif
		bool static can_lose;   // flaga dazenia do przegranej
		int static free_spins; // ilosc darmowych tur 
		int static raise;     // ilosc tur o podwyzsonych wygranych 

		int slot_all_games; // parametry indywidualne slotu, do podania przed pierwszymi losowaniami
		int slot_today_games;
	};

	struct Score
	{
		int static goal;         // poziom do jakiego dazy gra
		int static reduction;   // o ile sie zmienia 'goal', zmiana nastepuje po 'cycle' tur
		int static max;        // maksymalny poziom przy zmianie
		int static min;       // minimalny poziom przy zmianie
		int static cycle;    // co ile tur nastepuje przeliczenie 'goal'
		int static counter; // licznik cyklu
	};

	struct Shield
	{
		int static when_game_start;	  // podwyzszenie expa na poczatku rozgrywki
		int static when_begin_today; // przez ile tur od rozpoczecia gry danego dnia wygrywa sie wiecej
		int static annul_after;     // wymuszenie przegranej po przekroczeniu tego poziomu expa
		int static after_lose;     // ilosc tur podniesienia mnoznika rtp po przegranej
		bool static must_win;    // flaga wymuszonej wygranej, gdy brakuje kredytow
	};

	struct Mega_Wins
	{
		int static length;        // co ile gier podwyzsza sie prawdopodobienstwo wyzszego rtp 
		int static noise;        // odchylenie
		int static ture_length; // przez ile gier jest podwyzszone rtp
		int static ture_noise; // odchylenie
	};


	//*************************************************************
	// parametry okreslane indywidualnie dla kazdego slotu

	struct Exp
	{
		int open_lev;      // doswiadczenie potrzebne do otworzenia tego slotu
		int for_next_lev; // doswiadczenie potrzebne do otworzenia kolejnego slotu
		int for_play;    // doswiadcze otrzymywane za kazde zagranie
		Items for_bet;  // doswiadczenie zalezne od wysokosci zakladu
	};

	struct Bet
	{
		int num;               // ilosc zakladow
		Items value;          // wysokosci zakladow
		int choose;          // zaklad, ktory jest aktualnie wybrany
		Items x_length[10]; // prawdopodobienstwo wystapienia dlugosci linii w zaleznosci od zakladu 
	};

	struct Cycle
	{
		int win;             // co ile zdarza sie wygrana
		int everyday_start; // przez ile tur wygrana jest zwiekszona kazdego dnia, gdy zaczynamy grac na danym slocie  
		int raise;         // o ile wzrastaja wygrane w czasie zwiekszonych wygranych
		bool raise_on;    // flaga wystapienia
		int reduce;      // o ile zmniejszaja sie wygrane w czasie zmniejszonych wygranych
		int counter;    // licznik tur
	};

	struct Slot_Parameter
	{
		Exp exp;
		Bet bet;
		Cycle cycle;

		Items num_cover;       // progi dla losowan prawdopodobienstawa, gdy linie sie pokrywaja
		Items num_not_cover;  // progi dla losowan prawdopodobienstawa, gdy linie sie nie pokrywaja

		int lines_cover;    // prawdopodobienstwo wystapienia linii pokrywajacych sie
		int sybols_spread; // wspolczynik rozlozenia prawdopodobienstwa wystepowania symboli
						  // czym nizszy, tym prawdopodobienstwo jest rozlozone rowno na kazdy symbol
						 // czym wyzsze, tym symbole o nizszym rtp sa losowane czesciej, a o wysokim rzadko
	};


	//*************************************************************
	// parametry dla tury

	struct Multi_Major
	{
		float go_up;    // mnoznik zwiekszajacy wygrana
		float go_down; //  mnoznik zmniejszajacy wygrana
	};

	struct RTP
	{
		int min;      // minimalne rtp dla tury
		int max;     // maksymalne rtp dla tury
		int scen;   // ile maksymalnie rtp otzymamy, jesli scenariusz wystepuje
		int bonus; // kredyty otrzymywane gdy wystepuje 'bonus game'

		Multi_Major multip;
	};


	//*************************************************************
	// dla obiektu 'Symbols'

	struct Used
	{
		bool percent[100]; // lista do zaznacznia uzycia danego procentu
		int scope[21];    // progi podane w procentach
		int left[21];    // ile znajduje sie procent na danym progu, zmniejszane po zuzyciu kazdego procenta
		int all_left;   // ile zostalo nieuzytych procentow ogolem

		int recently_scope[4];    // pamiec ostatnich wylosowanych progow (3 dla linii i 1 dla mystery symbol)
		int recently_percent[4]; // pamiec ostatnich wylosowanych procentow
	};


	//*************************************************************
	// dla obiektu 'Result_Board'

	struct Exam // do sprawdzania symboli znajdujacych sie na tablicy wynikow
	{
		bool used[14]; // lista do zaznaczania uzytych symboli
		int left;     // licznik pozostalych symboli do uzycia
	};


	//*************************************************************
	// dla obiektu 'Lines'

	struct Active // lista znalezionych linii na tablicy wynikow
	{
		int counter;       // ilosc linii
		int line[50];     // lista linii
		int symb[50];    // lista symboli
		int length[50]; // lista dlugosci linii
	};

	//*************************************************************
	// dla obiektu 'Lines_Designer'

	struct Mem
	{
		int size;         // ilosc wylosowanych
		int symb[5];     // dopasowane symbole
		int line[5];    // dopasowane linie
		int length[5]; // dopasowane dlugosci
	};
}


