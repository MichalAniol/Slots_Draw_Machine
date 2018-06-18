#pragma once
#include "Slot_Base.h"

namespace Slots_Draw_Machine
{
	class Slot_01 : public Slot_Base
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
		Scenario freespin_1or2_start;
		Scenario freespin_1or2;

		void Establish_Parameters();
		void Read_File();

		void Execute_Scenario();
		float Scenario_Multipler();


	public:
		void Convert_Symbols();
		Slot_01();
	};
}