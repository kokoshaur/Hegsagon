// Bot.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"

int PMoves[36] = { 0 };

extern "C" 
{
	__declspec(dllexport) void PossibleMoves(int NumHegs, int FieldStatus[], int PMoves[])
	{
				if (FieldStatus[NumHegs + 1] == 0)
					PMoves[NumHegs + 1] = 1;
				if (FieldStatus[NumHegs - 1] == 0)
					PMoves[NumHegs - 1] = 1;
				if (FieldStatus[NumHegs + 6] == 0)
					PMoves[NumHegs + 6] = 1;
				if (FieldStatus[NumHegs - 6] == 0)               // Маленький шестиугольник
					PMoves[NumHegs - 6] = 1;
				if (FieldStatus[NumHegs + 7] == 0)
					PMoves[NumHegs + 7] = 1;
				if (FieldStatus[NumHegs - 7] == 0)
					PMoves[NumHegs - 7] = 1;


				
	}
	__declspec(dllexport) int PeacBot(int FieldStatus[])
	{
		int NumHegs = 0;
		int Move = 0;
		while (FieldStatus[NumHegs] != 2)
			NumHegs++;
		PossibleMoves(NumHegs, FieldStatus, PMoves);
		for (int i = 35; i > 0; i--)
			if (PMoves[i])
				Move = i;
		FieldStatus[Move] = 2;
		return(Move);
	}

	__declspec(dllexport) int EasyBot(int FieldStatus[])
	{
		int Move = 34;
		FieldStatus[Move] = 2;
		return(Move);
	}

	__declspec(dllexport) int NormalBot(int FieldStatus[])
	{
		int Move = 34;
		FieldStatus[Move] = 2;
		return(Move);
	}

	__declspec(dllexport) int HardlBotBot(int FieldStatus[])
	{
		int Move = 34;
		FieldStatus[Move] = 2;
		return(Move);
	}

	__declspec(dllexport) int NightmarelBotBot(int FieldStatus[])
	{
		int Move = 34;
		FieldStatus[Move] = 2;
		return(Move);
	}
}
