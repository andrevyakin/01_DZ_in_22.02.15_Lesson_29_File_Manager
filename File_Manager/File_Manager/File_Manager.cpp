//��� ����� ������� (���� �������� ��� ����)
//1. �������� ���������� ������� ���������� � ������ ���, �����, � ��������� (���, ���). 
//2. ������ ����� ������ �� ������� ���������� � �������, �������� �� 20 ����� �� ������, � ����������� �������� ������� �� ���� ������� � ������� �������
//3. ��� ������� Enter - ���� ���������� - ��������� �. 2, � ���� ���� - ������� ��������.
//4. ������� ����� ����� ��� ������� �������, ������� F2, ��� ���� ������ ����� ����� ���� �� �����.

#undef UNICODE			//��������� UNICODE, �������� �� ��������� � ���������� VS
						//��� ����� ��� ���������� ������ ������� GetCurrentDirectory
#include <iostream>
#include <iomanip>		//��� setw() � setfill()
#include <Windows.h>	//��� ����������� ��������� � ������ �������� ����� (SetConsoleOutputCP() � SetConsoleCP()), � ����� ��������� ����� � �������
#include <io.h>			//��� ������ ������
#include <conio.h>		//��� _getch()
using namespace std;

//�������� ���������� ��������� � ����������
int GetCount(char* path)
{
	int count = 1;
	_finddata_t temp;

	//�������� �������� � ������ ����������
	int firstID = _findfirst(path, &temp);

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

//�������� ���������
int Motion(_finddata_t* dir, int count)
{
	int y = 0, move = 0;

	SetConsoleCursorPosition(0, 0);
	SetConsoleColorTextBackground(clWhite, clGreen);
	//����������� ����� ������� �������
	cout << dir[0].name;

	while (move != ENTER && move != ESC && move != BackSpase)
	{
		//�������� ������� �������
		move = _getch();
		if (move == 0xe0 || !move)
			move = _getch();

		//�������� ��� �������, ����� �����������
		if (move != UP && move != DOWN && move != ENTER && move != ESC && move != BackSpase)
			move = 0;

		if (move)
		{
			SetConsoleCursorPosition(0, y);
			SetConsoleColorTextBackground(clGray, clBlack);
			//����� ��� ������
			cout << dir[y].name;

			if (move == UP && y > 0)
				y--;
			if (move == DOWN && y < count - 1)
				y++;

			SetConsoleCursorPosition(0, y);
			SetConsoleColorTextBackground(clWhite, clGreen);
			//����� ������ ������
			cout << dir[y].name;

			//� ��������� ����� � ����-�������
			SetConsoleColorTextBackground(clGray, clBlack);
		}

		if (move == ESC)
			y = -1;

	}
	SetConsoleCursorPosition(0, count);
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

	do
	{
		//�������� ����� � ����
		strcat_s(path, "\\*.*");

		//������ ���������� ��������� � ����������
		int count = GetCount(path);
		//������� ������ ��������� ��������� _finddata_t ��� �������� ������ ������� �����������
		_finddata_t* dir = new _finddata_t[count];
		GetDir(dir, count, path);

		// ��� ������������
		for (int i = 0; i < count; i++)
			cout << dir[i].name << endl;

		move = Motion(dir, count);

		//������ ����� (4-�� ��������� �������)
		strncpy_s(path, strlen(path) - 3, path, strlen(path) - 4);

		// ������� � ������������ �������
		if (move == 0 && dir[move].attrib & _A_SUBDIR || move == 1 && dir[move].attrib & _A_SUBDIR)
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
		}

		// ������� � ��������� �������
		if (move != 0 && dir[move].attrib & _A_SUBDIR && move != 1 && dir[move].attrib & _A_SUBDIR)
		{
			strcat_s(path, "\\");
			strcat_s(path, dir[move].name);
		}

		delete[]dir;
		system("cls");
	} while (move != -1);
}