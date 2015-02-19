#undef UNICODE			//Отключает UNICODE, заданный по умолчанию в настройках VS
//это нужно для корректной работы функции GetCurrentDirectory с char
#include <iostream>
#include <Windows.h>	//для корректного получения и вывода русского языка (SetConsoleOutputCP() и SetConsoleCP()), а также изменения цвета в консоле
#include <io.h>			//для поиска файлов
#include <conio.h>		//для _getch()
#include <ctime>		//для ctime_s
using namespace std;

//Для храниея истории в списке, типа динамического стека.
struct Elem
{
	char history[MAX_PATH];
	Elem * next, *prev;
};

class List
{
	Elem *Tail; //Голова не нужна. "Главное хвост!" (с).
	int Count;

public:

	List()
	{
		Tail = NULL;
		Count = 0;
	}

	//Деструктор удаляет, пока есть элемены.
	~List()
	{
		while (Count)
			Del();
	}

	//Добавляет элемент в хвост
	void Push(char* history)
	{
		Elem * temp = new Elem;
		strcpy_s(temp->history, MAX_PATH, history);
		temp->prev = Tail;
		if (!Count)
			Tail = temp;
		else
		{
			Tail->next = temp;
			Tail = temp;
		}
		Count++;
	}

	//Получить последний элемент
	Elem * Pop()
	{
		return Tail;
	}

	//Узнать количество элементов
	int GetCount()
	{
		return Count;
	}

	//Удалить последний элемент
	void List::Del()
	{
		Elem * Del = Tail;
		Elem * PrevDel = Del->prev;
		Tail = PrevDel;
		delete Del;
		Count--;
	}
};

