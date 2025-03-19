---
layout: default
---

# Digital Rain Animation
A C++ console application that simulates the iconic "Matrix" falling code effect. Using multithreading, double buffering, and randomization, the program generates cascading streams of Unicode characters that dynamically change and flow down the screen, mimicking the visual style from the Matrix movies.

![Matrix Rain Effect](docs/assets/images/2025-03-1418-05-19-ezgif.com-video-to-gif-converter.gif)
![Menu Interface](docs/assets/images/Screenshot20250308132546.png)

## Features
- **Realistic Falling Code Effect** – Uses a list-based system to manage individual character streams
- **Dynamic Character Changes** – Each raindrop element has a chance to mutate into a different symbol
- **Adjustable Rain Stream Size** – The length of each stream varies dynamically for a more organic look
- **Custom Unicode Characters** – Uses a selection of unique symbols to enhance the digital rain aesthetic
- **Multiple Character Sets** - Choose between Matrix, Snow, Diamonds, or Rain effects
- **Color Customization** - Select from Green, White, Blue, or Yellow color schemes
- **Interactive Controls** - Press 'q' to exit the animation and return to the menu

## Code Structure

### 1. Character Sets and Enums
The program defines different sets of Unicode characters for various visual effects:

```cpp
// Matrix-style characters (default)
const wchar_t unicodeCharacters[]{
    L'Ƣ',L'Ʃ',L'Ʊ',L'Ƹ',L'ǂ',L'ƽ',L'ǌ',L'Ȣ',L'Ⱦ',L'Ƚ',L'ɑ',L'ɤ',L'ʑ',
    // ... more characters ...
};

// Snow effect characters
const wchar_t unicodeCharactersSnow[]{
    L'✼',L'❋',L'❊'
};

// Rain effect characters
const wchar_t unicodeCharactersRain[]{
    L'░',L'▓'
};

// Diamond effect characters
const wchar_t unicodeCharactersDiamonds[]{
    L'⬖',L'⬗',L'⬘',L'⬙',L'◈'
};
```

The `CharacterSet` enum allows selection between different character sets:
```cpp
enum class CharacterSet {
    Standard,  // Matrix style
    Snow,      // Snow effect
    Diamonds,  // Diamond shapes
    Rain       // Rain drops
};
```

### 2. Character Set Management
Helper functions to manage character sets:

```cpp
// Returns the appropriate character set based on selection
inline const wchar_t* getCharacterSet(CharacterSet set) {
    switch (set) {
        case matrix::CharacterSet::Standard:
            return unicodeCharacters;
        case matrix::CharacterSet::Snow:
            return unicodeCharactersSnow;
        // ... other cases ...
    }
}

// Calculates the size of the selected character set
static size_t getCharacterSetSize(CharacterSet set) {
    switch (set) {
        case matrix::CharacterSet::Standard:
            return sizeof(unicodeCharacters) / sizeof(unicodeCharacters[0]);
        // ... other cases ...
    }
}
```

### 3. Random Number Generation
The program uses Mersenne Twister for high-quality random number generation:

```cpp
static std::random_device rd;  // Random device for seeding
static std::mt19937 mt{ rd() };  // Mersenne Twister generator
static std::uniform_int_distribution<size_t> disI(0, sizeof(unicodeCharacters) / sizeof(unicodeCharacters[0]));
const static size_t maxDepth = 50;  // Maximum depth for rain effect
```

### 4. RainDropElement Structure
Represents a single character in the falling effect:

```cpp
struct RainDropElement {
    const int x, y;  // Position coordinates
    wchar_t c;       // Character to display
    const wchar_t* charSet;  // Character set to use
    size_t charSetSize;     // Size of character set

    // Randomly changes the character based on probability
    inline void changeRandomly(const float chance) {
        static std::uniform_real_distribution<float> localDisF(0.0f, 1.0f);
        if (chance >= localDisF(mt)) {
            std::uniform_int_distribution<size_t> disI(0, charSetSize - 1);
            c = charSet[disI(mt)];
        }
    }

    // Constructor initializes position and character
    inline RainDropElement(const int x, const int y, const wchar_t* charSet, size_t charSetSize)
        : x(x), y(y), c(L' '), charSet(charSet), charSetSize(charSetSize) {
        changeRandomly(1);
    }
};
```

### 5. RainDrop Class
Manages a column of falling characters:

```cpp
class RainDrop {
private:
    raindrops_t rainDropElements;  // List of characters in the column
    const int maxSize;             // Maximum column length
    int size;                      // Current column length
    const int x;                   // X position
    const int maxDepthY;           // Screen height
    float changeChance{ 0.5 };     // Probability of character change
    int y{ 0 };                    // Current Y position
    const wchar_t* charSet;        // Character set to use
    size_t charSetSize;            // Size of character set

public:
    // Constructor initializes the raindrop column
    RainDrop(int maxSize, int x, int maxDepthY, float changeChance, 
            const wchar_t* charSet, size_t charSetSize)
        : maxSize(maxSize), x(x), y(0), size(maxSize / 2), 
          maxDepthY(maxDepthY), changeChance(changeChance),
          charSet(charSet), charSetSize(charSetSize) {}

    // Methods for managing the raindrop
    void fall();                   // Updates the falling animation
    const raindrops_t& get() const; // Returns the list of elements
    void operator++();             // Increases column length
    void operator--();             // Decreases column length
};
```

### 6. Double Buffered Console
The `DoubleBufferedConsole` class handles efficient screen rendering:

```cpp
template<typename char_t = wchar_t>
class DoubleBufferedConsole {
private:
    HANDLE buffer[2];              // Two screen buffers for double buffering
    SMALL_RECT writeRect;          // Screen region to update
    PCHAR_INFO charInfoBuffer;     // Character buffer
    COORD cursor, bufferSize;      // Screen dimensions
    int bufferIndex;               // Current active buffer
    short width, height;           // Console dimensions

public:
    // Constructor initializes the console
    DoubleBufferedConsole(LPCWSTR title, const short width, const short height);
    
    // Core rendering methods
    void flip();                   // Swaps buffers
    void clear(char_t filling, unsigned short attribute);  // Clears screen
    void write(int x, int y, char_t c, unsigned short attribute);  // Writes character
    std::pair<short, short> getSize() const;  // Returns dimensions
};
```

### 7. Main Program Flow
The main program handles user interaction and animation:

```cpp
int main() {
    while (true) {
        // Display menu and get user choices
        // Initialize console and raindrops
        // Run animation loop
        // Handle user input (q to quit)
        // Clean up and return to menu
    }
}
```

## Technical Details
- Uses Windows Console API for efficient screen rendering
- Implements double buffering to prevent screen flicker
- Uses Unicode characters for rich visual effects
- Employs modern C++ features for efficient memory management
- Provides smooth animation through controlled frame timing

## Building and Running
1. Compile with a C++17 or later compiler
2. Run the executable in a Windows console
3. Use the menu to select character set and color
4. Press 'q' to exit the animation and return to menu
