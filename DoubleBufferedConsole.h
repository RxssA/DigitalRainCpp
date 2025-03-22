#ifndef DOUBLE_BUFFERED_CONSOLE_H
#define DOUBLE_BUFFERED_CONSOLE_H


/*
	Double buffered console renders the characters before displaying them in the terminal
	https://github.com/Kevger/DoubleBufferedWindowsConsole/blob/master/source/DoubleBufferedConsole.h
*/

#include <Windows.h>

/*
	Windows API Functions Used:
	- CreateConsoleScreenBuffer: Creates a new console screen buffer
		https://learn.microsoft.com/en-us/windows/console/createconsolescreenbuffer
	- SetConsoleScreenBufferSize: Sets the size of the specified console screen buffer
		https://learn.microsoft.com/en-us/windows/console/setconsolescreenbuffersize
	- SetConsoleWindowInfo: Sets the current size and position of a console window
		https://learn.microsoft.com/en-us/windows/console/setconsolewindowinfo
	- GetConsoleCursorInfo: Retrieves information about the console's cursor
		https://learn.microsoft.com/en-us/windows/console/getconsolecursorinfo
	- SetConsoleCursorInfo: Sets the cursor's appearance and visibility
		https://learn.microsoft.com/en-us/windows/console/setconsolecursorinfo
	- SetConsoleTitle: Sets the title for the current console window
		https://learn.microsoft.com/en-us/windows/console/setconsoletitle
	- WriteConsoleOutput: Writes character and color attribute data to a specified rectangular block of character cells in a console screen buffer
		https://learn.microsoft.com/en-us/windows/console/writeconsoleoutput
	- SetConsoleActiveScreenBuffer: Sets the specified screen buffer to be the currently displayed console screen buffer
		https://learn.microsoft.com/en-us/windows/console/setconsoleactivescreenbuffer
	- GetConsoleScreenBufferInfo: Retrieves information about the specified console screen buffer
		https://learn.microsoft.com/en-us/windows/console/getconsolescreenbufferinfo

	Windows API Data Structures Used:
	- HANDLE: A generic handle type used to reference Windows objects (like console buffers, files, etc.)
		https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types#handle
	- SMALL_RECT: A structure that defines a rectangle by its top-left and bottom-right coordinates
		https://learn.microsoft.com/en-us/windows/console/small-rect-str
	- CHAR_INFO: A structure that holds a character and its attributes (color, etc.)
		https://learn.microsoft.com/en-us/windows/console/char-info-str
	- PCHAR_INFO: A pointer to a CHAR_INFO structure
	- COORD: A structure that defines a coordinate in the console (X,Y position)
		https://learn.microsoft.com/en-us/windows/console/coord-str
*/

template<typename char_t = wchar_t>
class DoubleBufferedConsole
{
	// Array of two screen buffer handles for double buffering
	// HANDLE is a Windows type used to reference system objects (like console buffers)
	// nullptr initialization ensures handles are invalid until properly created
	// https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types#handle
	HANDLE  buffer[2]{ nullptr, nullptr }; 
	// Defines the rectangular region of the console to be updated
	// SMALL_RECT contains Left, Top, Right, Bottom coordinates defining a rectangle
	// Used to specify which portion of the console to update
	// https://learn.microsoft.com/en-us/windows/console/small-rect-str
	SMALL_RECT writeRect;                    
	// Buffer to store character and attribute data before rendering
	// PCHAR_INFO is a pointer to CHAR_INFO, which contains:
	// - Char: The character to display (can be Unicode or ASCII)
	// - Attributes: Color and style information for the character
	// https://learn.microsoft.com/en-us/windows/console/char-info-str
	PCHAR_INFO charInfoBuffer{ nullptr };  
	// Tracks cursor position and buffer dimensions
	// COORD is a structure containing X and Y coordinates
	// Used for both cursor position and buffer size specification
	// https://learn.microsoft.com/en-us/windows/console/coord-str
	COORD cursor{ 0,0 }, bufferSize;  
	// Index to track which buffer is currently active
	// Alternates between 0 and 1 for double buffering
	int bufferIndex{ 0 };  
	// Stores console window dimensions
	// Used to track the size of the console window
	short width, height;   

