#pragma once
#include "Data_Holders.h"

namespace Slots_Draw_Machine
{
	class Probability
	{
		// sluzy do tak zwanego 'kwantowego prawdopodobienstwa'
		// losuje 1-na liczbe na 100 i zaznacza na tablicy, ktora liczba zostala uzyta
		// gdy cala tablica 100 liczb zostanie wyczerpana, nastepuje jej wyczyszczenie
		// pozwala to na utrzymanie stablinie rozlozonej roznorodnosci losowan

#if TESTS == ON
	protected:
#else
	private:
#endif
		int const LAST_HAPPEN; // ostatni element tablicy 'happened'

		bool used[100];      // tablica uzyc procentow
		int left;           // ilosc pozostalych procentow do uzycia
		int scope[5];      // progi okreslane w procentach
		int num_scope;    // ilosc progow
		int result;      // wynik losowan

		int Draw_Percent();
		bool Check_Use(int item);

	public:
		Probability();

		void Set_Scope(int val);
		void Set_Scope(Items &val);

		void Mark_Used();
		void Mark_Used(int item);
		int Draw_Scope();
		void Reset_Used();

		// 'interface' do pobierania zmiennej z obiektu tej klasy
		int Get_Resutl();
		int Get_Left();
	};
}


