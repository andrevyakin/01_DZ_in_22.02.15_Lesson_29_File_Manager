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

void main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	
	//������� ������ ��������� ��������� _finddata_t ��� �������� ������ ������� �����������
	_finddata_t* dir = new _finddata_t[2];
	_finddata_t* temp;
	int count = 0;

	//�������� ���� � ������� �����������
	char path [MAX_PATH];
	GetCurrentDirectory(sizeof(path), path);
	strcat_s(path, "\\*.*");
	
	//�������� � ��������� �������� � ������ ����������
	int firstID = _findfirst(path, &dir[0]);

	// ����� � ���������� � ������
	while (_findnext(firstID, &dir[++count]) != -1)
	{
		temp = new _finddata_t[count + 1];			//������� ������ ������ ��� ���������� ������
		for (int i = 0; i < count + 1; i++)
			temp[i] = dir[i];						//������� �������� ������ �� ���������
		delete[] dir;								//������ �������� ������
		dir = new _finddata_t[count + 2];			//������� ������ ������ ��� �������� ������
		for (int i = 0; i < count +1 ; i++)
			dir[i] = temp[i];						//������� ��������
		delete[]temp;								//������ ��� �� ������ ��������� ������
	}
	_findclose(firstID);
	
	//��� ������������
	for (int i = 0; i < count; i++)
		cout << dir[i].name << endl;

	delete[]dir;
}