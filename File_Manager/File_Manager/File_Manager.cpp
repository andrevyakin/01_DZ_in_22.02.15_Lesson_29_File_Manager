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
int GetCount()
{
	int count = 1;
	_finddata_t temp;
	//�������� ���� � ������� �����������
	char path[MAX_PATH];
	GetCurrentDirectory(sizeof(path), path);
	strcat_s(path, "\\*.*");
	//�������� � ��������� �������� � ������ ����������
	int firstID = _findfirst(path, &temp);

	while (_findnext(firstID, &temp) != -1)
		++count;
	_findclose(firstID);
	return count;
}

//�������� ������ ������� ����������
void GetDir(_finddata_t* dir, int count)
{
	//�������� ���� � ������� �����������
	char path[MAX_PATH];
	GetCurrentDirectory(sizeof(path), path);
	strcat_s(path, "\\*.*");
	//�������� � ��������� �������� � ������ ����������
	int firstID = _findfirst(path, &dir[0]);
	// ����� � ���������� � ������

	for (int i = 1; i < count; i++)
		_findnext(firstID, &dir[i]);
	_findclose(firstID);
}

void main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	//������ ���������� ��������� � ����������
	int count = GetCount();
	//������� ������ ��������� ��������� _finddata_t ��� �������� ������ ������� �����������
	_finddata_t* dir = new _finddata_t[count];
	GetDir(dir, count);

	// ��� ������������
	for (int i = 0; i < count; i++)
		cout << dir[i].name << endl;

	delete[]dir;
}