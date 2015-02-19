#undef UNICODE			//��������� UNICODE, �������� �� ��������� � ���������� VS
//��� ����� ��� ���������� ������ ������� GetCurrentDirectory � char
#include <iostream>
#include <Windows.h>	//��� ����������� ��������� � ������ �������� ����� (SetConsoleOutputCP() � SetConsoleCP()), � ����� ��������� ����� � �������
#include <io.h>			//��� ������ ������
#include <conio.h>		//��� _getch()
#include <ctime>		//��� ctime_s
using namespace std;

//��� ������� ������� � ������, ���� ������������� �����.
struct Elem
{
	char history[MAX_PATH];
	Elem * next, *prev;
};

class List
{
	Elem *Tail; //������ �� �����. "������� �����!" (�).
	int Count;

public:

	List()
	{
		Tail = NULL;
		Count = 0;
	}

	//���������� �������, ���� ���� �������.
	~List()
	{
		while (Count)
			Del();
	}

	//��������� ������� � �����
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

	//�������� ��������� �������
	Elem * Pop()
	{
		return Tail;
	}

	//������ ���������� ���������
	int GetCount()
	{
		return Count;
	}

	//������� ��������� �������
	void List::Del()
	{
		Elem * Del = Tail;
		Elem * PrevDel = Del->prev;
		Tail = PrevDel;
		delete Del;
		Count--;
	}
};

//�������� ���������� ��������� � ����������
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

//�������� ������ ������� ����������
void GetDir(_finddata_t* dir, int count, char* path)
{
	//�������� � ��������� �������� � ������ ����������
	int firstID = _findfirst(path, &dir[0]);

	// ����� � ���������� � ������
	for (int i = 1; i < count; i++)
		_findnext(firstID, &dir[i]);
	_findclose(firstID);
}

//���������� �����
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

//���������������� �������
void SetConsoleCursorPosition(short x_position, short y_position)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD _coordPosition = { x_position, y_position };
	SetConsoleCursorPosition(hConsole, _coordPosition);
}

//����������� ������ ����������
enum VKey
{
	UP = 72, LEFT = 75, RIGHT = 77, DOWN = 80,
	ENTER = 13, ESC = 27, BackSpase = 8,
	F1 = 59, F2 = 60, PageUp = 73, PageDown = 81
};

