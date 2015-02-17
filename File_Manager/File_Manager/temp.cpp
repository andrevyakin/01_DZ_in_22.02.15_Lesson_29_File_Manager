//#undef UNICODE
//
//#include <iostream>
//#include <iomanip>
//#include <Windows.h>
//#include <io.h>
//#include <conio.h>
//
//using namespace std;
//
//const int size = 255;
//
//// �������, ������� ������� ������ ����� � ������� ������ 
//void RemoveRSpacesAndRSlashes(char *str)
//{
//	int index = strlen(str) - 1;
//	while (str[index] == '\\' || str[index] == ' ')
//		index--;
//	strncpy_s(str, index + 2, str, index + 1);
//	str[index + 1] = '\0';
//}
//// ������� ��� ������ ������� ����������
//void ShowCurrentDir(char path[])
//{
//	cout << path << ">";
//}
//
//// ����� �� ����� ����������� �����
//bool ShowDir(char path[])
//{
//	// ����� ����������� ������� ����������
//	_finddata_t find;
//	char pathfind[MAX_PATH];
//	strcpy_s(pathfind, path);
//	strcat_s(pathfind, "\\*.*");
//	char info[MAX_PATH];
//	int count = 0;
//
//	// ������ ������
//	int result = _findfirst(pathfind, &find);
//	// ������� ������
//	system("cls");
//	int flag = result;
//	if (flag == -1)
//	{
//		strcpy_s(info, "����� ���������� ���");
//		cout << info << endl;
//		return false;
//	}
//
//	while (flag != -1)
//	{
//
//		if (strcmp(find.name, ".") && strcmp(find.name, ".."))
//		{
//			// ��������� ���������� ��� ���
//			find.attrib&_A_SUBDIR ? strcpy_s(info, " ������� ") : strcpy_s(info, " ���� ");
//
//			//���������� ��� ����� �� 40 ��������, ����� ������� � ������� � ���� ������
//			strncpy_s(pathfind, 41, find.name, 40);
//			cout << setw(40) << setfill(' ') << pathfind << setw(15) << setfill(' ') << info << endl;
//
//			// ���� ����� ��� ��������, ���������� ���������
//			if (++count == 24)
//			{
//				count = 0;
//				cout << "��� ����������� ������� ����� �������..." << flush;
//
//				// ������� ������� ����� �������
//				_getch();
//				cout << "\n";
//			}
//		}
//		// ���������� �����
//		flag = _findnext(result, &find);
//	}
//	cout << endl;
//	ShowCurrentDir(path);
//	// ������� �������, ���������� ��� �����
//	_findclose(result);
//	return true;
//}
//
////�������� ��������� � ������� �����, � �.�. ���������� � �������
//void GetDisks(char*)
//{
//	char disk[26]; //26 ���� � ���������� ��������. ����� ������ �� ���������� ��� ����� �����.
//	int count = 0;
//
//	for (int i = 0; i < 26; i++)
//	if ((GetLogicalDrives() >> i) & 1)
//
//		//������������ GetLogicalDrives() �������� �������� ���������� ���������� ������, ������������ ��������� �����.
//		//1 - ���� ����. 0 - ��� �����.
//		//������� ��� (����� �������) ������������� ����� A:, ������ ��� - ����� B:, ������ - ����� C: � �.�.
//		//��������� ������� �������� � ���������� ���� � �������� ��������� � 1 
//
//		disk[count++] = char(65 + i); // ����� �����. Char 65 - ��� A, � �.�.
//}
//
//void main()
//{
//	SetConsoleOutputCP(1251);
//	SetConsoleCP(1251);
//
//	// � ������ ���������� ����� ��������� ���� � ����������
//	char path[MAX_PATH];
//	// � ������ ���������� ����� �������, ��������� �������������
//	char action[size];
//	// ��������� ����������
//	char temp[MAX_PATH];
//	// �������� ���� � ������� ����������
//	GetCurrentDirectory(sizeof(path), path);
//
//	bool flag = true;
//
//	// ����� ����������� ������� ����������
//	ShowDir(path);
//	do
//	{
//		// ���� ������� ������������
//		cin.getline(action, size);
//		// ������� ������� � ����� ������
//		RemoveRSpacesAndRSlashes(action);
//		// ������� � �������� �������
//		if (!_strcmpi(action, "root"))
//		{
//			path[2] = '\0';
//			ShowDir(path);
//		}
//		// �������� �� ������� ������������ ����� 
//		else if (!_strcmpi(action, "exit"))
//			flag = false;
//
//		// �������� �� ������� cd
//		else if (!_strnicmp(action, "cd", 2))
//		{
//			// ����� ����������� ������� ����������
//			if ((!_strcmpi(action, "cd")))
//			{
//				// ����� ����������
//				ShowDir(path);
//			}
//			// ������� cd ���� ���� � ����������� 
//			else if (!_strnicmp(action, "cd ", 3))
//			{
//				// ������� ������ ��������� 
//				int index = strspn(action + 2, " ");
//				if (index)
//				{
//					// �������� �� ������ ���� � ����������
//					if (strchr(action + index + 2, ':'))
//					{
//						// ������� ���������� ���������� ����������	
//						if (strcpy_s(path, action + index + 2))
//						{
//							(ShowDir(action + index + 2));
//						}
//						else
//						{
//							// ��������� ������ 	
//							ShowCurrentDir(path);
//						}
//					}
//					// ����������� � ������������ �������
//					else if (!strcmp(action + index + 2, ".."))
//					{
//						char *result = strrchr(path, '\\');
//						if (result)
//						{
//							int delta = result - path;
//							strncpy_s(temp, delta + 1, path, delta);
//							temp[delta] = '\0';
//						}
//						else
//						{
//							strcpy_s(temp, path);
//						}
//
//						if (ShowDir(temp))
//						{
//							strcpy_s(path, temp);
//						}
//						else
//						{
//							// ��������� ������ 	
//							ShowCurrentDir(path);
//						}
//					}
//					// ����� ����������
//					else if (!strcmp(action + index + 2, "."))
//					{
//						ShowDir(path);
//					}
//					else if (!strcmp(action + index + 2, "/"))
//					{
//						ShowDir(path);
//					}
//					else
//					{
//						// ��� ��� �������� ���� 
//						strcpy_s(temp, path);
//						strcat_s(temp, "\\");
//						strcat_s(temp, action + index + 2);
//						// ������� ���������� ���������� ����������	
//						if (ShowDir(temp))
//						{
//							strcpy_s(path, temp);
//						}
//						else
//						{
//							// ��������� ������ 	
//							ShowCurrentDir(path);
//						}
//					}
//				}
//				else
//				{
//					// ����� ����������
//					ShowDir(path);
//				}
//			}
//			else
//			{
//				// ����� ����������
//				ShowDir(path);
//			}
//		}
//		else
//		{
//			// ����� ����������
//			ShowDir(path);
//		}
//	} while (flag);
//
//}