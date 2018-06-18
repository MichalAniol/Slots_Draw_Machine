#pragma once
#include "Slot_Base.h"

namespace Slots_Draw_Machine
{
	class Slot_03 : public Slot_Base
	{
#if TESTS == ON
	protected:
#else
	private:
#endif

		Scenario wild_start;
		Scenario wild;
		Scenario mystery_start;
		Scenario mystery;
		Scenario bonusgame_3_start;
		Scenario bonusgame_3;
		Scenario bonusgame_1or2_start;
		Scenario bonusgame_1or2;
		Scenario pic_anubis_start;
		Scenario pic_anubis;
		Scenario pic_isis_start;
		Scenario pic_isis;

		void Establish_Parameters();
		void Read_File();

		void Execute_Scenario();
		float Scenario_Multipler();

		void Convert_Symbols();

	public:
		Slot_03();
	};
}