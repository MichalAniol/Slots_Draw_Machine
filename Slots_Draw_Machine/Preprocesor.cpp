#include "stdafx.h"

namespace Slots_Draw_Machine
{
#define ON 1
#define OFF 0

#define RANGE ON             // wlacza assercje przekroczen zakresow w funkcjach
#define TESTS ON            // otwiera klasy do testow
#define SHOW_READ_VAL OFF  // wypisuje wczytane wartosci z pliku "Parameters.cfg"


	//--------------------------------  bada czy zakres zostal przekroczony
#define TEST_RANGE(variable, min, max) \
{ \
	if (variable < min) \
	{ \
		printf("!!! Przekroczony zakres minimalny: %d\n%s = %d,   w linii: %d\n\n%s\n\n", min, #variable, variable, __LINE__, __FILE__); \
		exit(EXIT_FAILURE); \
	} \
	\
	if (variable > max) \
	{ \
		printf("!!! Przekroczony zakres maksymalny: %d\n%s = %d,   w linii: %d\n\n%s\n\n", max, #variable, variable, __LINE__, __FILE__); \
		exit(EXIT_FAILURE); \
	} \
}


//------------------------------  zglasza blad, gdy warunek jest nie spelniony
#define ASSERT(condition, message) \
{ \
	if (condition == true) \
	{ \
		printf("Blad assercji: %s\n\nLinia: %d ---> %s %s\n\n", __FILE__, __LINE__, message, #condition); \
		exit( EXIT_FAILURE ); \
	} \
}


//------------------------------  zwraca wielkosc 1wszego wymiaru tablicy
#define ARRAY_LENGTH(x) (sizeof(x)/sizeof(x[0]))


//------------------------------  zwraca wielkosc 2giego wymiaru tablicy
#define ARRAY_SECOND(x) (sizeof(x[0])/sizeof(x[0][0]))
}