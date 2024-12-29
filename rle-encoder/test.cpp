/**
 * Runs functions that test key parts of the program.
 */

#include <iostream>
#include <string>
#include <sstream>

#include "input.hpp"

std::string currentTestSuite;
std::string currentTestGroup;

static void setTestSuite(std::string testSuite) {
    currentTestSuite = testSuite;
    std::cout << "Test Suite: " << currentTestSuite << std::endl;
}

static void setTestGroup(std::string testGroup) {
    currentTestGroup = testGroup;
    std::cout << "\tTest Group: " << currentTestGroup << std::endl;
}

static std::ostream& operator<<(std::ostream& os, Mode value) {
    switch (value) {
        case Mode::COMPRESS:
            os << "compress";
            break;
        case Mode::DECOMPRESS:
            os << "decompress";
            break;
        default:
            os << "unknown";
            break;
    }
    return os;
}

template<typename T>
static void assertEquals(T expected, T actual) {
    if (expected != actual) {
        std::ostringstream oss;
        oss << "Values are not equal. Expected: " << expected << ", Actual: " << actual;
        throw std::runtime_error(oss.str());
    }
}

static void assertThrows(std::function<void()> func) {
    try {
        func();
    } catch (std::runtime_error &ex) {
        return;
    }

    throw std::runtime_error("Exception was expected to be thrown but it was not thrown.");
}

static void testInput() {
    setTestSuite("Cmd input");
    
    setTestGroup("Multiple modes specified.");

    assertThrows([]() {
        parseArguments(4, (const char*[]) {"rle", "-c", "-d", "input.file"});
    });

    setTestGroup("Missing output file.");
    assertThrows([]() {
        parseArguments(3, (const char*[]) {"rle", "-c", "-o"});
    });

    setTestGroup("Missing input file.");
    assertThrows([]() {
        parseArguments(2, (const char*[]) {"rle", "-c"});
    });

    setTestGroup("Valid input, compression mode");
    ParsedArguments parsedArgs = parseArguments(3, (const char*[]) {"rle", "-c", "input.file"});
    assertEquals(parsedArgs.inputFile, std::string{"input.file"});
    assertEquals(parsedArgs.mode, Mode::COMPRESS);
    assertEquals(parsedArgs.outputFile, parsedArgs.outputFile);

    setTestGroup("Valid input, decompression mode");
    parsedArgs = parseArguments(3, (const char*[]) {"rle", "-d", "input2.file"});
    assertEquals(parsedArgs.inputFile, std::string{"input2.file"});
    assertEquals(parsedArgs.mode, Mode::DECOMPRESS);
    assertEquals(parsedArgs.outputFile, parsedArgs.outputFile);

    setTestGroup("Valid input, explicit output file");
    parsedArgs = parseArguments(5, (const char*[]) {"rle", "-d", "-o", "output.out", "input2.file"});
    assertEquals(parsedArgs.inputFile, std::string{"input2.file"});
    assertEquals(parsedArgs.mode, Mode::DECOMPRESS);
    assertEquals(parsedArgs.outputFile, std::string{"output.out"});
}

int main() {
    try {
        testInput();
    } catch (std::runtime_error &ex) {
        std::cerr << ex.what();
    }
    return 0;
}