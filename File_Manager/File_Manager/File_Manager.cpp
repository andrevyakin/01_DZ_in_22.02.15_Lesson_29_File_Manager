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
using namespace std;

//Получить количество элементов в директории
int GetCount()
{
	int count = 1;
	_finddata_t temp;
	//Получить путь к текущей дирректории
	char path[MAX_PATH];
	GetCurrentDirectory(sizeof(path), path);
	strcat_s(path, "\\*.*");
	//Получить и сохранить сведения о первом экземпляре
	int firstID = _findfirst(path, &temp);

	while (_findnext(firstID, &temp) != -1)
		++count;
	_findclose(firstID);
	return count;
}

//Получить данные текущей директории
void GetDir(_finddata_t* dir, int count)
{
	//Получить путь к текущей дирректории
	char path[MAX_PATH];
	GetCurrentDirectory(sizeof(path), path);
	strcat_s(path, "\\*.*");
	//Получить и сохранить сведения о первом экземпляре
	int firstID = _findfirst(path, &dir[0]);
	// Поиск и сохранение в массив

	for (int i = 1; i < count; i++)
		_findnext(firstID, &dir[i]);
	_findclose(firstID);
}

void main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	//Узнать количество элементов в директории
	int count = GetCount();
	//Создать массив элементов структуры _finddata_t для хранения данных текущей дирректории
	_finddata_t* dir = new _finddata_t[count];
	GetDir(dir, count);

	// Для самопроверки
	for (int i = 0; i < count; i++)
		cout << dir[i].name << endl;

	delete[]dir;
}