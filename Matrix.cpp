#include <iostream>
#include <thread> 
#include <chrono>
#include <vector>
#include <random>
#include <tuple>
#include "RainDrop.h"
#include "DoubleBufferedConsole.h"
#include <conio.h>
#include <windows.h>

using namespace std::chrono_literals;
const auto refreshRate = 0.05s;
const auto resizeChance = 0.9;

static std::random_device rd;
static std::mt19937 mt(rd());

void initRainDrops(std::vector<matrix::RainDrop>& rainDrops, int width, int height, const wchar_t* charSet, size_t charSetSize)
{
    std::uniform_int_distribution<int> disSize(0, height);
    std::uniform_int_distribution<int> disDepth(height, height + height / 2);

    for (int x = 0; x < width; ++x)
    {
        rainDrops.emplace_back(disSize(mt), x, disDepth(mt), 0.2f, charSet, charSetSize);
    }
}

void clearScreen() {
    system("cls");  // Use system cls instead of newlines
}

matrix::CharacterSet selectCharacterSet()
{
    clearScreen();
    std::cout << "Digital Rain Animation"<< "\n""====================="<<"\n";
    std::cout << "Choose a character set:\n1: Matrix\n2: Snow\n3: Diamonds\n4: Rain\nEnter your choice: ";

    int choice;
    std::cin >> choice;

    switch (choice)
    {
    case 1: return matrix::CharacterSet::Standard;
    case 2: return matrix::CharacterSet::Snow;
    case 3: return matrix::CharacterSet::Diamonds;
    case 4: return matrix::CharacterSet::Rain;
    default:
        std::cout << "Invalid choice. Please try again.\n";
        std::this_thread::sleep_for(1s);
        return selectCharacterSet();
    }
}

WORD selectRainColor()
{
    clearScreen();
    std::cout << "Digital Rain" << "\n""=====================" << "\n";
    std::cout << "Choose a colour for the rain:\n1: Green\n2: White\n3: Blue\n4: Yellow\nEnter your choice: ";

    int colourChoice;
    std::cin >> colourChoice;

    switch (colourChoice)
    {
    case 1: return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    case 2: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    case 3: return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
    case 4: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    default:
        std::cout << "Invalid choice. Defaulting to Green.\n";
        return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    }
}

bool shouldExit()
{
    if (_kbhit())
    {
        return _getch() == 'q';
    }
    return false;
}


void runRainAnimation(matrix::CharacterSet selectedSet, WORD txtAttributes)
{
    const wchar_t* selectedCharacters = matrix::getCharacterSet(selectedSet);
    size_t selectedCharacterSet = matrix::getCharacterSetSize(selectedSet);

    DoubleBufferedConsole<wchar_t> myConsole(L"Matrix");
    short width, height;
    std::tie(width, height) = myConsole.getSize();

    myConsole.clear(' ', txtAttributes);

    std::vector<matrix::RainDrop> rainDrops;
    initRainDrops(rainDrops, width, height, selectedCharacters, selectedCharacterSet);

    std::uniform_real_distribution<double> disD(0, 1);
    bool running = true;

    while (!shouldExit())  // while should exit is true, is set to false in the shouldExit func when q is pressed
    {
        for (auto& rainDrop : rainDrops)
        {
            rainDrop.fall();
            for (const auto& element : rainDrop.get())
            {
                if (element.y < height)
                {
                    myConsole.write(element.x, element.y, element.c, txtAttributes);
                }
            }

            double randValue = disD(mt);
            if (randValue <= resizeChance)
            {
                (randValue <= 0.5) ? ++rainDrop : --rainDrop;  //new and improved
            }

        }

        myConsole.flip();
        myConsole.clear(' ', txtAttributes);
        std::this_thread::sleep_for(refreshRate);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    myConsole.cleanup();
    clearScreen();
}

void startRainSimulation()
{
    while (true)
    {
        matrix::CharacterSet selectedSet = selectCharacterSet();
        WORD txtAttributes = selectRainColor();
        runRainAnimation(selectedSet, txtAttributes);
    }
}

int main()
{
    startRainSimulation();
    return 0;
}
