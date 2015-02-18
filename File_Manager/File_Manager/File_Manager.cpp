//Что нужно сделать (план действий для себя)
//1. Записать содержимое текущей директории в массив или, лучше, в структуру (Имя, тип). 
//2. Сдеать чтобы данные из массива выводились в консоль, например по 20 строк не больше, и реализовать движение курсора по этим строкам с помощью стрелок
//3. При нажатии Enter - если директория - повторить п. 2, а если файл - вывести атрибуты.
//4. Сделать выбор диска при нажатии клавиши, напимер F2, для чего узнать какие диски есть на компе.

#undef UNICODE			//Отключает UNICODE, заданный по умолчанию в настройках VS
						//это нужно для корректной работы функции GetCurrentDirectory
#include <iostream>
#include <iomanip>		//для setw() и setfill()
#include <Windows.h>	//для корректного получения и вывода русского языка (SetConsoleOutputCP() и SetConsoleCP()), а также изменения цвета в консоле
#include <io.h>			//для поиска файлов
#include <conio.h>		//для _getch()
#include <ctime>
using namespace std;

//Получить количество элементов в директории
int GetCount(char* path)
{
	int count = 1;
	_finddata_t temp;

	//Получить сведения о первом экземпляре
	int firstID = _findfirst(path, &temp);

	while (_findnext(firstID, &temp) != -1)
		++count;
	_findclose(firstID);
	return count;
}

//Получить данные текущей директории
void GetDir(_finddata_t* dir, int count, char* path)
{
	//Получить и сохранить сведения о первом экземпляре
	int firstID = _findfirst(path, &dir[0]);
	// Поиск и сохранение в массив

	for (int i = 1; i < count; i++)
		_findnext(firstID, &dir[i]);
	_findclose(firstID);
}

//Консольные цвета
enum
{
	clBlack = 0, clNavy = 1, clGreen = 2, clTeal = 3, clBrown = 4,
	clPurple = 5, clOlive = 6, clGray = 7, clSilver = 8, clBlue = 9,
	clLime = 10, clCyan = 11, clRed = 12, clMagneta = 13, clYellow = 14,
	clWhite = 15
};

void SetConsoleColorTextBackground(unsigned short Tex = clWhite, unsigned short Bg = clBlack)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	Bg = Bg << 4 | Tex;
	SetConsoleTextAttribute(hConsole, Bg);
}

//Позиционирование курсора
void SetConsoleCursorPosition(short x_position, short y_position)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD _coordPosition = { x_position, y_position };
	SetConsoleCursorPosition(hConsole, _coordPosition);
}

//Определение клавиш клавиатуры
enum VKey
{
	UP = 72, LEFT = 75, RIGHT = 77, DOWN = 80,
	ENTER = 13, ESC = 27, BackSpase = 8,
	F1 = 59, F2 = 60, PageUp = 73, PageDown = 81
};

