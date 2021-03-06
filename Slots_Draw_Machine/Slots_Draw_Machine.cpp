// Slots_Draw_Machine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Slot_01.h"
#include "Slot_02.h"
#include "Slot_03.h"
#include "Slot_04.h"
#include "Slot_05.h"
#include "Slot_06.h"

using namespace std;
using namespace Slots_Draw_Machine;


int main()
{	
	Slots_Draw_Machine::Slot_04 slot;

	{
		// zestaw parametrow ktore musza zostac nadane slotowi przed losowaniami
		// i powinny zostac zapamietanie w save'je

		slot.game.slot_all_games = 0; // dla kazdego slotu intywidualnie
		slot.game.slot_today_games = 0;

		slot.game.credits = 500000; // raz dla wszystkich slotow
		slot.game.experience = 0;
		slot.game.all_games = 0;
		slot.game.all_today_games = 0;
		slot.game.raise = 0;
		slot.game.free_spins = 0;

		slot.game.can_lose = false; // opuszczenie flagi wymuszajacej przegrana
	}

	slot.Draw();

	return 0;
}