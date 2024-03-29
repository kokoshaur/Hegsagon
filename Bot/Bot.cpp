// Bot.cpp: определяет экспортированные функции для приложения DLL.
//
#include "stdafx.h"
#include <fstream>
#include <thread>
#include <string>

const int SmollRasius = 6; //Радиус длинной 1
const int BigRadius = 12; //Радиус длинной 2
const int SizeField = 37; //Размер поля
const int OurCell = 2; //Ячейка, принадлежащая боту
const int ForeignCell = 1; //Ячейка, принадлежащая игроку
const int FreeCell = 0; //Ячейка нейтральна
const int EmptyCell = -1; //Ячейка пуста, либо в неё нельзя походить
const std::string TrackBot = "../Field.txt"; //Путь к файлу с полем

struct Hegsagon
{
	int status = FreeCell; //статус ячейки
	int PosMovSmall[SmollRasius] = { EmptyCell, EmptyCell, EmptyCell, EmptyCell, EmptyCell, EmptyCell }; //возможные ходы длинной 1
	int PosMovBig[BigRadius] = { EmptyCell, EmptyCell, EmptyCell, EmptyCell, EmptyCell, EmptyCell, EmptyCell, EmptyCell, EmptyCell, EmptyCell, EmptyCell, EmptyCell }; //возможные ходы длинной 2

};

Hegsagon FieldStatus[SizeField]; //Поле
int PMoves[SizeField-1] = { FreeCell }; //Полный список возможных ходов
int SmallBuf = 1; //Ценность маленького хода для лёгкого бота
int BigBuf = 0; //Ценность большого хода для для лёгкого бота
int MedSmallBuf = 1; //Ценность маленького хода для среднего бота
int MedBigBuf = 0; //Ценность большого хода для для среднего бота
int HardSmallBuf = 1; //Ценность маленького хода для сложного бота
int HardBigBuf = 0; //Ценность большого хода для для сложного бота
int SMove = 0; //Сам маленький ход лёгкого бота
int BMove = 0; //Сам большой ход лёгкого бота
int MedSMove = 0; //Сам маленький ход среднего бота
int MedBMove = 0; //Сам большой ход среднего бота
int HardSMove = 0; //Сам маленький ход сложного бота
int HardBMove = 0; //Сам большой ход сложного бота
int Ansver = 0; //Итоговый номер
int Bigpeak = 0; //Вершина лёгкого бота при большом ходе
int MedBigpeak = 0;	//Вершина среднего бота при большом ходе
int HardBigpeak = 0; //Вершина лёгкого бота при большом ходе

void CheckNeig(int peak, int color)	//Проверка и перекрас соседей после хода
{
	for (int i = 0; i < SmollRasius; i++)
		if (FieldStatus[FieldStatus[peak].PosMovSmall[i]].status == 3 - color)
			FieldStatus[FieldStatus[peak].PosMovSmall[i]].status = color;
}

void SmallMove(int Peak) //Ход по малому кольцу глубиной 1
{
	int rez = 1; //Альфа
	int IdMove = 0; //Промежуточный итоговый ход

	for (int i = 0; i < SmollRasius; i++) //Проверяем все соседствующие клетки из малого круга
		if (FieldStatus[Peak].PosMovSmall[i] == EmptyCell)
			break; //Возможные ходы кончились
		else
		{
			if (FieldStatus[FieldStatus[Peak].PosMovSmall[i]].status == FreeCell) //Проверяем, свободна ли клетка, в которую бот хочет походить
			{
				int buf = 1; //Бета
				for (int j = 0; j < SmollRasius; j++)
					if (FieldStatus[FieldStatus[FieldStatus[Peak].PosMovSmall[i]].PosMovSmall[j]].status == ForeignCell) //Проверяем принадлежность соседней клетки сопернику
						buf++; //Увеличиваем стоимость хода, если соседняя клетка принадлежит сопернику
				if (buf >= rez) //Записываем промежуточный лучший результат
				{
					rez = buf;
					IdMove = FieldStatus[Peak].PosMovSmall[i];
				}
			}
		}

	if ((rez >= SmallBuf) & (IdMove != FreeCell)) //Выводим из функции САМЫЙ лучший результат
	{
		SmallBuf = rez;
		SMove = IdMove;
	}
}

