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
	::SetConsoleCP(65001);
	::SetConsoleOutputCP(65001);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::wifstream inf("russian_names.csv");
	if (inf.is_open())
	{
		for (int i = 0; i < 10 && !inf.eof(); i++) {
			std::wstring row;
			std::getline(inf, row, L'\n');
			std::wstringstream readed_data(row);
			while(!readed_data.eof())
			{
				std::wstring data;
				std::getline(readed_data, data, L';');
				std::wcout << std::setw(20) <<std::setiosflags(std::ios::left) << data ;
				for (
					COORD coord = GetConsoleCursorPosition(hConsole);
					coord.X % 21;
					coord.X += 1,
					SetConsoleCursorPosition(hConsole, { coord.X,coord.Y })
					);
				
			}
			std::wcout << std::endl;
		}
	}
}