//Получить количество элементов в директории
int GetCount(char* path)
{
	int count = 1;
	_finddata_t temp;

	int firstID = _findfirst(path, &temp);

	if (firstID == -1)
		return -1;

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
int Motion(_finddata_t* dir, int count, char* path)
{
	int y = 0, move = 0, block = 0;
	char info[MAX_PATH];

	//Нужно укоротить имена файлов до 40 символов, чтобы не ломали картину
	_finddata_t* temp = new _finddata_t[count];
	for (int i = 0; i < count; i++)
		strncpy_s(temp[i].name, 41, dir[i].name, 40);

	//Вывод в консоль/. Внешний цикл для вывода блоками по 20 элементов. Для PageUp, PageDown
	do
	{
		system("cls");
		move = y = 0;

		for (int i = block; i < count; i++)
		{
			// Проверяем Директория или Нет
			dir[i].attrib&_A_SUBDIR ? strcpy_s(info, "Каталог") : strcpy_s(info, "Файл");

			SetConsoleCursorPosition(0, y + i - block);
			cout << temp[i].name;
			SetConsoleCursorPosition(50, y + i - block);
			cout << info;

			if (i == 19 + block)
				break;
		}

		SetConsoleColorTextBackground(clLime, clBlack);
		SetConsoleCursorPosition(0, (count < 20) ? (count - block + 1) : 22);
		cout << path;

		if ((count - block) > 20)
		{
			SetConsoleCursorPosition(25, 23);
			cout << "PageDown";
		}

		if (block >= 20)
		{
			SetConsoleCursorPosition(43, 23);
			cout << "PageUp";
		}

		//Подсвечиваю самую верхнюю позицию
		SetConsoleCursorPosition(0, 0);
		SetConsoleColorTextBackground(clWhite, clGreen);
		dir[block].attrib&_A_SUBDIR ? strcpy_s(info, "Каталог") : strcpy_s(info, "Файл");
		cout << temp[block].name;
		SetConsoleCursorPosition(50, 0);
		cout << info;

		//Внутренний цикл для движения стрелками по списку
		while (move != ENTER && move != ESC && move != BackSpase && move != F1 && move != F2 && move != PageDown && move != PageUp)
		{
			//считываю нажатые клавиши
			move = _getch();
			if (move == 0xe0 || !move)
				move = _getch();

			//исключаю все клавиши, кроме разрешенных
			if (move != UP && move != DOWN && move != ENTER && move != ESC && move != BackSpase && move != F1 && move != F2 && move != PageDown && move != PageUp)
				move = 0;

			//движение стрелками
			if (move)
			{
				//здесь был курсор
				SetConsoleCursorPosition(0, y);
				SetConsoleColorTextBackground(clGray, clBlack);
				dir[y + block].attrib&_A_SUBDIR ? strcpy_s(info, "Каталог") : strcpy_s(info, "Файл");
				cout << temp[y + block].name;
				SetConsoleCursorPosition(50, y);
				cout << info;

				if (move == UP && y > 0)
					y--;
				if (move == DOWN && y + block < count - 1 && y < 19)
					y++;

				//здесь курсор сейчас
				SetConsoleCursorPosition(0, y);
				SetConsoleColorTextBackground(clWhite, clGreen);
				dir[y + block].attrib&_A_SUBDIR ? strcpy_s(info, "Каталог") : strcpy_s(info, "Файл");
				cout << temp[y + block].name;
				SetConsoleCursorPosition(50, y);
				cout << info;
				SetConsoleColorTextBackground(clGray, clBlack);
			}
		}

		if (move == PageDown && (count - block) > 20)
			block += 20;
		if (move == PageUp && block >= 20)
			block -= 20;

		if (move == ESC)
			y = -1;
		if (move == BackSpase)
			y = -2;
		if (move == F1)
			y = -3;
		if (move == F2)
			y = -4;

	} while (move != ESC && move != BackSpase && move != F1 && move != F2 && move != ENTER);

	SetConsoleCursorPosition(0, (count < 20) ? (count - block + 3) : 24);
	delete[]temp;

	if (move == ESC || move == BackSpase || move == F1 || move == F2)
		return y;
	return y + block;
}

//Узнать имеющиеся в системе диски, в т.ч. логические и съемные
int GetDisks(char* disk)
{
	int count = 0;

	for (int i = 0; i < 26; i++)
	if ((GetLogicalDrives() >> i) & 1)

		//Возвращаемое GetLogicalDrives() значение является коллекцией однобитных флагов, определяющих найденные диски.
		//1 - есть диск. 0 - нет диска.
		//Нулевой бит (самый младший) соответствует диску A:, первый бит - диску B:, второй - диску C: и т.д.
		//Побитовым сдвигом перехожу к следующему биту и побитово сравниваю с 1 

		disk[count++] = char(65 + i); // Буква диска. Char 65 - это A, и т.д.

	return count;
}

char ChangeDisk(char* disk, int countDisks)
{
	int y = 0, move = 0;
	system("cls");
	for (int i = 0; i < countDisks; i++)
		cout << disk[i] << ":\\" << endl;
	cout << endl;

	SetConsoleCursorPosition(0, 0);
	SetConsoleColorTextBackground(clWhite, clGreen);
	cout << disk[0] << ":\\" << endl;

	while (move != ENTER)
	{
		//считываю нажатые клавиши
		move = _getch();
		if (move == 0xe0 || !move)
			move = _getch();

		//исключаю все клавиши, кроме разрешенных
		if (move != UP && move != DOWN && move != ENTER && move != ESC)
			move = 0;

		if (move == ESC)
		{
			SetConsoleColorTextBackground(clGray, clBlack);
			SetConsoleCursorPosition(0, countDisks);
			return '!';
		}

		if (move)
		{
			//здесь был курсор
			SetConsoleCursorPosition(0, y);
			SetConsoleColorTextBackground(clGray, clBlack);
			cout << disk[y] << ":\\";

			if (move == UP && y > 0)
				y--;
			if (move == DOWN && y < countDisks - 1)
				y++;

			//здесь курсор сейчас
			SetConsoleCursorPosition(0, y);
			SetConsoleColorTextBackground(clWhite, clGreen);
			cout << disk[y] << ":\\";
			SetConsoleColorTextBackground(clGray, clBlack);
		}
	}
	SetConsoleCursorPosition(0, countDisks);
	return disk[y];
}

//Мануал :))
void Help()
{
	system("cls");
	cout << "\n\tДвижение по катологу осуществляется стрелками вверх и вниз." << endl;
	cout << "\n\tВыбор - Enter." << endl;
	cout << "\n\tBackSpace - возврат в предыдущий каталог." << endl;
	cout << "\n\tF1 - вызов этой справки." << endl;
	cout << "\n\tPageDown и PageUp переход на следующую, или предыдущую страницы." << endl;
	SetConsoleColorTextBackground(clCyan, clBlack);
	cout << "\n\tF2 - сменить диск." << endl;
	SetConsoleColorTextBackground(clGray, clBlack);
	cout << "\n\tEsc - выход из программы.\n" << endl;
	system("pause");
}

void main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	//Элемент меню
	int move = -3;

	//Путь к директории
	char path[MAX_PATH];

	//массив элементов структуры _finddata_t для хранения данных текущей дирректории
	_finddata_t* dir = nullptr;

	//Список-стек для хранения истории
	List ST;

	//Массив для хранения информации о дисках компа. 26 букв в английском алфавите. Винда больше не предлагает для буквы диска.
	char disk[26];

	//Получпю путь к текущей директории
	GetCurrentDirectory(sizeof(path), path);

	do
	{
		if (move == -3)
			Help();

		//Добавляю маску к пути
		strcat_s(path, "\\*.*");

		//Узнать количество элементов в директории
		int count = GetCount(path);

		//Если ошибка, связанная с отсутствием носителя
		if (count == -1)
		{
			cout << "\n\tНосителя нет в устройстве, иди другая ошибка доступа..." << endl;
			cout << "\n\tВыберите другой диск...\n" << endl;
			move = -4;
			system("pause");
		}
		else
		{
			dir = new _finddata_t[count];

			//Заполнить массив данными текущей директории
			GetDir(dir, count, path);

			//убрать маску
			strncpy_s(path, strlen(path) - 3, path, strlen(path) - 4);

			//Передаю управление юзеру и узнаю что он хочет
			move = Motion(dir, count, path);

			//Добавить маску к пути
			strcat_s(path, "\\*.*");
		}

		//Возврат по BackSpace
		if (move == -2)
		{
			strcpy_s(path, MAX_PATH, ST.Pop()->history);
			if (ST.GetCount() > 1)
				ST.Del();
		}

		//Если не BackSpace и не ошибка - запоминаю историю
		if (move != -2 && count != -1 && move != -3)
			ST.Push(path);

		//Сменить диск
		if (move == -4)
		{
			int countDisks = GetDisks(disk);
			char temp = ChangeDisk(disk, countDisks);
			if (temp != '!')
			{
				path[0] = temp;
				path[1] = '\0';
				strcat_s(path, ":\\*.*");
			}
			else
				move = -1;
		}

		//убрать маску
		strncpy_s(path, strlen(path) - 3, path, strlen(path) - 4);

		// Переход в родительский каталог
		if (move == 0 && (dir[move].attrib & _A_SUBDIR) || move == 1 && (dir[move].attrib & _A_SUBDIR))
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
		if (move > 1 && (dir[move].attrib & _A_SUBDIR))
		{
			strcat_s(path, "\\");
			strcat_s(path, dir[move].name);
		}

		//Если файл - вывести атрибуты
		if (move > -1 && !(dir[move].attrib & _A_SUBDIR))
		{
			char buffer[30];
			ctime_s(buffer, _countof(buffer), &dir[move].time_write);
			strncpy_s(buffer, 25, buffer, 24);
			cout << dir[move].name << endl << endl;
			cout << "RDO | HID | SYS | ARC |           DATE           | SIZE" << endl;
			cout << "--- + --- + --- + --- +--------------------------+ ----" << endl;
			cout << ((dir[move].attrib & _A_RDONLY) ? " Y  " : " N  ") << "|";
			cout << " " << ((dir[move].attrib & _A_HIDDEN) ? " Y  " : " N  ") << "|";
			cout << " " << ((dir[move].attrib & _A_SYSTEM) ? " Y  " : " N  ") << "|";
			cout << " " << ((dir[move].attrib & _A_ARCH) ? " Y  " : " N  ") << "|";
			cout << " " << buffer << " | " << dir[move].size << endl << endl;
			system("pause");
		}
		dir = nullptr;
		delete[]dir;
	} while (move != -1);
}