//Движение стрелками и выдод директории в консоль
int Motion(_finddata_t* dir, int count)
{
	int y = 0, move = 0;
	char info[MAX_PATH];

	//Нужно укоротить имена файлов до 40 символов, чтобы не ломали картину
	_finddata_t* temp = new _finddata_t[count];
	for (int i = 0; i < count; i++)
		strncpy_s(temp[i].name, 41, dir[i].name, 40);
	
	system("cls");
	//Вывод в консоль
	for (int i = 0; i < count; i++)
	{
		// Проверяем Директория или Нет
		dir[i].attrib&_A_SUBDIR ? strcpy_s(info, "Каталог") : strcpy_s(info, "Файл");
		SetConsoleCursorPosition(0, y + i);
		cout << temp[i].name;
		SetConsoleCursorPosition(50, y + i);
		cout << info;
	}

	SetConsoleCursorPosition(0, 0);
	SetConsoleColorTextBackground(clWhite, clGreen);
	//Подсвечиваю самую верхнюю позицию
	dir[0].attrib&_A_SUBDIR ? strcpy_s(info, "Каталог") : strcpy_s(info, "Файл");
	cout << temp[0].name;
	SetConsoleCursorPosition(50, 0);
	cout << info;

	while (move != ENTER && move != ESC && move != BackSpase)
	{
		//считываю нажатые клавиши
		move = _getch();
		if (move == 0xe0 || !move)
			move = _getch();

		//исключаю все клавиши, кроме разрешенных
		if (move != UP && move != DOWN && move != ENTER && move != ESC && move != BackSpase)
			move = 0;

		if (move)
		{
			SetConsoleCursorPosition(0, y);
			SetConsoleColorTextBackground(clGray, clBlack);
			//здесь был курсор
			dir[y].attrib&_A_SUBDIR ? strcpy_s(info, "Каталог") : strcpy_s(info, "Файл");
			cout << temp[y].name;
			SetConsoleCursorPosition(50, y);
			cout << info;

			if (move == UP && y > 0)
				y--;
			if (move == DOWN && y < count - 1)
				y++;

			SetConsoleCursorPosition(0, y);
			SetConsoleColorTextBackground(clWhite, clGreen);
			//здесь курсор сейчас
			dir[y].attrib&_A_SUBDIR ? strcpy_s(info, "Каталог") : strcpy_s(info, "Файл");
			cout << temp[y].name;
			SetConsoleCursorPosition(50, y);
			cout << info;

			//и вернуться опять к серо-черному
			SetConsoleColorTextBackground(clGray, clBlack);
		}

		if (move == BackSpase)
			y = -2;
		
		if (move == ESC)
			y = -1;
	}
	SetConsoleCursorPosition(0, count);
	delete[]temp;
	return y;
}

void main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	int move = 0;
	char path[MAX_PATH];
	char temp[MAX_PATH];

	GetCurrentDirectory(sizeof(path), path);
	strcpy_s(temp, path);

	do
	{
		//Добавляю маску к пути
		strcat_s(path, "\\*.*");

		//Узнать количество элементов в директории
		int count = GetCount(path);

		//Создать массив элементов структуры _finddata_t для хранения данных текущей дирректории
		_finddata_t* dir = new _finddata_t[count];
		
		//Заполнить массив данными текущей директории
		GetDir(dir, count, path);
		
		//Передаю управление юзеру и узнаю что он хочет
		move = Motion(dir, count);
				
		if (move == -2)
			strcpy_s(path, temp);
						
		strcpy_s(temp, path);

		//убрать маску (4-ре последних символа)
		strncpy_s(path, strlen(path) - 3, path, strlen(path) - 4);

		// Переход в родительский каталог
		if (move == 0 || move == 1)
		{
			//Найти адрес последнего слэша
			char *end = strrchr(path, '\\');
			if (end) //если нашел слэш
			{
				//Узнать сколько лишних символов			
				int delta = end - path;
				//Убрать лишние символы
				strncpy_s(path, delta + 1, path, delta);
			}
			else //Родительского каталога нет. Заходим в тот на котором курсор.
			{
				strcat_s(path, "\\");
				strcat_s(path, dir[move].name);
			}
		}

		// Переход в выбранный каталог
		if ((dir[move].attrib & _A_SUBDIR) && move > 1)
		{
			strcat_s(path, "\\");
			strcat_s(path, dir[move].name);
		}

		//Если файл - вывести атрибуты
		if (!(dir[move].attrib & _A_SUBDIR) && move != -1 && move != -2)
		{
			cout << endl;
			cout << "RDO | HID | SYS | ARC |           DATE           | SIZE" << endl;
			cout << "--- + --- + --- + --- +--------------------------+ ----" << endl;
				
					char buffer[30];
					cout << ((dir[move].attrib & _A_RDONLY) ? " Y  " : " N  ") << "|";
					cout << " " << ((dir[move].attrib & _A_HIDDEN) ? " Y  " : " N  ") << "|";
					cout << " " << ((dir[move].attrib & _A_SYSTEM) ? " Y  " : " N  ") << "|";
					cout << " " << ((dir[move].attrib & _A_ARCH) ? " Y  " : " N  ") << "|";
					ctime_s(buffer, _countof(buffer), & dir[move].time_write);
					strncpy_s(buffer, 25, buffer, 24);
					cout << " " << buffer << " | " << dir[move].size << endl << endl;
					system("pause");
		}

		delete[]dir;
	} while (move != -1);
}