//�������� ��������� � ����� ���������� � �������
int Motion(_finddata_t* dir, int count, char* path)
{
	int y = 0, move = 0, block = 0;
	char info[MAX_PATH];

	//����� ��������� ����� ������ �� 40 ��������, ����� �� ������ �������
	_finddata_t* temp = new _finddata_t[count];
	for (int i = 0; i < count; i++)
		strncpy_s(temp[i].name, 41, dir[i].name, 40);

	//����� � �������/. ������� ���� ��� ������ ������� �� 20 ���������. ��� PageUp, PageDown
	do
	{
		system("cls");
		move = y = 0;

		for (int i = block; i < count; i++)
		{
			// ��������� ���������� ��� ���
			dir[i].attrib&_A_SUBDIR ? strcpy_s(info, "�������") : strcpy_s(info, "����");

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

		//����������� ����� ������� �������
		SetConsoleCursorPosition(0, 0);
		SetConsoleColorTextBackground(clWhite, clGreen);
		dir[block].attrib&_A_SUBDIR ? strcpy_s(info, "�������") : strcpy_s(info, "����");
		cout << temp[block].name;
		SetConsoleCursorPosition(50, 0);
		cout << info;

		//���������� ���� ��� �������� ��������� �� ������
		while (move != ENTER && move != ESC && move != BackSpase && move != F1 && move != F2 && move != PageDown && move != PageUp)
		{
			//�������� ������� �������
			move = _getch();
			if (move == 0xe0 || !move)
				move = _getch();

			//�������� ��� �������, ����� �����������
			if (move != UP && move != DOWN && move != ENTER && move != ESC && move != BackSpase && move != F1 && move != F2 && move != PageDown && move != PageUp)
				move = 0;

			//�������� ���������
			if (move)
			{
				//����� ��� ������
				SetConsoleCursorPosition(0, y);
				SetConsoleColorTextBackground(clGray, clBlack);
				dir[y + block].attrib&_A_SUBDIR ? strcpy_s(info, "�������") : strcpy_s(info, "����");
				cout << temp[y + block].name;
				SetConsoleCursorPosition(50, y);
				cout << info;

				if (move == UP && y > 0)
					y--;
				if (move == DOWN && y + block < count - 1 && y < 19)
					y++;

				//����� ������ ������
				SetConsoleCursorPosition(0, y);
				SetConsoleColorTextBackground(clWhite, clGreen);
				dir[y + block].attrib&_A_SUBDIR ? strcpy_s(info, "�������") : strcpy_s(info, "����");
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

//������ ��������� � ������� �����, � �.�. ���������� � �������
int GetDisks(char* disk)
{
	int count = 0;

	for (int i = 0; i < 26; i++)
	if ((GetLogicalDrives() >> i) & 1)

		//������������ GetLogicalDrives() �������� �������� ���������� ���������� ������, ������������ ��������� �����.
		//1 - ���� ����. 0 - ��� �����.
		//������� ��� (����� �������) ������������� ����� A:, ������ ��� - ����� B:, ������ - ����� C: � �.�.
		//��������� ������� �������� � ���������� ���� � �������� ��������� � 1 

		disk[count++] = char(65 + i); // ����� �����. Char 65 - ��� A, � �.�.

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
		//�������� ������� �������
		move = _getch();
		if (move == 0xe0 || !move)
			move = _getch();

		//�������� ��� �������, ����� �����������
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
			//����� ��� ������
			SetConsoleCursorPosition(0, y);
			SetConsoleColorTextBackground(clGray, clBlack);
			cout << disk[y] << ":\\";

			if (move == UP && y > 0)
				y--;
			if (move == DOWN && y < countDisks - 1)
				y++;

			//����� ������ ������
			SetConsoleCursorPosition(0, y);
			SetConsoleColorTextBackground(clWhite, clGreen);
			cout << disk[y] << ":\\";
			SetConsoleColorTextBackground(clGray, clBlack);
		}
	}
	SetConsoleCursorPosition(0, countDisks);
	return disk[y];
}

//������ :))
void Help()
{
	system("cls");
	cout << "\n\t�������� �� �������� �������������� ��������� ����� � ����." << endl;
	cout << "\n\t����� - Enter." << endl;
	cout << "\n\tBackSpace - ������� � ���������� �������." << endl;
	cout << "\n\tF1 - ����� ���� �������." << endl;
	cout << "\n\tPageDown � PageUp ������� �� ���������, ��� ���������� ��������." << endl;
	SetConsoleColorTextBackground(clCyan, clBlack);
	cout << "\n\tF2 - ������� ����." << endl;
	SetConsoleColorTextBackground(clGray, clBlack);
	cout << "\n\tEsc - ����� �� ���������.\n" << endl;
	system("pause");
}

void main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	//������� ����
	int move = -3;

	//���� � ����������
	char path[MAX_PATH];

	//������ ��������� ��������� _finddata_t ��� �������� ������ ������� �����������
	_finddata_t* dir = nullptr;

	//������-���� ��� �������� �������
	List ST;

	//������ ��� �������� ���������� � ������ �����. 26 ���� � ���������� ��������. ����� ������ �� ���������� ��� ����� �����.
	char disk[26];

	//������� ���� � ������� ����������
	GetCurrentDirectory(sizeof(path), path);

	do
	{
		if (move == -3)
			Help();

		//�������� ����� � ����
		strcat_s(path, "\\*.*");

		//������ ���������� ��������� � ����������
		int count = GetCount(path);

		//���� ������, ��������� � ����������� ��������
		if (count == -1)
		{
			cout << "\n\t�������� ��� � ����������, ��� ������ ������ �������..." << endl;
			cout << "\n\t�������� ������ ����...\n" << endl;
			move = -4;
			system("pause");
		}
		else
		{
			dir = new _finddata_t[count];

			//��������� ������ ������� ������� ����������
			GetDir(dir, count, path);

			//������ �����
			strncpy_s(path, strlen(path) - 3, path, strlen(path) - 4);

			//������� ���������� ����� � ����� ��� �� �����
			move = Motion(dir, count, path);

			//�������� ����� � ����
			strcat_s(path, "\\*.*");
		}

		//������� �� BackSpace
		if (move == -2)
		{
			strcpy_s(path, MAX_PATH, ST.Pop()->history);
			if (ST.GetCount() > 1)
				ST.Del();
		}

		//���� �� BackSpace � �� ������ - ��������� �������
		if (move != -2 && count != -1 && move != -3)
			ST.Push(path);

		//������� ����
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

		//������ �����
		strncpy_s(path, strlen(path) - 3, path, strlen(path) - 4);

		// ������� � ������������ �������
		if (move == 0 && (dir[move].attrib & _A_SUBDIR) || move == 1 && (dir[move].attrib & _A_SUBDIR))
		{
			//����� ����� ���������� �����
			char *end = strrchr(path, '\\');
			if (end) //���� ����� ����
			{
				//������ ������� ������ ��������			
				int delta = end - path;
				//������ ������ �������
				strncpy_s(path, delta + 1, path, delta);
			}
			else //������������� �������� ���. ������� � ��� �� ������� ������.
			{
				strcat_s(path, "\\");
				strcat_s(path, dir[move].name);
			}
		}

		// ������� � ��������� �������
		if (move > 1 && (dir[move].attrib & _A_SUBDIR))
		{
			strcat_s(path, "\\");
			strcat_s(path, dir[move].name);
		}

		//���� ���� - ������� ��������
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