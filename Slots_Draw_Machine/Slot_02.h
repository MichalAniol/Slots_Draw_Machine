#pragma once
#include "Slot_Base.h"

namespace Slots_Draw_Machine
{
	class Slot_02 : public Slot_Base
	{
#if TESTS == ON
	protected:
#else
	private:
#endif

		Scenario wild_start;
		Scenario wild;
		Scenario freespin_3_start;
		Scenario freespin_3;
		Scenario bonusgame_3_start;
		Scenario bonusgame_3;
		Scenario freespin_1or2_start;
		Scenario freespin_1or2;
		Scenario bonusgame_1or2_start;
		Scenario bonusgame_1or2;

		void Establish_Parameters();
		void Read_File();

		void Execute_Scenario();
		float Scenario_Multipler();

		void Convert_Symbols();

	public:
		Slot_02();
	};
}