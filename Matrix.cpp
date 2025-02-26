
/*
    Digital Rain
	Ross Amritage
	G00401655
	16/02/2025
*/
#include <iostream>
#include <thread> // For sleep/delay 
#include <chrono>
#include <vector>
#include <random> // For generating random values
#include <tuple> // For returning multiple values
#include "RainDrop.h"
#include "DoubleBufferedConsole.h"

using namespace std::chrono_literals;  //so i can use time duration directly
const auto refreshRate = 0.05s;
const auto resizeChance = 0.9;  //length of stream

static std::random_device rd;
static std::mt19937 mt(rd());

void initRainDrops(std::vector<matrix::RainDrop>& rainDrops, int width, int height)
{
    std::uniform_int_distribution<int> disSize(0, height);  // rain drop start https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
    std::uniform_int_distribution<int> disDepth(height, height + height / 2);  // max fall

    for (int x = 0; x < width; ++x)  // loops through width of the console
    {
        /*emplace_back directly contructs the raindrop inside the vetcor, doesnt copy or move, more efficient than puch_back()
        Don't think it makes a huge difference to speed
        https://www.geeksforgeeks.org/vectoremplace_back-c-stl/ */
        rainDrops.emplace_back(disSize(mt), x, disDepth(mt), 0.2f); //random drop size, x = position, how far the drop will fall,
    }
}

int main()
{

    std::this_thread::sleep_for(std::chrono::milliseconds(16)); // delay before the chaos begins, 

    DoubleBufferedConsole<wchar_t> myConsole(L"Matrix"); // creates the double buffered conssole, Called it matrix
    const auto txtAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY; // colour green, intensifies colour.
    short width, height;
    /* std::tie unpacks multiple returb values into seperate variables, used tuple to return multiple values
    tried using std::pair as I am only return 2 values, but couldnt get it to work?*/
    std::tie(width, height) = myConsole.getSize(); 
    myConsole.clear(' ', txtAttributes);

    std::vector<matrix::RainDrop> rainDrops;
    initRainDrops(rainDrops, width, height);

    std::uniform_real_distribution<double> disD(0, 1);

    while (true)
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

    return 0;
}
