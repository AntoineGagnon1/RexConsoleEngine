#pragma once
#define _WIN32_WINNT 0x0500
#include <windows.h>

#include <iostream>
#include <chrono>
#include <atomic>
#include <mutex>

#define Error(a) PrintError(a, __LINE__) // is undef at the end of the file

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
		Pixel(Console::Color foreground, Console::Color background, Type t) : Pixel((Console::Color)((short)foreground | ((short)background << 4)), t) {}

		Pixel(Console::Color c) : Pixel(c, Type::Full) {}

		Pixel(Console::Color c, char character) : Pixel(c, (Type)character) {}
		Pixel(Console::Color foreground, Console::Color background, char character) : Pixel(foreground, background, (Type)character) {}
	};

	enum class Key
	{
		MouseLeft = VK_LBUTTON,
		MouseRight = VK_RBUTTON,
		MouseMiddle = VK_MBUTTON,
		MouseBackward = VK_XBUTTON1,
		MouseForward = VK_XBUTTON2,
		Backspace = VK_BACK,
		Tab = VK_TAB,
		Enter = VK_RETURN,
		Shift = VK_SHIFT,
		Control = VK_CONTROL,
		Alt = VK_MENU,
		Pause = VK_PAUSE,
		CapsLock = VK_CAPITAL,
		Escape = VK_ESCAPE,
		Space = VK_SPACE,
		PageUp = VK_PRIOR,
		PageDown = VK_NEXT,
		End = VK_END,
		Home = VK_HOME,
		Left = VK_LEFT,
		Up = VK_UP,
		Right = VK_RIGHT,
		Down = VK_DOWN,
		Print = VK_PRINT,
		PrintScreen = VK_SNAPSHOT,
		Insert = VK_INSERT,
		Delete = VK_DELETE,
		Alpha0 = 0x30,
		Alpha1 = 0x31,
		Alpha2 = 0x32,
		Alpha3 = 0x33,
		Alpha4 = 0x34,
		Alpha5 = 0x35,
		Alpha6 = 0x36,
		Alpha7 = 0x37,
		Alpha8 = 0x38,
		Alpha9 = 0x39,
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,
		Numpad0 = VK_NUMPAD0,
		Numpad1 = VK_NUMPAD1,
		Numpad2 = VK_NUMPAD2,
		Numpad3 = VK_NUMPAD3,
		Numpad4 = VK_NUMPAD4,
		Numpad5 = VK_NUMPAD5,
		Numpad6 = VK_NUMPAD6,
		Numpad7 = VK_NUMPAD7,
		Numpad8 = VK_NUMPAD8,
		Numpad9 = VK_NUMPAD9,
		NumpadMultiply = VK_MULTIPLY,
		NumpadAdd = VK_ADD,
		NumpadSubtract = VK_SUBTRACT,
		NumpadDecimal = VK_DECIMAL,
		NumpadDivide = VK_DIVIDE,
		F1 = VK_F1,
		F2 = VK_F2,
		F3 = VK_F3,
		F4 = VK_F4,
		F5 = VK_F5,
		F6 = VK_F6,
		F7 = VK_F7,
		F8 = VK_F8,
		F9 = VK_F9,
		F10 = VK_F10,
		F11 = VK_F11,
		F12 = VK_F12,
		NumLock = VK_NUMLOCK,
		ScrollLock = VK_SCROLL,
		ShiftLeft = VK_LSHIFT,
		ShiftRight = VK_RSHIFT,
		ControlLeft = VK_LCONTROL,
		ControlRight = VK_RCONTROL,
		AltLeft = VK_LMENU,
		AltRight = VK_RMENU,
		Semicolon = VK_OEM_1,
		Plus = VK_OEM_PLUS,
		Comma = VK_OEM_COMMA,
		Minus = VK_OEM_MINUS,
		Period = VK_OEM_PERIOD,
		Slash = VK_OEM_2,
		Tilde = VK_OEM_3,
		LeftBrace = VK_OEM_4,
		Backslash = VK_OEM_5,
		RightBrace = VK_OEM_6,
		Quote = VK_OEM_7,
		AngleBracket = VK_OEM_102
	};

