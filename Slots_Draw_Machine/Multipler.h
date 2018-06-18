#pragma once
#include "Specials.h"

namespace Slots_Draw_Machine
{
	class Multipler
	{
	private:
		Lines_Designer *design; // wskazniki do komunikacji z obiektem
		Slot_Parameter *param;
		Game *game;
		Shield *shield;
		RTP *rtp;
		Mega_Wins *mega_wins;
		Score *score;

	public:
		float average_bet;

		Multipler(
			Lines_Designer &design,
			Slot_Parameter &param,
			Game &game,
			Shield &shield,
			RTP &rtp,
			Mega_Wins &mega_wins,
			Score &score);

		float Major();
		float Rise_Cycle();
		void Rise_Counter();
		void Rise_Start(int tours);
		float Score_Goal();
		void Score_Counter();
	};
}