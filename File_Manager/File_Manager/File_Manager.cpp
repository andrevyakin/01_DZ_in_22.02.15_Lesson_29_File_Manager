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
#include <ctime>
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

//�������� ��������� � ����� ���������� � �������
int Motion(_finddata_t* dir, int count)
{
	int y = 0, move = 0;
	char info[MAX_PATH];

	//����� ��������� ����� ������ �� 40 ��������, ����� �� ������ �������
	_finddata_t* temp = new _finddata_t[count];
	for (int i = 0; i < count; i++)
		strncpy_s(temp[i].name, 41, dir[i].name, 40);
	
	system("cls");
	//����� � �������
	for (int i = 0; i < count; i++)
	{
		// ��������� ���������� ��� ���
		dir[i].attrib&_A_SUBDIR ? strcpy_s(info, "�������") : strcpy_s(info, "����");
		SetConsoleCursorPosition(0, y + i);
		cout << temp[i].name;
		SetConsoleCursorPosition(50, y + i);
		cout << info;
	}

	SetConsoleCursorPosition(0, 0);
	SetConsoleColorTextBackground(clWhite, clGreen);
	//����������� ����� ������� �������
	dir[0].attrib&_A_SUBDIR ? strcpy_s(info, "�������") : strcpy_s(info, "����");
	cout << temp[0].name;
	SetConsoleCursorPosition(50, 0);
	cout << info;

	while (move != ENTER && move != ESC && move != BackSpase && move != F1 && move != F2)
	{
		//�������� ������� �������
		move = _getch();
		if (move == 0xe0 || !move)
			move = _getch();

		//�������� ��� �������, ����� �����������
		if (move != UP && move != DOWN && move != ENTER && move != ESC && move != BackSpase && move != F1 && move != F2)
			move = 0;

		if (move)
		{
			SetConsoleCursorPosition(0, y);
			SetConsoleColorTextBackground(clGray, clBlack);
			//����� ��� ������
			dir[y].attrib&_A_SUBDIR ? strcpy_s(info, "�������") : strcpy_s(info, "����");
			cout << temp[y].name;
			SetConsoleCursorPosition(50, y);
			cout << info;

			if (move == UP && y > 0)
				y--;
			if (move == DOWN && y < count - 1)
				y++;

			SetConsoleCursorPosition(0, y);
			SetConsoleColorTextBackground(clWhite, clGreen);
			//����� ������ ������
			dir[y].attrib&_A_SUBDIR ? strcpy_s(info, "�������") : strcpy_s(info, "����");
			cout << temp[y].name;
			SetConsoleCursorPosition(50, y);
			cout << info;

			//� ��������� ����� � ����-�������
			SetConsoleColorTextBackground(clGray, clBlack);
		}

		if (move == ESC)
			y = -1;
		if (move == BackSpase)
			y = -2;
		if (move == F1)
			y = -3;
		if (move == F2)
			y = -3;
	}
	SetConsoleCursorPosition(0, count);
	delete[]temp;
	return y;
}

//������ ��������� � ������� �����, � �.�. ���������� � �������
void GetDisks(char* disk)
{
	int count = 0;

	for (int i = 0; i < 26; i++)
	if ((GetLogicalDrives() >> i) & 1)

		//������������ GetLogicalDrives() �������� �������� ���������� ���������� ������, ������������ ��������� �����.
		//1 - ���� ����. 0 - ��� �����.
		//������� ��� (����� �������) ������������� ����� A:, ������ ��� - ����� B:, ������ - ����� C: � �.�.
		//��������� ������� �������� � ���������� ���� � �������� ��������� � 1 

		disk[count++] = char(65 + i); // ����� �����. Char 65 - ��� A, � �.�.
}

//������ :))
void Help()
{
	system("cls");
	cout << "\n\t�������� �� �������� �������������� ��������� ����� � ����." << endl;
	cout << "\n\t����� - Enter." << endl;
	cout << "\n\tBackSpace - ������� � ���������� �������." << endl;
	cout << "\n\tF1 - ����� ���� �������." << endl;
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
	int move = -3;
	char path[MAX_PATH];
	char temp[MAX_PATH];

	char disk[26]; //������ ��� �������� ���������� � ������ �����. 26 ���� � ���������� ��������. ����� ������ �� ���������� ��� ����� �����.

	GetCurrentDirectory(sizeof(path), path);
	strcpy_s(temp, path);

	do
	{
		
		if (move == -3)
			Help();
		
		//�������� ����� � ����
		strcat_s(path, "\\*.*");

		//������ ���������� ��������� � ����������
		int count = GetCount(path);

		//������� ������ ��������� ��������� _finddata_t ��� �������� ������ ������� �����������
		_finddata_t* dir = new _finddata_t[count];
		
		//��������� ������ ������� ������� ����������
		GetDir(dir, count, path);
		
		//������� ���������� ����� � ����� ��� �� �����
		move = Motion(dir, count);
				
		if (move == -2)
			strcpy_s(path, temp);
						
		strcpy_s(temp, path);

		//������ ����� (4-�� ��������� �������)
		strncpy_s(path, strlen(path) - 3, path, strlen(path) - 4);

		// ������� � ������������ �������
		if (move == 0 || move == 1)
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
		if ((dir[move].attrib & _A_SUBDIR) && move > 1)
		{
			strcat_s(path, "\\");
			strcat_s(path, dir[move].name);
		}

		//���� ���� - ������� ��������
		if (!(dir[move].attrib & _A_SUBDIR) && move > -1)
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