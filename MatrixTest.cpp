#include "matrixTest.h"
#include "RainDrop.h"
#include <cassert>
#include <iostream>

// Simplified test functions
void testRainDropBasics() {
    matrix::RainDrop drop(10, 5, 20, 0.5f, L"AB", 2);

    // Initial state
    assert(drop.get().empty() && "New drop should be empty");

    // Basic falling
    drop.fall();
    assert(drop.get().size() == 1 && "Should add first element");

    std::cout << "testRainDropBasics: OK\n";
}

void testRainDropLimits() {
    matrix::RainDrop drop(5, 0, 10, 0.5f, L"X", 1);

    // Test max size
    for (int i = 0; i < 10; i++) {
        drop.fall();
        ++drop;  // Increase size
    }
    assert(drop.get().size() <= 5 && "Should respect max size");

    std::cout << "testRainDropLimits: OK\n";
}

void testCharacterSets() {
    using namespace matrix;

    // Test default set
    assert(getCharacterSet(CharacterSet::Standard)[0] != L' ' && "Default chars loaded");
    assert(getCharacterSetSize(CharacterSet::Snow) == 3 && "Snow set size correct");

    std::cout << "testCharacterSets: OK\n";
}

void runAllTests() {
    testRainDropBasics();
    testRainDropLimits();
    testCharacterSets();

    std::cout << "\nAll essential tests passed!\n";
}