private:
	struct KeyData
	{
		bool isDown = false;
		bool justDown = false;
		bool justUp = false;
	};

	HANDLE m_hPreviousConsole; // Handle to the initial console buffer
	HANDLE m_hConsole; // Handle to the new console buffer (the one used)
	HANDLE m_hConsoleIn; // Handle to the input buffer
	HWND m_console; // Window index (actual window, not console)

	CHAR_INFO* m_bufScreen; // The screen buffer
	SMALL_RECT m_displaySize; // Size of the display area, used by WriteConsoleOutput()

	int m_width, m_height; // the size of the screen, in characters
	std::wstring m_title; // The title set by the user

	std::chrono::steady_clock::time_point m_timeLastDraw; // Time of the last draw call (used for m_deltaDrawTime)
	float m_deltaDrawTime; // Delta time since last draw call, in seconds

	// Inputs
	static const int KeyCount = 254; // max index of the Key enum
	KeyData* m_keys; // Key data
	int m_mouseDeltaX, m_mouseDeltaY, m_mouseX, m_mouseY;
	int m_scrollDelta;

	// Close events
	static std::atomic_bool m_shouldClose;
	static std::mutex m_closeMutex;
	static std::condition_variable m_closeCall;
public:

	Console(unsigned int width, unsigned int height, const std::wstring& title) 
		: m_width(width), m_height(height),
		  m_displaySize({0, 0, (SHORT)width - 1, (SHORT)height - 1}),
		  m_mouseDeltaX(0), m_mouseDeltaY(0), m_scrollDelta(0),
		  m_title(title)
	{
		// Create the key array and the screen buffer
		m_keys = new KeyData[KeyCount];
		m_bufScreen = new CHAR_INFO[m_width * m_height];

		// Get the window HWND
		m_console = GetConsoleWindow();
		if (m_console == NULL)
			Error("Could not get the window HWND");

		// Save the console handle for the destructor
		m_hPreviousConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (m_hPreviousConsole == INVALID_HANDLE_VALUE)
			Error("Could not get the output handle");


		// Create the new output buffer
		memset(m_bufScreen, 0, sizeof(CHAR_INFO) * m_width * m_height);
		m_hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		if (m_hConsole == INVALID_HANDLE_VALUE)
			Error("Could not create the new output buffer");

		if (!SetConsoleActiveScreenBuffer(m_hConsole))
			Error("Could not set the screen buffer");
		
		// Hack to be able to reduce the buffer size :
		SMALL_RECT const minimal_window = { 0, 0, 1, 1 };
		if (!SetConsoleWindowInfo(m_hConsole, TRUE, &minimal_window))
			Error("Could not resize the window");

		if (!SetConsoleScreenBufferSize(m_hConsole, { (SHORT)m_width, (SHORT)m_height }))
			Error("Could not set the size of the screen buffer");

		// Set font to 8x8 Terminal (Raster fonts) -> after resizing
		CONSOLE_FONT_INFOEX font;
		font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		font.nFont = 0;
		font.dwFontSize = { 8,8 };
		font.FontFamily = FF_DONTCARE;
		font.FontWeight = FW_NORMAL;
		wcscpy_s(font.FaceName, L"Terminal");
		if (!SetCurrentConsoleFontEx(m_hConsole, FALSE, &font))
			Error("Could not set the font");

		// Prevent user from resizing
		if (!SetWindowLong(m_console, GWL_STYLE, GetWindowLong(m_console, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX))
			Error("Could not set the window parameters");

		ClientResize(m_width * 8, m_height * 8); // Resize after disabling resizing else it wont work properly

		// Init last draw time
		m_timeLastDraw = std::chrono::high_resolution_clock::now();
		m_deltaDrawTime = 0.0f;

		// Inputs
		m_hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
		if (m_hConsoleIn == INVALID_HANDLE_VALUE)
			Error("Could not get the input handle");
		if (!SetConsoleMode(m_hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT)) // ENABLE_EXTENDED_FLAGS = no text selection with the mouse
			Error("Could not set the console mode");

		// Disable the cursor
		CONSOLE_CURSOR_INFO cursorInfo;
		cursorInfo.dwSize = 1;
		cursorInfo.bVisible = false;
		if (!SetConsoleCursorInfo(m_hConsole, &cursorInfo))
			Error("Could not remove the cursor");


		// Set the close button handler
		if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE))
			Error("Could not set the close handler");
	}

	~Console()
	{
		if (!SetConsoleActiveScreenBuffer(m_hPreviousConsole))// Switch back to the old buffer
			Error("Could not set the screen buffer");
		if (!CloseHandle(m_hConsole)) // Close the new buffer that was opened 
			Error("Could not delete the screen buffer");
		delete[] m_bufScreen;
		delete[] m_keys;
		m_closeCall.notify_all(); // Tell the close handler that it can close (if it was called)
		Sleep(10); // Prevent the mutex from being destroyed while in use
	}

	// Width of the console, in characters
	inline int Width() const { return m_width; }
	// Height of the console, in characters
	inline int Height() const { return m_height; }
	// Time since the last BlipToScreen() call, in seconds
	inline float DeltaTime() const { return m_deltaDrawTime; }

	// Set the title of the window
	inline void SetTitle(const std::wstring& title) { m_title = title; }

	// Should the app close ? (ex : close button was pressed)
	inline bool ShouldClose() const { return m_shouldClose.load(); }


	/* ----- Graphics ------ */

	// Fill the buffer with a pixel
	inline void Clear(const Pixel& pixel)
	{
		for (int i = 0; i < m_width * m_height; i++)
		{
			m_bufScreen[i].Char.UnicodeChar = (short)pixel.type;
			m_bufScreen[i].Attributes = (short)pixel.color;
		}
	}

	// Set a pixel at x,y
	inline void Draw(int x, int y, const Pixel& pixel)
	{
		if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		{
			m_bufScreen[y * m_width + x].Char.UnicodeChar = (short)pixel.type;
			m_bufScreen[y * m_width + x].Attributes = (short)pixel.color;
		}
	}

	// Draw a string using the top-left position
	inline void DrawString(int leftX, int topY, Color foreground, Color background, const std::wstring& str)
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

	// Draw a line from (x1,y1) to (x2,y2)
	void DrawLine(int x1, int y1, int x2, int y2, const Pixel& pixel) // Very bad and slow, uses the y = mx+b and x = my+b equations to draw
	{
		if (abs(x2 - x1) > abs(y2 - y1)) // dx > dy then plot along the x axis (this is to reduce/remove gaps)
		{
			if (x1 > x2) // Swap both points if x1 > x2
			{
				Swap<int>(x1, x2);
				Swap<int>(y1, y2);
			}

			float m = (float)(y2 - y1) / (float)(x2 - x1);
			float b = y1 - (m * x1);

			for (int x = x1; x <= x2; x++)
				Draw(x, roundf(m * x + b), pixel);
		}
		else // plot along the y axis
		{
			if (y1 > y2) // Swap both points if y1 > y2
			{
				Swap<int>(x1, x2);
				Swap<int>(y1, y2);
			}

			float m = (float)(x2 - x1) / (float)(y2 - y1);
			float b = x1 - (m * y1);

			for (int y = y1; y <= y2; y++)
				Draw(roundf(m * y + b), y, pixel);
		}
	}

	// Print the buffer to screen
	inline void BlipToScreen()
	{
		// Delta time
		auto now = std::chrono::high_resolution_clock::now();
		m_deltaDrawTime = std::chrono::duration<float>(now - m_timeLastDraw).count();
		m_timeLastDraw = now;

		// Title - fps
		wchar_t s[256];
		swprintf_s(s, 256, L"%s - %d fps", m_title.c_str(), (int)(1.0f / m_deltaDrawTime));
		if (!SetConsoleTitle(s))
			Error("Could not set the title");

		// Blip tot screen
		if (!WriteConsoleOutput(m_hConsole, m_bufScreen, { (short)m_width, (short)m_height }, { 0,0 }, &m_displaySize))
			Error("Could not write to the output buffer");
	}




	/* ----- Inputs ----- */

	// Check for new inputs
	inline void PollInputs()
	{
		m_scrollDelta = 0;

		// Catch console events
		int mx = m_mouseX, my = m_mouseY;
		INPUT_RECORD inBuf[32];
		DWORD events = 0;
		if (!GetNumberOfConsoleInputEvents(m_hConsoleIn, &events))
			Error("Could not get the number of input events");

		if (events > 0)
		{
			if (!ReadConsoleInput(m_hConsoleIn, inBuf, events, &events))
				Error("Could not read the input events");
		}

		for (DWORD i = 0; i < events; i++)
		{
			if (inBuf[i].EventType == MOUSE_EVENT) // Mouse
			{
				switch (inBuf[i].Event.MouseEvent.dwEventFlags)
				{
				case MOUSE_MOVED: // Mouse movements
					m_mouseX = inBuf[i].Event.MouseEvent.dwMousePosition.X;
					m_mouseY = inBuf[i].Event.MouseEvent.dwMousePosition.Y;
					break;
				case MOUSE_WHEELED: // Scroll wheel
					m_scrollDelta = (int)inBuf[i].Event.MouseEvent.dwButtonState >> 16;
					break;
				case 0: // Mouse click
					UpdateKey((int)Key::MouseLeft, inBuf[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED);
					UpdateKey((int)Key::MouseMiddle, inBuf[i].Event.MouseEvent.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED);
					UpdateKey((int)Key::MouseRight, inBuf[i].Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED);
					// Mouse forward and backward are not creating an event ?
					break;
				default:
					break;
				}
			}
			else if (inBuf[i].EventType == KEY_EVENT) // Keyboard
			{
				UpdateKey(inBuf[i].Event.KeyEvent.wVirtualKeyCode, inBuf[i].Event.KeyEvent.bKeyDown);
			}
		}

		// Fix for mouse forward and backward not generating events :
		UpdateKey((int)Key::MouseForward, (GetAsyncKeyState((int)Key::MouseForward) & 0x8000));
		UpdateKey((int)Key::MouseBackward, (GetAsyncKeyState((int)Key::MouseBackward) & 0x8000));

		m_mouseDeltaX = m_mouseX - mx;
		m_mouseDeltaY = m_mouseY - my;
	}

	// Is the key pressed now ?
	inline bool IsPressed(Key key) const { return m_keys[(int)key].isDown; }
	// Was the key pressed since the last call to PollInputs() ?
	inline bool WasJustPressed(Key key) const { return m_keys[(int)key].justDown; }
	// Was the key released since the last call to PollInputs() ?
	inline bool WasJustReleased(Key key) const { return m_keys[(int)key].justUp; }

	// Distance traveled by the mouse since the last call to PollInputs()
	inline int MouseDeltaX() const { return m_mouseDeltaX; }
	// Distance traveled by the mouse since the last call to PollInputs()
	inline int MouseDeltaY() const { return m_mouseDeltaY; }
	// Position of the mouse on the screen, in charaters
	inline int MouseX() const { return m_mouseX; }
	// Position of the mouse on the screen, in charaters
	inline int MouseY() const { return m_mouseY; }

	// If > 0 the user scrolled up, if < 0 the user scrolled down
	inline int ScrollDelta() const { return m_scrollDelta; }

private:
	// Swap a and b
	template<class T>
	static void Swap(T& a, T& b)
	{
		T temp = a;
		a = b;
		b = temp;
	}

	// Resize the usable part of the console
	void ClientResize(int nWidth, int nHeight) const
	{
		RECT rcClient, rcWind;
		POINT ptDiff;
		GetClientRect(m_console, &rcClient);
		GetWindowRect(m_console, &rcWind);
		ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
		ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
		MoveWindow(m_console, rcWind.left, rcWind.top, nWidth + ptDiff.x, nHeight + ptDiff.y, TRUE);
	}

	// Update the key using the new value
	void UpdateKey(int keycode, bool down)
	{
		m_keys[keycode].justDown = !m_keys[keycode].isDown && down; // Was not already down, and is down not
		m_keys[keycode].justUp = m_keys[keycode].isDown && !down; // Was down and now is up
		m_keys[keycode].isDown = down;
	}

	// Handles the close button
	static BOOL CloseHandler(DWORD event)
	{
		if (event == CTRL_CLOSE_EVENT)
		{
			std::unique_lock<std::mutex> lock(m_closeMutex);
			std::chrono::seconds duration(5); // timeout after 5 seconds
			m_shouldClose.exchange(true);
			
			m_closeCall.wait_for(lock, duration, [] {return false; });
			return TRUE;
		}

		return FALSE; // Pass the event to the next handler
	}

	// Report an error
	void PrintError(const std::string& str, int line) 
	{
		m_shouldClose.exchange(true); // Tell the user to close
		SetConsoleActiveScreenBuffer(m_hPreviousConsole); // Switch back to the old buffer
		std::cout << "[Error] (RexConsoleEngine, line : " << line << ") : " << str << std::endl; // Print the error
		std::cin.get(); // Wait for user feedback
	}
};

// Define static vars
std::atomic_bool Console::m_shouldClose(false);
std::mutex Console::m_closeMutex;
std::condition_variable Console::m_closeCall;

#undef Error(a)