void BigMove(int Peak) //Ход по большому кольцу глубиной 1
{
	int rez = 0; //Альфа
	int IdMove = 0; //Промежуточный итоговый ход


	for (int i = 0; i < BigRadius; i++) //Проверяем все соседствующие клетки из большого круга
		if (FieldStatus[Peak].PosMovBig[i] == EmptyCell)
			break; //Возможные ходы кончились
		else
		{
			if (FieldStatus[FieldStatus[Peak].PosMovBig[i]].status == FreeCell) //Проверяем, свободна ли клетка, в которую бот хочет походить
			{
				int buf = 0; //Бета
				for (int j = 0; j < SmollRasius; j++)
					if (FieldStatus[FieldStatus[FieldStatus[Peak].PosMovBig[i]].PosMovSmall[j]].status == ForeignCell) //Проверяем принадлежность соседней клетки сопернику
						buf++; //Увеличиваем стоимость хода, если соседняя клетка принадлежит сопернику
				if (buf >= rez) //Записываем промежуточный лучший результат
				{
					rez = buf;
					IdMove = FieldStatus[Peak].PosMovBig[i];
				}
			}
		}
	if ((rez >= BigBuf) & (IdMove != FreeCell)) //Выводим из функции САМЫЙ лучший результат
	{
		BigBuf = rez;
		BMove = IdMove;
		Bigpeak = Peak;
	}
}

void MedSmallMove(int Peak) //Ход по малому кольцу глубиной 2
{
	int rez = 1; //Альфа
	int IdMove = 0; //Промежуточный итоговый ход

	for (int i = 0; i < SmollRasius; i++) //Проверяем все соседствующие клетки из малого круга
		if (FieldStatus[Peak].PosMovSmall[i] == EmptyCell)
			break; //Возможные ходы кончились
		else
		{
			if (FieldStatus[FieldStatus[Peak].PosMovSmall[i]].status == FreeCell) //Проверяем, свободна ли клетка, в которую бот хочет походить
			{
				int buf = 1; //Бета
				for (int j = 0; j < SmollRasius; j++)
					if (FieldStatus[FieldStatus[FieldStatus[Peak].PosMovSmall[i]].PosMovSmall[j]].status == ForeignCell)
						buf++;
				if (buf >= rez) //Альфа бета отсечение
				{
					for (int l = 1; l < SizeField; l++) //Пробегаем по полю в поисках клеток соперника
						if (FieldStatus[l].status == ForeignCell) //Проверяем принадлежность соседней клетки сопернику
						{
							FieldStatus[FieldStatus[Peak].PosMovSmall[i]].status = OurCell; //Делаем ход
							std::thread Small(SmallMove, i); //Вычисляем наилучший ход противника глубиной 1 по малому кругу (ставим на место игрока лёгкого бота)
							std::thread Big(BigMove, i); //Вычисляем наилучший ход противника глубиной 1 по большому кругу (ставим на место игрока лёгкого бота)
							Small.join();
							Big.join();
							if ((SmallBuf >= BigBuf)) //Выводим окончательный наилучший возможный ход противника
								Ansver = SMove;
							else
								Ansver = BMove;
							if (FieldStatus[Ansver].status == FreeCell) //Проверка корректности хода(на всякий случай)
							{
								FieldStatus[Ansver].status = ForeignCell; //Ходим наилучшим ходом от имени игрока
								for (int h = 0; h < SmollRasius; h++) //Считаем, сколько наш соперник получит очков при наилучшем ходе. Вычитаем эти очки из нашего Бета
									if (FieldStatus[FieldStatus[Ansver].PosMovSmall[h]].status == OurCell)
										buf--;
								FieldStatus[Ansver].status = FreeCell; //Возвращаем полю исходный вид
							}
							else
								Ansver = FreeCell;

							FieldStatus[FieldStatus[Peak].PosMovSmall[i]].status = FreeCell; //Возвращаем полю исходный вид
						}
				}
				if (buf >= rez) //Сравниваем итоговый Бета с исходным Альфа. Если Бета больше - переписываем Альфу и промежуточный ответ
				{
					rez = buf;
					IdMove = FieldStatus[Peak].PosMovSmall[i];
				}
			}
		}

	if ((rez >= MedSmallBuf) & (IdMove != FreeCell)) //Выводим из функции САМЫЙ лучший результат
	{
		MedSmallBuf = rez;
		MedSMove = IdMove;
	}
}

