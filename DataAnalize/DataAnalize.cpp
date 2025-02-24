#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>

#include <Windows.h>

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		return cbsi.dwCursorPosition;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

int main()
{
	// Установка консоли в режим обработки символов unicode
	::SetConsoleCP(65001);
	::SetConsoleOutputCP(65001);
	//Запоминание программой в каком именно окне она работает
	//необходимо для дальнейших манипуляций с окном
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
	//чтение из потока файла в формате двубайтных символов
	//без этого unicode будет восприниматься как мусор
	std::wifstream inf("russian_names.csv");
	if (inf.is_open())
	{
		//демо вывод того что было в файле в консоль
		for (int i = 0; i < 10 && !inf.eof(); i++) {
			//строки для работы с unicodom тоже нужны особые
			//wstring заточен под хранение двубайтных символов и подходит для 
			//наших целей
			std::wstring row;
			std::getline(inf, row, L'\n');//L'' - литерал именно двубайтного символа
			std::wstringstream readed_data(row);//wstringstream для разбивки данных из wide строки
			while(!readed_data.eof())
			{
				std::wstring data;
				std::getline(readed_data, data, L';');
				//с помощью манипуляторов потока выводим данные в красивые таблички
				std::wcout << std::setw(20) <<std::setiosflags(std::ios::left) << data ;
				//весь цикл нужен чтобы поправить беду с восприятием любого символа с id > 255
				//как двух символов. мы смещаем курсор вывода вправо до тех пор пока он
				//не встанет на правильную позицию для дальнейшей работы и всё это тольк
				//для того чтобы мы могли использовать для имён не английские буковки
				for (
					COORD coord = GetConsoleCursorPosition(hConsole);//текущая позиция в консоли
					coord.X % 21;//проверяем что каретка отведена на ширину нашего поля таблицы
					coord.X += 1,//смешаем координату вправо на 1 символ 
					SetConsoleCursorPosition(hConsole, { coord.X,coord.Y })
					//устанавливаем новую позицию для коретки консоли
					);
				//цикл for без тела потому что мы всю деятельность расписали в его заголовке
				
			}
			std::wcout << std::endl;
		}
	}
}