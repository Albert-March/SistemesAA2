#include "ConsoleControl.h"
#include <conio.h>

HANDLE ConsoleControl::GetConsole()
{
	return GetInstance()._console;
}

void ConsoleControl::SetColor(ConsoleColor TextColor, ConsoleColor BackgroundColor)
{
	WORD color = (BackgroundColor << 4) | TextColor;
	SetConsoleTextAttribute(GetConsole(), color);
}

void ConsoleControl::SetPosition(short int x, short int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetConsole(), pos);
}

void ConsoleControl::Clear()
{
	std::cout << "\033[2J\033[1;1H";//Clear the console and move the cursor to the top left corner
	//ClearCharacter(' ', WHITE, BLACK);//Another less optimal way to clean
}

void ConsoleControl::FillWithCharacter(char character, ConsoleColor TextColor, ConsoleColor BackgroundColor)
{
	COORD topLeft = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;
	HANDLE console = GetConsole();
	WORD color = (BackgroundColor << 4) | TextColor;
	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, character, screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, color,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

void ConsoleControl::ClearKeyBuffer()
{
	while (_kbhit()) {
		_getch();
	}
}

int ConsoleControl::ReadNextKey()
{
	int KB_code = 0;

	if (_kbhit())
	{
		KB_code = _getch();
	}
	return KB_code;
}

int ConsoleControl::WaithForReadNextKey()
{
	int KB_code = 0;

	while (KB_code == 0)
	{
		if (_kbhit())
		{
			KB_code = _getch();
		}
	}

	return KB_code;
}

char ConsoleControl::WaitForReadNextChar()
{
	char c = 0;

	while (c == 0)
	{
		if (_kbhit())
		{
			c = _getch();
		}
	}

	return c;
}

ConsoleControl ConsoleControl::GetInstance() {
	static ConsoleControl instance;

	return instance;
}

void ConsoleControl::Lock()
{
	GetInstance()._consoleMutex->lock();
}

void ConsoleControl::Unlock()
{
	GetInstance()._consoleMutex->unlock();
}