void MedBigMove(int Peak) //Ход по большому кольцу глубиной 2
{
	int rez = 0; //Альфа
	int IdMove = 0; //Промежуточный итоговый ход


	for (int i = 0; i < BigRadius; i++) //Проверяем все соседствующие клетки из большого круга
		if (FieldStatus[Peak].PosMovBig[i] == EmptyCell)
			break; //Возможные ходы кончились
		else
		{
			if (FieldStatus[FieldStatus[Peak].PosMovBig[i]].status == FreeCell) //Проверяем, свободна ли клетка, в которую бот хочет походить
			{
				int buf = 0; //Бета
				for (int j = 0; j < SmollRasius; j++)
					if (FieldStatus[FieldStatus[FieldStatus[Peak].PosMovBig[i]].PosMovSmall[j]].status == ForeignCell) //Проверяем принадлежность соседней клетки сопернику
						buf++; //Увеличиваем стоимость хода, если соседняя клетка принадлежит сопернику
				if (buf >= rez) //Альфа бета отсечение
				{
					for (int l = 1; l < SizeField; l++) //Пробегаем по полю в поисках клеток соперника
						if (FieldStatus[l].status == ForeignCell) //Проверяем принадлежность соседней клетки сопернику
						{
							FieldStatus[FieldStatus[Peak].PosMovBig[i]].status = OurCell; //Делаем ход
							std::thread Small(SmallMove, i); //Вычисляем наилучший ход противника глубиной 1 по малому кругу (ставим на место игрока лёгкого бота)
							std::thread Big(BigMove, i); //Вычисляем наилучший ход противника глубиной 1 по большому кругу (ставим на место игрока лёгкого бота)
							Small.join();
							Big.join();
							if ((SmallBuf > BigBuf)) //Выводим окончательный наилучший возможный ход противника
								Ansver = SMove;
							else
								Ansver = BMove;
							if (FieldStatus[Ansver].status == FreeCell) //Проверка корректности хода(на всякий случай)
							{
								FieldStatus[Ansver].status = ForeignCell; //Ходим наилучшим ходом от имени игрока
								for (int h = 0; h < SmollRasius; h++) //Считаем, сколько наш соперник получит очков при наилучшем ходе. Вычитаем эти очки из нашего Бета
									if (FieldStatus[FieldStatus[Ansver].PosMovSmall[h]].status == OurCell)
										buf--;
								FieldStatus[Ansver].status = FreeCell; //Возвращаем полю исходный вид
							}
							else
								Ansver = FreeCell;
							FieldStatus[FieldStatus[Peak].PosMovBig[i]].status = FreeCell; //Возвращаем полю исходный вид
						}
				}
				if (buf >= rez) //Сравниваем итоговый Бета с исходным Альфа. Если Бета больше - переписываем Альфу и промежуточный ответ
				{
					rez = buf;
					IdMove = FieldStatus[Peak].PosMovBig[i];
				}
			}
		}
	if ((rez >= MedBigBuf) & (IdMove != FreeCell)) //Выводим из функции САМЫЙ лучший результат
	{
		MedBigBuf = rez;
		MedBMove = IdMove;
		MedBigpeak = Peak;
	}
}

