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

void main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	
	//Создать массив элементов структуры _finddata_t для хранения данных текущей дирректории
	_finddata_t* dir = new _finddata_t[2];
	_finddata_t* temp;
	int count = 0;

	//Получить путь к текущей дирректории
	char path [MAX_PATH];
	GetCurrentDirectory(sizeof(path), path);
	strcat_s(path, "\\*.*");
	
	//Получить и сохранить сведения о первом экземпляре
	int firstID = _findfirst(path, &dir[0]);

	// Поиск и сохранение в массив
	while (_findnext(firstID, &dir[++count]) != -1)
	{
		temp = new _finddata_t[count + 1];			//Выделяю нужную память под временныый массив
		for (int i = 0; i < count + 1; i++)
			temp[i] = dir[i];						//Копирую основной массив во временный
		delete[] dir;								//Удаляю основной массив
		dir = new _finddata_t[count + 2];			//Выделяю нужную память под основной массив
		for (int i = 0; i < count +1 ; i++)
			dir[i] = temp[i];						//Копирую элементы
		delete[]temp;								//Удаляю уже не нужный временный массив
	}
	_findclose(firstID);
	
	//Для самопроверки
	for (int i = 0; i < count; i++)
		cout << dir[i].name << endl;

	delete[]dir;
}