#undef UNICODE			//Отключает UNICODE, заданный по умолчанию в настройках VS
						//это нужно для корректной работы функции GetCurrentDirectory с char
#include <iostream>
#include <Windows.h>	//для корректного получения и вывода русского языка (SetConsoleOutputCP() и SetConsoleCP()), а также изменения цвета в консоле
#include <io.h>			//для поиска файлов
#include <conio.h>		//для _getch()
#include <ctime>		//для ctime_s
using namespace std;

class Stack
{
	// Нижняя и верхняя границы стека
	enum { EMPTY = -1, FULL = 30 };

	// Массив для хранения данных
	char st[FULL + 1][MAX_PATH];

	// Указатель на вершину стека
	int top;

public:
	
	// Конструктор
	Stack();

	// Добавление элемента
	void Push(char* history);

	// Извлечение элемента
	char Pop(char* history);

	// Очистка стека
	void Clear();

	// Проверка существования элементов в стеке
	bool IsEmpty();

	// Проверка на переполнение стека
	bool IsFull();

	// Количество элементов в стеке
	int GetCount();
};

Stack::Stack()
{
	// Изначально стек пуст
	top = EMPTY;
}

void Stack::Clear()
{
	top = EMPTY;
}

bool Stack::IsEmpty()
{
	// Пуст?
	return top == EMPTY;
}

bool Stack::IsFull()
{
	// Полон?
	return top == FULL;
}

int Stack::GetCount()
{
	// Количество присутствующих в стеке элементов
	return top + 1;
}

void Stack::Push(char* history)
{
	// Если в стеке есть место, то увеличиваем указатель
	// на вершину стека и вставляем новый элемент
	if (!IsFull())
		strcpy_s(st[++top], history);
}

char Stack::Pop(char* history)
{
	// Если в стеке есть элементы, то возвращаем верхний и
	// уменьшаем указатель на вершину стека
	if (!IsEmpty())
	{
		strcpy_s(history, MAX_PATH, st[top--]);
		return 1;
	}
	else // Если в стеке элементов нет
		return 0;
}

//Получить количество элементов в директории
int GetCount(char* path)
{
	int count = 1;
	_finddata_t temp;

	//Получить сведения о первом экземпляре
	int firstID = _findfirst(path, &temp);

	if (firstID == -1)
	{
		system("cls");
		cout << "Носителя нет в устройстве" << endl;
		return -1;
	}
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

	//Вывод в консоль
	do
	{
		if (move != ESC && move != BackSpase && move != F1 && move != F2 && move != ENTER)
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

		SetConsoleCursorPosition(0, 0);
		SetConsoleColorTextBackground(clWhite, clGreen);
		//Подсвечиваю самую верхнюю позицию
		dir[block].attrib&_A_SUBDIR ? strcpy_s(info, "Каталог") : strcpy_s(info, "Файл");
		cout << temp[block].name;
		SetConsoleCursorPosition(50, 0);
		cout << info;

		while (move != ENTER && move != ESC && move != BackSpase && move != F1 && move != F2 && move != PageDown && move != PageUp)
		{
			//считываю нажатые клавиши
			move = _getch();
			if (move == 0xe0 || 0)
				move = _getch();

			//исключаю все клавиши, кроме разрешенных
			if (move != UP && move != DOWN && move != ENTER && move != ESC && move != BackSpase && move != F1 && move != F2 && move != PageDown && move != PageUp)
				move = 0;

			//движение стрелками
			if (move)
			{
				SetConsoleCursorPosition(0, y);
				SetConsoleColorTextBackground(clGray, clBlack);
				//здесь был курсор
				dir[y + block].attrib&_A_SUBDIR ? strcpy_s(info, "Каталог") : strcpy_s(info, "Файл");
				cout << temp[y + block].name;
				SetConsoleCursorPosition(50, y);
				cout << info;

				if (move == UP && y > 0)
					y--;
				if (move == DOWN && y + block < count - 1 && y < 19)
					y++;

				SetConsoleCursorPosition(0, y);
				SetConsoleColorTextBackground(clWhite, clGreen);
				//здесь курсор сейчас
				dir[y + block].attrib&_A_SUBDIR ? strcpy_s(info, "Каталог") : strcpy_s(info, "Файл");
				cout << temp[y + block].name;
				SetConsoleCursorPosition(50, y);
				cout << info;

				//и вернуться опять к серо-черному
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

	SetConsoleCursorPosition(0, count - block + 3);
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
		if (move != UP && move != DOWN && move != ENTER)
			move = 0;

		if (move)
		{
			SetConsoleCursorPosition(0, y);
			SetConsoleColorTextBackground(clGray, clBlack);
			//здесь был курсор
			cout << disk[y] << ":\\";

			if (move == UP && y > 0)
				y--;
			if (move == DOWN && y < countDisks - 1)
				y++;

			SetConsoleCursorPosition(0, y);
			SetConsoleColorTextBackground(clWhite, clGreen);
			//здесь курсор сейчас
			cout << disk[y] << ":\\";

			//и вернуться опять к серо-черному
			SetConsoleColorTextBackground(clGray, clBlack);
		}
	}
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

	//Путь
	char path[MAX_PATH];
	char history[MAX_PATH];
	Stack ST;

	//массив элементов структуры _finddata_t для хранения данных текущей дирректории
	_finddata_t* dir = nullptr;

	char disk[26]; //Массив для хранения информации о дисках компа. 26 букв в английском алфавите. Винда больше не предлагает для буквы диска.

	GetCurrentDirectory(sizeof(path), path);

	//Добавляю маску к пути
	strcat_s(path, "\\*.*");
	strcpy_s(history, path);
	//Сохранить историю в стек
	ST.Push(history);
	//убрать маску
	strncpy_s(path, strlen(path) - 3, path, strlen(path) - 4);

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
			cout << "Выберите другой диск..." << endl;
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
			char temp = ST.Pop(history);

			temp != 0 ? strcpy_s(path, history) : temp;
		}

		//Сменить диск
		if (move == -4)
		{
			int countDisks = GetDisks(disk);
			path[0] = ChangeDisk(disk, countDisks);
			path[1] = ':';
			path[2] = '\0';
			strcat_s(path, "\\*.*");
		}

		if (move != -2)
		{
			strcpy_s(history, path);
			ST.Push(history);
		}

		//убрать маску
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
		if (move > 1 && (dir[move].attrib & _A_SUBDIR))
		{
			strcat_s(path, "\\");
			strcat_s(path, dir[move].name);
		}

		//Если файл - вывести атрибуты
		if (move > -1 && !(dir[move].attrib & _A_SUBDIR))
		{
			cout << endl;
			cout << "RDO | HID | SYS | ARC |           DATE           | SIZE" << endl;
			cout << "--- + --- + --- + --- +--------------------------+ ----" << endl;

			char buffer[30];
			cout << ((dir[move].attrib & _A_RDONLY) ? " Y  " : " N  ") << "|";
			cout << " " << ((dir[move].attrib & _A_HIDDEN) ? " Y  " : " N  ") << "|";
			cout << " " << ((dir[move].attrib & _A_SYSTEM) ? " Y  " : " N  ") << "|";
			cout << " " << ((dir[move].attrib & _A_ARCH) ? " Y  " : " N  ") << "|";
			ctime_s(buffer, _countof(buffer), &dir[move].time_write);
			strncpy_s(buffer, 25, buffer, 24);
			cout << " " << buffer << " | " << dir[move].size << endl << endl;
			system("pause");
		}
		dir = nullptr;
		delete[]dir;
	} while (move != -1);
	system("cls");
}