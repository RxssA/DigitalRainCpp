#include <iostream>
#include <thread> 
#include <chrono>
#include <vector>
#include <random>
#include <tuple>
#include "RainDrop.h"
#include "DoubleBufferedConsole.h"
#include <conio.h> 

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
    std::cout << std::string(50, '\n');  // Simulate clearing screen
}

int main()
{
    while (true)
    {
        clearScreen();
        std::cout << "Choose a character set:\n";
        std::cout << "1: Matrix\n";
        std::cout << "2: Snow\n";
        std::cout << "3: Diamonds\n";
        std::cout << "4: Rain\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        matrix::CharacterSet selectedSet;
        switch (choice)
        {
        case 1:
            selectedSet = matrix::CharacterSet::Standard;
            break;
        case 2:
            selectedSet = matrix::CharacterSet::Snow;
            break;
        case 3:
            selectedSet = matrix::CharacterSet::Diamonds;
            break;
        case 4:
            selectedSet = matrix::CharacterSet::Rain;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            std::this_thread::sleep_for(1s);
            continue;  // Restart menu loop
        }
        clearScreen();
        

        std::cout << "Choose a color for the rain:\n";
        std::cout << "1: Green\n";
        std::cout << "2: White\n";
        std::cout << "3: Blue\n";
        std::cout << "4: Yellow\n";
        std::cout << "Enter your choice: ";

        int colorChoice;
        std::cin >> colorChoice;

        WORD txtAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

        switch (colorChoice)
        {
        case 1:
            txtAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case 2:
            txtAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case 3:
            txtAttributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case 4:
            txtAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        default:
            std::cout << "Invalid choice. Defaulting to Green.\n";
            txtAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        }

        const wchar_t* selectedCharacters = matrix::getCharacterSet(selectedSet);
        size_t selectedCharacterSet = matrix::getCharacterSetSize(selectedSet);

        DoubleBufferedConsole<wchar_t> myConsole(L"Matrix");
        short width, height;
        std::tie(width, height) = myConsole.getSize();
        myConsole.clear(' ', txtAttributes);

        std::vector<matrix::RainDrop> rainDrops;
        initRainDrops(rainDrops, width, height, selectedCharacters, selectedCharacterSet);

        std::uniform_real_distribution<double> disD(0, 1);

        std::cout << "\nPress ENTER to return to the menu...\n";

        while (true)
        {
            if (_kbhit())
            {
                char key = _getch();
                if (key == 13)  // Enter key
                    break;  // Exit rain animation and return to menu
            }

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

                if (disD(mt) <= resizeChance)
                {
                    if (disD(mt) <= 0.5)
                        ++rainDrop;
                    else
                        --rainDrop;
                }
            }

            myConsole.flip();
            myConsole.clear(' ', txtAttributes);
            std::this_thread::sleep_for(refreshRate);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        myConsole.~DoubleBufferedConsole(); 
    }

    return 0;
}
