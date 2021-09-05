#pragma once
#define _WIN32_WINNT 0x0500
#include <windows.h>

#include <iostream>
#include <chrono>
#include <string>



class Console
{
public:
	enum class Color : short {
		Black = 0x0000,
		Dark_Blue = 0x0001,
		Dark_Green = 0x0002,
		Dark_Cyan = 0x0003,
		Dark_Red = 0x0004,
		Dark_Magenta = 0x0005,
		Dark_Yellow = 0x0006,
		Grey = 0x0007,
		Dark_Grey = 0x0008,
		Blue = 0x0009,
		Green = 0x000A,
		Cyan = 0x000B,
		Red = 0x000C,
		Magenta = 0x000D,
		Yellow = 0x000E,
		White = 0x000F,
	};

	struct Pixel
	{
		enum class Type : short { Empty = 0x0000, Full = 0x2588, ThreeQuarters = 0x2593, Half = 0x2592, Quarter = 0x2591 };
		Type type;

		Console::Color color;

		Pixel(Console::Color foreground, Type t) : type(t), color(foreground) {}
		Pixel(Console::Color foreground, Console::Color background, Type t) : Pixel((Console::Color)((short)foreground | ((short)background<<4)), t) {}

		Pixel(Console::Color c) : Pixel(c, Type::Full) {}
		
		Pixel(Console::Color c, char character) : Pixel(c, (Type)character) {}
		Pixel(Console::Color foreground, Console::Color background, char character) : Pixel(foreground, background, (Type)character) {}
	};


private:
	HANDLE m_hConsole; // a handle to the console window
	HWND m_console;
	SMALL_RECT m_rectWindow; // the size of the screen buffer

	CHAR_INFO* m_bufScreen; // The screen buffer

	int m_width, m_height; // the size of the screen, in characters

	std::chrono::steady_clock::time_point m_timeLastDraw;

	float m_deltaDrawTime; // Delta time since last draw call, in seconds
public:

	Console(unsigned int width, unsigned int height) 
		: m_width(width), m_height(height),
		  m_rectWindow({ 0, 0, (short)width - 1, (short)height - 1 })
	{
		// Resize the console
		m_console = GetConsoleWindow();
		RECT r;
		GetWindowRect(m_console, &r); //stores the console's current dimensions
		MoveWindow(m_console, r.left, r.top, m_width * 8, m_height * 8, TRUE);

		// Create the screen buffer
		m_bufScreen = new CHAR_INFO[m_width * m_height];
		memset(m_bufScreen, 0, sizeof(CHAR_INFO) * m_width * m_height);
		m_hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleActiveScreenBuffer(m_hConsole);

		// Disable resizing
		SetWindowLong(m_console, GWL_STYLE, GetWindowLong(m_console, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

		// Set last draw time
		m_timeLastDraw = std::chrono::high_resolution_clock::now();
		m_deltaDrawTime = 0.0f;
	}

	~Console()
	{
		delete[] m_bufScreen;
	}

	inline void Clear(const Pixel& pixel)
	{
		for (int i = 0; i < m_width * m_height; i++)
		{
			m_bufScreen[i].Char.UnicodeChar = (short)pixel.type;
			m_bufScreen[i].Attributes = (short)pixel.color;
		}
	}

	inline void Draw(int x, int y, const Pixel& pixel)
	{
		if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		{
			m_bufScreen[y * m_width + x].Char.UnicodeChar = (short)pixel.type;
			m_bufScreen[y * m_width + x].Attributes = (short)pixel.color;
		}
	}

	inline void DrawString(int leftX, int topY, Color foreground, Color background, const std::string& str)
	{
		int x = leftX;
		for (int i = 0; i < str.length(); i++)
		{
			if (str[i] == '\n')
			{
				x = leftX;
				topY++;
			}
			else
			{
				Draw(x, topY, Pixel(foreground, background, str[i]));
				x++;
			}
		}
	}

	inline void BlipToScreen()
	{
		auto now = std::chrono::high_resolution_clock::now();
		m_deltaDrawTime = std::chrono::duration<float>(now - m_timeLastDraw).count();
		m_timeLastDraw = now;

		wchar_t s[256];
		swprintf_s(s, 256, L"%3.2f fps", 1.0f / m_deltaDrawTime);
		SetConsoleTitle(s);
		
		WriteConsoleOutput(m_hConsole, m_bufScreen, { (short)m_width, (short)m_height }, { 0,0 }, &m_rectWindow);
	}
	
	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
	inline float DeltaTime() const { return m_deltaDrawTime; }
};
