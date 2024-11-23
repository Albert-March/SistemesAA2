#pragma once
#include <mutex>
#include <iostream>
#include <sstream>
#include <Windows.h>

static class ConsoleControl {
private:
	HANDLE _console = GetStdHandle(STD_OUTPUT_HANDLE);
	std::mutex* _consoleMutex = new std::mutex();
	static ConsoleControl GetInstance();

	static HANDLE GetConsole();

public:
	enum ConsoleColor {
		BLACK, DARKBLUE, DARKGREEN, DARKCYAN, DARKRED, DARKMAGENTA, DARKYELLOW, LIGHTGREY,
		DARKGREY, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE
	};

	/// <summary>Change color of text and background</summary>
	static void SetColor(ConsoleColor TextColor = WHITE, ConsoleColor BackgroundColor = BLACK);
	/// <summary>Change position and the next write start here</summary>
	static void SetPosition(short int x, short int y);
	/// <summary>Clear console</summary>
	static void Clear();
	/// <summary>Fill all console with specific character</summary>
	static void FillWithCharacter(char character, ConsoleColor ForeColor, ConsoleColor BackColor);

	/// <summary>Delete buffer for previous pressed keys</summary>
	static void ClearKeyBuffer();
	/// <summary>Try to read the currently pressed key, it does not block the code</summary>
	static int ReadNextKey();
	/// <summary>Reads the next key pressed, this blocks the code until a key is pressed</summary>
	static int WaithForReadNextKey();
	/// <summary>Reads the next character pressed, this blocks the code until a key is pressed</summary>
	static char WaitForReadNextChar();

	/// <summary>Block the private mutex</summary>
	static void Lock();
	/// <summary>Unlock the private mutex</summary>
	static void Unlock();
};

using CC = ConsoleControl;