	// Sets up console handlers and initializes the console window
	void handlerSetup(const LPCWSTR title)
	{
		// Structure to hold cursor information
		CONSOLE_CURSOR_INFO cursorInfo;

		// Create and configure two screen buffers
		for (auto i = 0; i < 2; ++i) 
		{
			// Create a new console screen buffer with write access
			buffer[i] = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
			// Set the size of the screen buffer
			SetConsoleScreenBufferSize(buffer[i], bufferSize);
			// Configure the console window size and position
			SetConsoleWindowInfo(buffer[i], TRUE, &writeRect);

			// Check if buffer creation was successful
			if (!buffer[i])
				throw "DoubleBufferedConsole: Handler creation failed!";

			// Hide the cursor in the console
			GetConsoleCursorInfo(buffer[i], &cursorInfo);
			cursorInfo.bVisible = false;
			SetConsoleCursorInfo(buffer[i], &cursorInfo);
		}
		// Set the console window title
		SetConsoleTitle(title);
	}

public:
	// Constructor that takes explicit width and height
	DoubleBufferedConsole(LPCWSTR title, const short width, const short height)
		: writeRect{0, 0, width,height},      // Initialize console dimensions
		charInfoBuffer(new CHAR_INFO[width * height]), // Allocate character buffer
		bufferSize{ width,height },           // Set buffer size
		width(width),                         // Store width
		height(height)                        // Store height
	{
		handlerSetup(title);                  // Initialize console handlers
	}

	// Constructor that uses default console size
	DoubleBufferedConsole(const LPCWSTR title, short height = 100)
		:height(height)                       // Store height
	{
		// Get current console buffer information
		CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleBufferInfo);
		width = consoleBufferInfo.dwSize.X - 1;  // Calculate width from current console

		// Initialize buffer dimensions and character buffer
		bufferSize = { width,height };
		writeRect = { 0,0,width,height };
		charInfoBuffer = new CHAR_INFO[width * height];
		handlerSetup(title);                  // Initialize console handlers
	}

	// Add cleanup function becasue clearScreen() was not working, i was getting unable to start app after pressing Q.
	void cleanup() {
		// Close the console handles and sent them back to nullptr.
		for (auto i = 0; i < 2; ++i) {
			if (buffer[i]) {
				CloseHandle(buffer[i]);
				buffer[i] = nullptr;
			}
		}
	}

	// Destructor to clean up allocated memory
	~DoubleBufferedConsole()
	{
		cleanup();
		delete[] charInfoBuffer;              // Free character buffer memory
		charInfoBuffer = nullptr;             // Set pointer to null
	}

	// Swaps the background buffer to the foreground
	void flip()
	{
		// Write the character buffer to the next screen buffer
		WriteConsoleOutput(
			buffer[++bufferIndex & 0x01],     // Select next buffer (alternates between 0 and 1)
			charInfoBuffer,                    // Source buffer containing characters and attributes
			bufferSize,                        // Size of the buffer
			{ 0, 0 },                         // Starting position in source buffer
			&writeRect);                      // Region to update in console
		// Make the written buffer the active screen buffer
		SetConsoleActiveScreenBuffer(buffer[bufferIndex & 0x01]);
	}

	// Clears the background buffer with specified character and color
	void clear(const char_t filling, unsigned short attribute)
	{
		// Fill the entire buffer with the specified character and color
		for (auto y = 0; y < height; ++y)
		{
			for (auto x = 0; x < width; ++x)
			{
				charInfoBuffer[x + width * y].Attributes = attribute;  // Set color attributes
				// Set character based on template type (Unicode or ASCII)
				if constexpr (std::is_same<char_t, wchar_t>::value)
					charInfoBuffer[x + width * y].Char.UnicodeChar = filling;
				else
					charInfoBuffer[x + width * y].Char.AsciiChar = filling;
			}
		}
	}

	// Writes a single character with attributes to the background buffer
	void write(const int x, const int y, const char_t c, unsigned short attribute)
	{
		const auto coord = x + y * width;     // Calculate buffer position
		// Set character based on template type (Unicode or ASCII)
		if constexpr (std::is_same<char_t, wchar_t>::value)
			charInfoBuffer[x + width * y].Char.UnicodeChar = c;
		else
			charInfoBuffer[x + width * y].Char.AsciiChar = c;

		charInfoBuffer[coord].Attributes = attribute;  // Set color attributes
	}

	// Returns the current console dimensions
	std::pair<short, short> getSize() const
	{
		return { width,height };
	}
};

#endif // DOUBLE_BUFFERED_CONSOLE_H