void HardSmallMove(int Peak) //Ход по малому кольцу глубиной 4
{
	int rez = 1; //Альфа
	int IdMove = 0; //Промежуточный итоговый ход

	for (int i = 0; i < SmollRasius; i++) //Проверяем все соседствующие клетки из большого круга
		if (FieldStatus[Peak].PosMovSmall[i] == EmptyCell)
			break; //Возможные ходы кончились
		else
		{
			if (FieldStatus[FieldStatus[Peak].PosMovSmall[i]].status == FreeCell) //Проверяем, свободна ли клетка, в которую бот хочет походить
			{
				int buf = 1; //Бета
				for (int j = 0; j < SmollRasius; j++)
					if (FieldStatus[FieldStatus[FieldStatus[Peak].PosMovSmall[i]].PosMovSmall[j]].status == ForeignCell) //Проверяем принадлежность соседней клетки сопернику
						buf++; //Увеличиваем стоимость хода, если соседняя клетка принадлежит сопернику
				if (buf >= rez) //Альфа бета отсечение
				{
					for (int l = 1; l < SizeField; l++) //Пробегаем по полю в поисках клеток соперника
						if (FieldStatus[l].status == ForeignCell) //Проверяем принадлежность соседней клетки сопернику
						{
							FieldStatus[FieldStatus[Peak].PosMovSmall[i]].status = OurCell; //Делаем ход
							std::thread Small(MedSmallMove, i); //Вычисляем наилучший ход противника глубиной 2 по малому кругу (ставим на место игрока среднего бота)
							std::thread Big(MedBigMove, i); //Вычисляем наилучший ход противника глубиной 2 по большому кругу (ставим на место игрока среднего бота)
							Small.join();
							Big.join();
							if ((MedSmallBuf >= MedBigBuf)) //Выводим окончательный наилучший возможный ход противника
								Ansver = MedSMove;
							else
								Ansver = MedBMove;
							if (FieldStatus[Ansver].status == FreeCell) //Проверка корректности хода(на всякий случай)
							{
								FieldStatus[Ansver].status = ForeignCell; //Ходим наилучшим ходом от имени игрока
								for (int h = 0; h < SmollRasius; h++) //Считаем, сколько наш соперник получит очков при наилучшем ходе. Вычитаем эти очки из нашего Бета
									if (FieldStatus[FieldStatus[Ansver].PosMovSmall[h]].status == OurCell)
										buf--;
								FieldStatus[Ansver].status = FreeCell; //Возвращаем полю исходный вид
							}
							else
								Ansver = FreeCell;
							FieldStatus[FieldStatus[Peak].PosMovSmall[i]].status = FreeCell; //Возвращаем полю исходный вид
						}
				}
				if (buf >= rez) //Сравниваем итоговый Бета с исходным Альфа. Если Бета больше - переписываем Альфу и промежуточный ответ
				{
					rez = buf;
					IdMove = FieldStatus[Peak].PosMovSmall[i];
				}
			}
		}

	if ((rez >= HardSmallBuf) & (IdMove != FreeCell)) //Выводим из функции САМЫЙ лучший результат
	{
		HardSmallBuf = rez;
		HardSMove = IdMove;
	}
}

void HardBigMove(int Peak) //Ход по большому кольцу глубиной 4
{
	int rez = 0; //Альфа
	int IdMove = 0; //Промежуточный итоговый ход


	for (int i = 0; i < BigRadius; i++) //Проверяем все соседствующие клетки из большого круга
		if (FieldStatus[Peak].PosMovBig[i] == EmptyCell)
			break; //Возможные ходы кончились
		else
		{
			if (FieldStatus[FieldStatus[Peak].PosMovBig[i]].status == FreeCell) //Проверяем, свободна ли клетка, в которую бот хочет походить
			{
				int buf = 0; //Бета
				for (int j = 0; j < SmollRasius; j++)
					if (FieldStatus[FieldStatus[FieldStatus[Peak].PosMovBig[i]].PosMovSmall[j]].status == ForeignCell) //Проверяем принадлежность соседней клетки сопернику
						buf++; //Увеличиваем стоимость хода, если соседняя клетка принадлежит сопернику
				if (buf >= rez) //Альфа бета отсечение
				{
					for (int l = 1; l < SizeField; l++) //Пробегаем по полю в поисках клеток соперника
						if (FieldStatus[l].status == ForeignCell) //Проверяем принадлежность соседней клетки сопернику
						{
							FieldStatus[FieldStatus[Peak].PosMovBig[i]].status = OurCell; //Делаем ход
							std::thread Small(MedSmallMove, i); //Вычисляем наилучший ход противника глубиной 2 по малому кругу (ставим на место игрока среднего бота)
							std::thread Big(MedBigMove, i); //Вычисляем наилучший ход противника глубиной 2 по большому кругу (ставим на место игрока среднего бота)
							Small.join();
							Big.join();
							if ((MedSmallBuf > MedBigBuf)) //Выводим окончательный наилучший возможный ход противника
								Ansver = MedSMove;
							else
								Ansver = MedBMove;
							if (FieldStatus[Ansver].status == FreeCell) //Проверка корректности хода(на всякий случай)
							{
								FieldStatus[Ansver].status = ForeignCell; //Ходим наилучшим ходом от имени игрока
								for (int h = 0; h < SmollRasius; h++)
									if (FieldStatus[FieldStatus[Ansver].PosMovSmall[h]].status == OurCell) //Считаем, сколько наш соперник получит очков при наилучшем ходе. Вычитаем эти очки из нашего Бета
										buf--;
								FieldStatus[Ansver].status = FreeCell; //Возвращаем полю исходный вид
							}
							else
								Ansver = FreeCell;
							FieldStatus[FieldStatus[Peak].PosMovBig[i]].status = FreeCell; //Возвращаем полю исходный вид
						}
				}
				if (buf >= rez) //Сравниваем итоговый Бета с исходным Альфа. Если Бета больше - переписываем Альфу и промежуточный ответ
				{
					rez = buf;
					IdMove = FieldStatus[Peak].PosMovBig[i];
				}
			}
		}
	if ((rez >= HardBigBuf) & (IdMove != FreeCell)) //Выводим из функции САМЫЙ лучший результат
	{
		HardBigBuf = rez;
		HardBMove = IdMove;
		HardBigpeak = Peak;
	}
}

