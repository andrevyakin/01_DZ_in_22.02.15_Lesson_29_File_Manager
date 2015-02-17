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
//// Функция, которая убирает лишние слеши и пробелы справа 
//void RemoveRSpacesAndRSlashes(char *str)
//{
//	int index = strlen(str) - 1;
//	while (str[index] == '\\' || str[index] == ' ')
//		index--;
//	strncpy_s(str, index + 2, str, index + 1);
//	str[index + 1] = '\0';
//}
//// Функция для показа текущей директории
//void ShowCurrentDir(char path[])
//{
//	cout << path << ">";
//}
//
//// Показ на экран содержимого папки
//bool ShowDir(char path[])
//{
//	// Показ содержимого текущей директории
//	_finddata_t find;
//	char pathfind[MAX_PATH];
//	strcpy_s(pathfind, path);
//	strcat_s(pathfind, "\\*.*");
//	char info[MAX_PATH];
//	int count = 0;
//
//	// Начало Поиска
//	int result = _findfirst(pathfind, &find);
//	// Очистка экрана
//	system("cls");
//	int flag = result;
//	if (flag == -1)
//	{
//		strcpy_s(info, "Такой Директории Нет");
//		cout << info << endl;
//		return false;
//	}
//
//	while (flag != -1)
//	{
//
//		if (strcmp(find.name, ".") && strcmp(find.name, ".."))
//		{
//			// Проверяем Директория или Нет
//			find.attrib&_A_SUBDIR ? strcpy_s(info, " Каталог ") : strcpy_s(info, " Файл ");
//
//			//Укорачиваю имя файла до 40 символов, чтобы вывести в консоле в одну строку
//			strncpy_s(pathfind, 41, find.name, 40);
//			cout << setw(40) << setfill(' ') << pathfind << setw(15) << setfill(' ') << info << endl;
//
//			// Если экран уже заполнен, производим остановку
//			if (++count == 24)
//			{
//				count = 0;
//				cout << "Для продолжения нажмите любую клавишу..." << flush;
//
//				// Ожидаем нажатия любой клавиши
//				_getch();
//				cout << "\n";
//			}
//		}
//		// Продолжаем Поиск
//		flag = _findnext(result, &find);
//	}
//	cout << endl;
//	ShowCurrentDir(path);
//	// Очищаем ресурсы, выделенные под поиск
//	_findclose(result);
//	return true;
//}
//
////Получить имеющиеся в системе диски, в т.ч. логические и съемные
//void GetDisks(char*)
//{
//	char disk[26]; //26 букв в английском алфавите. Винда больше не предлагает для буквы диска.
//	int count = 0;
//
//	for (int i = 0; i < 26; i++)
//	if ((GetLogicalDrives() >> i) & 1)
//
//		//Возвращаемое GetLogicalDrives() значение является коллекцией однобитных флагов, определяющих найденные диски.
//		//1 - есть диск. 0 - нет диска.
//		//Нулевой бит (самый младший) соответствует диску A:, первый бит - диску B:, второй - диску C: и т.д.
//		//Побитовым сдвигом перехожу к следующему биту и побитово сравниваю с 1 
//
//		disk[count++] = char(65 + i); // Буква диска. Char 65 - это A, и т.д.
//}
//
//void main()
//{
//	SetConsoleOutputCP(1251);
//	SetConsoleCP(1251);
//
//	// В данной переменной будет храниться путь к Директории
//	char path[MAX_PATH];
//	// В данной переменной будет команда, введенная пользователем
//	char action[size];
//	// Временная переменная
//	char temp[MAX_PATH];
//	// Получаем Путь к текущей Директории
//	GetCurrentDirectory(sizeof(path), path);
//
//	bool flag = true;
//
//	// Показ содержимого текущей директории
//	ShowDir(path);
//	do
//	{
//		// Ввод команды пользователя
//		cin.getline(action, size);
//		// Убираем пробелы и слэши справа
//		RemoveRSpacesAndRSlashes(action);
//		// Переход в корневой каталог
//		if (!_strcmpi(action, "root"))
//		{
//			path[2] = '\0';
//			ShowDir(path);
//		}
//		// Проверка на желание пользователя выйти 
//		else if (!_strcmpi(action, "exit"))
//			flag = false;
//
//		// Проверка на команду cd
//		else if (!_strnicmp(action, "cd", 2))
//		{
//			// Показ содержимого текущей директории
//			if ((!_strcmpi(action, "cd")))
//			{
//				// Показ Директории
//				ShowDir(path);
//			}
//			// Команда cd была дана с параметрами 
//			else if (!_strnicmp(action, "cd ", 3))
//			{
//				// Находим индекс параметра 
//				int index = strspn(action + 2, " ");
//				if (index)
//				{
//					// Проверка на полный путь к Директории
//					if (strchr(action + index + 2, ':'))
//					{
//						// Попытка отобразить содержимое Директории	
//						if (strcpy_s(path, action + index + 2))
//						{
//							(ShowDir(action + index + 2));
//						}
//						else
//						{
//							// Произошла Ошибка 	
//							ShowCurrentDir(path);
//						}
//					}
//					// Поднимаемся в родительский каталог
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
//							// Произошла Ошибка 	
//							ShowCurrentDir(path);
//						}
//					}
//					// Показ Директории
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
//						// Был Дан неполный путь 
//						strcpy_s(temp, path);
//						strcat_s(temp, "\\");
//						strcat_s(temp, action + index + 2);
//						// Попытка отобразить содержимое Директории	
//						if (ShowDir(temp))
//						{
//							strcpy_s(path, temp);
//						}
//						else
//						{
//							// Произошла Ошибка 	
//							ShowCurrentDir(path);
//						}
//					}
//				}
//				else
//				{
//					// Показ Директории
//					ShowDir(path);
//				}
//			}
//			else
//			{
//				// Показ Директории
//				ShowDir(path);
//			}
//		}
//		else
//		{
//			// Показ Директории
//			ShowDir(path);
//		}
//	} while (flag);
//
//}