#pragma once
#include "Lines.h"
#include "Lines_Designer.h"

namespace Slots_Draw_Machine
{
	class Specials
	{
		// zdarzenia specjalne wlaczane przez scenariusze

		Lines_Designer *design; // wskazniki do komunikacji z obiektami
		Symbols *symb;
		Result_Board *board; 
		Slot_Parameter *param;
		Game *game;
		RTP *rtp;

		bool wild_used[3][5];          // pamiec wystapienia danej kombinacji
		int wild_left;                // licznik pozostalych do wykorzystania kombinacji
		const bool WILD_SETUP[2][5]; // prawdopodobienstwo wystapienia symboli bezposrenio nad soba

		bool mummy_smal[10];        // pamiec losowan mumi na 4 slocie dla zachowania stabilnej roznorodnosci
		int num_mummy_smal;       // licznik uzytych komorek pamieci
		bool mummy_medium[10];   // pamiec losowan mumi na 4 slocie dla zachowania stabilnej roznorodnosci
		int num_mummy_medium;   // licznik uzytych komorek pamieci
		bool mummy_big[10];    // pamiec losowan mumi na 4 slocie dla zachowania stabilnej roznorodnosci
		int num_mummy_big;    // licznik uzytych komorek pamieci
			
		Probability mystery;

		void Copy_To_Back();
		Position Draw_Position();
		void Clear_Wild_Used();
		void Wilds_Positons(int num, int setup, int spec);
		void Draw_Vertical(int num, int symbol);
		void Draw_Horizontal(int num, int symbol);
		int Mummy_Not_Used(bool *tab, int &num);

	public:
		Specials(
			Lines_Designer &design,
			Symbols &symb,
			Result_Board &board, 
			Slot_Parameter &param, 
			Game &game, 
			RTP &rtp);

		void Expand_Wild(int spec);
		void Wild(int spec);
		void Free_Spins(int num, int spec);
		void Bonus_Game(int num, int spec);
		void Mystery(int spec);
		void Pictures_1_x_2_or_3(int symbol);

		void Mummy_Smal(int spec); // tylko dla slotu 4
		void Mummy_Medium(int spec); // tylko dla slotu 4
		void Mummy_Big(int spec); // tylko dla slotu 4
		void Mummy_Restart();

		void Wadjet(int spec);
	};
}