extern "C"
{
	__declspec(dllexport) void BotFieldInit() //Инициализируем поле из файла
	{
		using namespace std;
		ifstream ReaderField(TrackBot);
		for (int i = 1; i < SizeField; i++) //Проверяем все строки из файла (их должно быть столько же, сколько ячеек в поле)
		{
			string buff;
			getline(ReaderField, buff); //Получаем строку
			int j = 0;
			string rez;
			string semicolon = ";";
			string comma = ",";
			while (buff[j] != semicolon[0]) //Вычленяем первую часть и записываем её в буфер
			{
				rez += buff[j];
				j++;
			}
			FieldStatus[i].status = stoi(rez); //Конвертируем буфер в число и заносим в поле
			rez = "";
			j++;
			int h = 0;
			while (buff[j] != semicolon[0]) //Вычленяем вторую часть и записываем её в буфер
			{
				while ((buff[j] != comma[0]) && (buff[j] != semicolon[0]))
				{
					rez += buff[j];
					j++;
				}
				FieldStatus[i].PosMovSmall[h] = stoi(rez); //Конвертируем буфер в число и заносим в поле
				rez = "";
				h++;
				if (buff[j] == semicolon[0])
					break;
				j++;
			}
			j++;
			rez = "";
			h = 0;
			while (j < buff.length()) //Вычленяем третью часть и записываем её в буфер
			{
				while ((buff[j] != comma[0]) && (j < buff.length()))
				{
					rez += buff[j];
					j++;
				}
				FieldStatus[i].PosMovBig[h] = stoi(rez); //Конвертируем буфер в число и заносим в поле
				rez = "";
				h++;
				j++;
			}
		}
	}

	__declspec(dllexport) int CheckMove(int peak, int move)//Проверка корректности хода
	{
		for (int i = 0; i < SmollRasius; i++)
			if (FieldStatus[peak].PosMovSmall[i] == move)
				return 1;
		for (int i = 0; i < BigRadius; i++)
			if (FieldStatus[peak].PosMovBig[i] == move)
				return 2;
		return 0;
	}

	__declspec(dllexport) void Move(int i, int b) //Запись хода игрока (синхронизация с основной формой)
	{
		FieldStatus[i].status = b;
		if (b == ForeignCell)
			CheckNeig(i, ForeignCell);
	}

	__declspec(dllexport) int PeacBot() //Мирный бот. Возвращаем детерминированное число (нужен был для тестинга)
	{
		return (FieldStatus[SizeField-1].PosMovBig[1]);
	}

	__declspec(dllexport) int EasyBot() //Лёгкий бот. Возвращает наилучший ход для прохода глубиной 1
	{
		SmallBuf = 1; //Промежуточный наилучший ход длинной 1
		BigBuf = 0; //Промежуточный наилучший ход длинной 2
		Ansver = FreeCell; //Ответ (номер ячейки, куда бот решил походить)
		for (int i = 1; i < SizeField; i++) //Пробегаем по полю в поисках наших ячеек
		{
			if (FieldStatus[i].status == OurCell)
			{
				std::thread Small(SmallMove, i); //Вычисляем наилучший ход глубиной 1 по малому кругу
				std::thread Big(BigMove, i); //Вычисляем наилучший ход глубиной 1 по большому кругу
				Small.join();
				Big.join();
				if ((SmallBuf > BigBuf)) //Выбираем наилучший промежуточный результат
					Ansver = SMove;
				else
					Ansver = BMove;
			}
		}
		if (FieldStatus[Ansver].status == FreeCell) //Проверка ошибок
			FieldStatus[Ansver].status = OurCell;
		else
			Ansver = FreeCell;

		CheckNeig(Ansver, OurCell); //Перекрашиваем съеденные клетки игрока

		if ((SmallBuf <= BigBuf)) //Выводим окончательный наилучший возможный ход противника
		{
			FieldStatus[Bigpeak].status = FreeCell;
			Ansver = BMove * 100 + Bigpeak;
			Bigpeak = FreeCell;
		}
		return(Ansver);
	}

	__declspec(dllexport) int NormalBot() //Средний бот. Возвращает наилучший ход для прохода глубиной 2
	{
		MedSmallBuf = 1; //Промежуточный наилучший ход длинной 1
		MedBigBuf = 0; //Промежуточный наилучший ход длинной 2
		Ansver = FreeCell; //Ответ (номер ячейки, куда бот решил походить)
		for (int i = 1; i < SizeField; i++) //Пробегаем по полю в поисках наших ячеек
		{
			if (FieldStatus[i].status == OurCell)
			{
				std::thread Small(MedSmallMove, i); //Вычисляем наилучший ход глубиной 2 по малому кругу
				std::thread Big(MedBigMove, i); //Вычисляем наилучший ход глубиной 2 по большому кругу
				Small.join();
				Big.join();
				if ((MedSmallBuf > MedBigBuf)) //Выбираем наилучший промежуточный результат
					Ansver = MedSMove;
				else
					Ansver = MedBMove;
			}
		}
		if (FieldStatus[Ansver].status == FreeCell) //Проверка ошибок
			FieldStatus[Ansver].status = OurCell;
		else
			Ansver = FreeCell;

		CheckNeig(Ansver, OurCell); //Перекрашиваем съеденные клетки игрока

		if ((MedSmallBuf <= MedBigBuf)) //Выводим окончательный наилучший возможный ход противника
		{
			FieldStatus[MedBigpeak].status = FreeCell;
			Ansver = MedBMove * 100 + MedBigpeak;
			MedBigpeak = FreeCell;
		}
		return(Ansver);
	}

	__declspec(dllexport) int HardBot() //Сложный бот. Возвращает наилучший ход для прохода глубиной 4
	{
		HardSmallBuf = 1; //Промежуточный наилучший ход длинной 1
		HardBigBuf = 0; //Промежуточный наилучший ход длинной 2
		Ansver = FreeCell; //Ответ (номер ячейки, куда бот решил походить)
		for (int i = 1; i < SizeField; i++) //Пробегаем по полю в поисках наших ячеек
		{
			if (FieldStatus[i].status == OurCell)
			{
				std::thread Small(HardSmallMove, i); //Вычисляем наилучший ход глубиной 4 по малому кругу
				std::thread Big(HardBigMove, i); //Вычисляем наилучший ход глубиной 4 по большому кругу
				Small.join();
				Big.join();
				if ((HardSmallBuf > HardBigBuf)) //Выбираем наилучший промежуточный результат
					Ansver = HardSMove;
				else
					Ansver = HardBMove;
			}
		}
		if (FieldStatus[Ansver].status == FreeCell) //Проверка ошибок
			FieldStatus[Ansver].status = OurCell;
		else
			Ansver = FreeCell;

		CheckNeig(Ansver, OurCell); //Перекрашиваем съеденные клетки игрока

		if ((HardSmallBuf <= HardBigBuf)) //Выводим окончательный наилучший возможный ход противника
		{
			FieldStatus[HardBigpeak].status = FreeCell;
			Ansver = HardBMove * 100 + HardBigpeak;
			HardBigpeak = FreeCell;
		}
		return(Ansver);
	}

	__declspec(dllexport) int NightmarelBotBot() //Нечестный бот. Пытается походить туда, куда ему нельзя походить (нужен был для тестинга)
	{
		int Move = -1;
		FieldStatus[Move].status = OurCell;
		return(Move);
	}

	__declspec(dllexport) int CheckPosMov() //Проверяем остались ли возможные ходы игрока (чтобы установить конец игры)
	{
		for (int i = 1; i < SizeField; i++) // Пробегаем по полю
			if (FieldStatus[i].status == ForeignCell) //Находим ячейка игрока
			{
				for (int b = 0; b < SmollRasius-1; b++) //Проверяем, можно ли походить из неё по малому кругу
					if (FieldStatus[i].PosMovSmall[b] != EmptyCell)
						if (FieldStatus[FieldStatus[i].PosMovSmall[b]].status == FreeCell)
							return 1; //Если можно, возвращаем 1
				for (int b = 0; b < BigRadius-1; b++) //Проверяем, можно ли походить из неё по большому кругу
					if (FieldStatus[i].PosMovBig[b] != EmptyCell)
						if (FieldStatus[FieldStatus[i].PosMovBig[b]].status == FreeCell)
							return 1; //Если можно, возвращаем 1
			}
		return 0; //Если нельзя, возвращаем 0 (конец игры)
	}
}