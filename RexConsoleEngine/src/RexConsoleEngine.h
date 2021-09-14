#pragma once
#include <Shlobj.h>
#if _WIN32_WINNT != 0x0500
	#define _WIN32_WINNT 0x0500
#endif
#include <windows.h>

#include <atomic>
#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <random>

#define Error(a) PrintError(a, __LINE__) // is undef at the end of the file

namespace RexConsoleEngine
{
	// Convert string to wstring
	inline static std::wstring StringToWString(const std::string& str)
	{
		int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
		wchar_t* wstr = new wchar_t[count]; // + 1 because of null termination
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, count);
		std::wstring wString(wstr);
		delete[] wstr;
		return wString;
	}


	/// <summary>
	/// Class to handle input and output operations with the console
	/// </summary>
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

		// Graphics
		HANDLE m_hPreviousConsole; // Handle to the initial console buffer
		HANDLE m_hConsole; // Handle to the new console buffer (the one used)
		HWND m_console; // Window index (actual window, not console)

		CHAR_INFO* m_bufScreen; // The screen buffer
		SMALL_RECT m_displaySize; // Size of the display area, used by WriteConsoleOutput()

		int m_width, m_height; // the size of the screen, in characters
		std::wstring m_title; // The title set by the user


		// Inputs 
		static const int KeyCount = 254; // max index of the Key enum
		HANDLE m_hConsoleIn; // Handle to the input buffer
		KeyData* m_keys; // Key data
		int m_mouseDeltaX, m_mouseDeltaY, m_mouseX, m_mouseY;
		int m_scrollDelta;


		// Time
		std::chrono::steady_clock::time_point m_timeLastDraw; // Time of the last draw call (used for m_deltaDrawTime)
		float m_deltaDrawTime; // Delta time since last draw call, in seconds

		// Close events
		static std::atomic_bool m_shouldClose;
		static std::mutex m_closeMutex;
		static std::condition_variable m_closeCall;

	public:

		Console(unsigned int width, unsigned int height, const std::string& title)
			: m_width(width), m_height(height),
			m_displaySize({ 0, 0, (SHORT)width - 1, (SHORT)height - 1 }),
			m_mouseDeltaX(0), m_mouseDeltaY(0), m_scrollDelta(0)
		{
			// Convert the title from string to wstring
			m_title = StringToWString(title);

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
			SMALL_RECT const minimalWindow = { 0, 0, 1, 1 };
			if (!SetConsoleWindowInfo(m_hConsole, TRUE, &minimalWindow))
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

			// Set the final size
			SMALL_RECT const rectWindow = { 0, 0, (SHORT)m_width - 1, (SHORT)m_height - 1 };
			if (!SetConsoleWindowInfo(m_hConsole, TRUE, &rectWindow))
				Error("Could not set the window info");

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

			// Wait for the close thread to finish
			m_closeMutex.lock();
			m_closeMutex.unlock();
		}

		// Width of the console, in characters
		int Width() const { return m_width; }
		// Height of the console, in characters
		int Height() const { return m_height; }
		// Time since the last BlipToScreen() call, in seconds
		float DeltaTime() const { return m_deltaDrawTime; }

		// Set the title of the window
		void SetTitle(const std::string& title) { m_title = StringToWString(title); }

		// Should the app close ? (ex : close button was pressed)
		bool ShouldClose() const { return m_shouldClose.load(); }



		/* ----- Graphics ------ */

		// Fill the buffer with a pixel
		void Clear(const Pixel& pixel)
		{
			for (int i = 0; i < m_width * m_height; i++)
			{
				m_bufScreen[i].Char.UnicodeChar = (short)pixel.type;
				m_bufScreen[i].Attributes = (short)pixel.color;
			}
		}

		// Set a pixel at x,y
		void Draw(int x, int y, const Pixel& pixel)
		{
			if (x >= 0 && x < m_width && y >= 0 && y < m_height)
			{
				m_bufScreen[y * m_width + x].Char.UnicodeChar = (short)pixel.type;
				m_bufScreen[y * m_width + x].Attributes = (short)pixel.color;
			}
		}

		// Draw a string using the top-left position
		void DrawString(int leftX, int topY, Color foreground, Color background, const std::string& str)
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

		// Fill the area formed by x,y and width,height using the pixel
		void Fill(int x, int y, int width, int height, const Pixel& pixel)
		{
			for (int xPos = x; xPos < x + width; xPos++)
			{
				for (int yPos = y; yPos < y + height; yPos++)
					Draw(xPos, yPos, pixel);
			}
		}

		// Print the buffer to screen
		void BlipToScreen()
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
		void PollInputs()
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
		bool IsPressed(Key key) const { return m_keys[(int)key].isDown; }
		// Was the key pressed since the last call to PollInputs() ?
		bool WasJustPressed(Key key) const { return m_keys[(int)key].justDown; }
		// Was the key released since the last call to PollInputs() ?
		bool WasJustReleased(Key key) const { return m_keys[(int)key].justUp; }

		// Distance traveled by the mouse since the last call to PollInputs()
		int MouseDeltaX() const { return m_mouseDeltaX; }
		// Distance traveled by the mouse since the last call to PollInputs()
		int MouseDeltaY() const { return m_mouseDeltaY; }
		// Position of the mouse on the screen, in charaters
		int MouseX() const { return m_mouseX; }
		// Position of the mouse on the screen, in charaters
		int MouseY() const { return m_mouseY; }

		// If > 0 the user scrolled up, if < 0 the user scrolled down
		int ScrollDelta() const { return m_scrollDelta; }

	private:
		// Swap a and b
		template<class T>
		static void Swap(T& a, T& b)
		{
			T temp = a;
			a = b;
			b = temp;
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
				m_shouldClose.exchange(true);

				m_closeCall.wait(lock);
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
	inline std::atomic_bool Console::m_shouldClose(false);
	inline std::mutex Console::m_closeMutex;
	inline std::condition_variable Console::m_closeCall;



	/// <summary>
	/// <para> A derivable class to allow objects to be serialized and deserialized. </para>
	/// <para> The Pop() and Push() functions work from the same starting point : you need to pop in the same order you pushed </para>
	/// <para> The output format is : "arg1,arg2,arg3,..." with a trailing comma. </para>
	/// </summary>
	class UserData
	{
	private:
		std::deque<std::string> m_data;

	public:
		// Convert the object to a string
		bool ToString(std::string& out) 
		{
			m_data.clear(); // clear the data
			bool success = Serialize();

			// Copy the data to the output string
			out = ""; 
			for (auto& str : m_data)
				out += str + ",";

			return success;
		}

		// Update this object using a string
		bool FromString(const std::string& in)
		{
			m_data.clear(); // Clear the data

			// Split the input
			std::string temp;
			for (const char& c : in)
			{
				if (c == ',')
				{
					m_data.push_back(temp);
					temp = "";
				}
				else
					temp += c;
			}

			return Deserialize();
		}

	protected:
		virtual bool Serialize() = 0; // Use Push() to serialize the object
		virtual bool Deserialize() = 0; // Use Pop() to form the object

		// Push a new argument
		void Push(const std::string& value)
		{
			m_data.push_back(value);
		}

		// Pop the next argument
		std::string Pop()
		{
			if (m_data.size() == 0)
				return "";
			std::string str = m_data.front(); // Save the string
			m_data.pop_front(); // Remove it from the vector
			return str;
		}
	};

	// Int UserData type
	class IntData : public UserData
	{
	public:
		int value;

		IntData(int v) : value (v){}

	protected:
		bool Serialize() override
		{
			Push(std::to_string(value));
			return true;
		}

		bool Deserialize() override
		{
			value = std::stoi(Pop());
			return true;
		}
	};

	// Float UserData type
	class FloatData : public UserData
	{
	public:
		float value;

		FloatData(float v) : value(v) {}
	protected:
		bool Serialize() override
		{
			Push(std::to_string(value));
			return true;
		}

		bool Deserialize() override
		{
			value = std::stof(Pop());
			return true;
		}
	};
	
	// String UserData type
	class StringData : public UserData
	{
	public:
		std::string value;

		StringData(std::string v) : value(v) {}
	protected:
		bool Serialize() override
		{
			Push(value);
			return true;
		}

		bool Deserialize() override
		{
			value = Pop();
			return true;
		}
	};

	/// <summary>
	/// <para> A very primitive way to store and load user data. The data is stored in a (key,value) pair. </para>
	/// <para> Both the key and the value are string and should not contain newlines (\n) or commas (,): See UserData::SanitizeString. </para>
	/// <para> The output file will contain the data a the key=value format. </para>
	/// </summary>
	class Archive
	{
	public:
		// The file extention used to create and access the files
		static std::string m_fileExtension; // ".userdata"
	private:
		std::wstring m_filePath; // Path to the file to use

		std::map<std::string, std::string> m_cache; // Cached values

		constexpr static char KeyValueSeparator = '='; // Separator used in the output file

	public:
		Archive(const std::string& appName, const std::string& fileName)
		{
			// Get the file path and create the folder if needed
			wchar_t* appDataPath = 0;
			SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &appDataPath);

			m_filePath = appDataPath;
			m_filePath += L"\\RexConsoleEngine";
			CreateDirectory(m_filePath.c_str(), NULL); // Make the RexGameEngine folder (if it does not exist)
			m_filePath += L"\\" + StringToWString(appName);
			CreateDirectory(m_filePath.c_str(), NULL); // Make the app specific folder (if it does not exist)
			m_filePath += L"\\" + StringToWString(fileName) + StringToWString(m_fileExtension);
			CoTaskMemFree(static_cast<void*>(appDataPath)); // Free the memory used by GetKnownFolderPath

			GetCache();
		}

		// Get the value at the key, returns true for success
		bool Get(const std::string& key, UserData& outValue)
		{
			auto iterator = m_cache.find(key);
			if (iterator == m_cache.end()) // Not found in cache, reload it
			{
				GetCache();
				iterator = m_cache.find(key);
			}

			if (iterator == m_cache.end()) // Not found again
				return false;

			return outValue.FromString(iterator->second);
		}

		// Get all of the data in the Archive as strings
		std::map<std::string, std::string> GetAll(bool reloadCache = true)
		{
			if (reloadCache)
				GetCache();
			return m_cache;
		}

		// Returns true for success, the key and value are sanitized
		bool Set(std::string& key, UserData& value)
		{
			std::string strValue;
			if (!value.ToString(strValue))
				return false;

			SanitizeString(key);
			SanitizeString(strValue);

			GetCache(); // Update the cache before any save operations


			if (m_cache.find(key) == m_cache.end()) // Not in cache, so also not in the file
			{
				std::ofstream file(m_filePath, std::ios::app);
				if (!file.is_open())
					return false;

				m_cache.insert({ key, strValue }); // Keep the cache updated
				file.seekp(file.end); // Add to the end of the file
				file << key << KeyValueSeparator << strValue << std::endl;
				file.close();
			}
			else if (strValue != m_cache[key]) // Not in the cache, write the file from scratch
			{
				// Copy the file
				const std::wstring tempPath = m_filePath + L".temp";
				std::ifstream src(m_filePath);
				std::ofstream dst(tempPath);
				if (!src.is_open() || !dst.is_open())
					return false;

				dst << src.rdbuf();
				src.close();
				dst.close();

				// Copy the data back, but change the value
				std::ifstream temp(tempPath);
				std::ofstream file(m_filePath, std::ios::trunc);
				if (!temp.is_open() || !file.is_open())
					return false;

				std::string line;
				while (getline(temp, line))
				{
					auto foundPos = line.find(key);
					if (foundPos == 0) // If the key was found at the start of the line (so it's not a value)
					{
						file << key << KeyValueSeparator << strValue << std::endl;
					}
					else
						file << line << std::endl;
				}

				temp.close();
				file.close();

				// Delete the temp file
				DeleteFile(tempPath.c_str());
			} // else : the new value was the same as the old

			return true;
		}

		static void SanitizeString(std::string& str)
		{
			// Change \n to spaces and , to .
			for (int i = 0; i < str.length(); i++)
			{
				if (str[i] == '\n')
					str[i] = ' ';
				else if (str[i] == ',')
					str[i] = '.';
			}
		}

	private:

		void GetCache()
		{
			std::ifstream file(m_filePath);

			if (!file.is_open())
				return;

			m_cache.clear();

			for (std::string line; getline(file, line); )
			{
				std::size_t splitPos = line.find(KeyValueSeparator);
				if (splitPos != std::string::npos)
				{
					// Dont include spaces either side of the =
					std::string key = line.substr(0, line[splitPos - 1] == ' ' ? splitPos - 1 : splitPos);
					std::string value = line.substr(line[splitPos + 1] == ' ' ? splitPos + 2 : splitPos + 1);
					if (!key.empty() && !value.empty())
						m_cache.insert({ key, value });
				}
			}
			file.close();
		}
	};
	inline std::string Archive::m_fileExtension(".userdata");



	/// <summary>
	/// A simple random number generator, a new random seed is automatically set at application launch
	/// </summary>
	class Random
	{
	private:
		static std::mt19937 m_randEngine;

	public:
		static void Seed(int seed)
		{
			m_randEngine.seed(seed);
		}

		static float Get(float min, float max)
		{
			std::uniform_real_distribution<float> dist(min, std::nextafter(max, DBL_MAX));
			return dist(m_randEngine);
		}

		static int Get(int min, int max)
		{
			std::uniform_int_distribution<int> dist(min, std::nextafter(max, DBL_MAX));
			return dist(m_randEngine);
		}
	};
	inline std::mt19937 Random::m_randEngine = [] 
	{
		// Seed the random engine
		std::random_device rd;
		Seed(rd());
		return m_randEngine;
	}();

}

#undef Error(a)