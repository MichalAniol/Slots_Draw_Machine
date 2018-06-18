#pragma once
#include "Data_Holders.h"

namespace Slots_Draw_Machine
{
	class Symbols
	{
		// lista symboli wystepujacych na danym slocie
		// oraz procedury losujace i sprawdzajace

	private:
		const int LAST_RTP;
		const int LAST_RTP_ITEM;

#if TESTS == ON
	protected:
#else
	private:
#endif
		int size; // ilosc symboli elementow

		void Change_to_Percents(float part[], float sum);
		int Rand_Percent();

#if TESTS == ON
	public:
#else
	private:
#endif
		Used used; // proporcje rozlozenia symboli

	public:
		int rtp[20][3];
		Symbols();

		void Add_RTP(int rtp_3rd);
		void Add_RTP(int rtp_3rd, int rtp_4th, int rtp_5th);
		void Add_RTP_special(int rtp_5th);
		void Set_Spread(int spread);

		int Get_RTP(int symb, int line_length);
		int Get_Random();
		Items Get_A_Few_Random(int counter);

		void Mark_Used();
		void Reset_Used();

		// 'interface' do pobierania zmiennych z obiektu tej klasy
		int Get_Size();
		int Get_Left();
	};
}
