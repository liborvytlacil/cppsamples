/**
 * Runs functions that test key parts of the program.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>

#include "input.hpp"
#include "rle.hpp"

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
        parseArguments({"rle", "-c", "-d", "input.file"});
    });

    setTestGroup("Missing output file.");
    assertThrows([]() {
        parseArguments({"rle", "-c", "-o"});
    });

    setTestGroup("Missing input file.");
    assertThrows([]() {
        parseArguments({"rle", "-c"});
    });

    setTestGroup("Valid input, compression mode");
    ParsedArguments parsedArgs = parseArguments({"rle", "-c", "input.file"});
    assertEquals(parsedArgs.inputFile, std::string{"input.file"});
    assertEquals(parsedArgs.mode, Mode::COMPRESS);
    assertEquals(parsedArgs.outputFile, parsedArgs.outputFile);

    setTestGroup("Valid input, decompression mode");
    parsedArgs = parseArguments({"rle", "-d", "input2.file"});
    assertEquals(parsedArgs.inputFile, std::string{"input2.file"});
    assertEquals(parsedArgs.mode, Mode::DECOMPRESS);
    assertEquals(parsedArgs.outputFile, parsedArgs.outputFile);

    setTestGroup("Valid input, explicit output file");
    parsedArgs = parseArguments({"rle", "-d", "-o", "output.out", "input2.file"});
    assertEquals(parsedArgs.inputFile, std::string{"input2.file"});
    assertEquals(parsedArgs.mode, Mode::DECOMPRESS);
    assertEquals(parsedArgs.outputFile, std::string{"output.out"});
}

static void testRleEncoding() {
    setTestSuite("RLE Encoding");
    setTestGroup("Encoding single sequence.");
    std::vector<char> input {'A', 'A', 'A', 'A'};
    std::vector<Repetition> encodedInput = encode(input, 4);
    assertEquals((size_t) 1, encodedInput.size());
    assertEquals(encodedInput[0].byte, 'A');
    assertEquals((int) encodedInput[0].count, 4);

    setTestGroup("Encoding multiple sequences.");
    input = {'A', 'B', 'B', 'A', 'C', 'C', 'C', 'C', 'D'};
    encodedInput = encode(input, 9);
    assertEquals((size_t) 5, encodedInput.size());
    assertEquals(encodedInput[0].byte, 'A');
    assertEquals((int) encodedInput[0].count, 1);

    assertEquals(encodedInput[1].byte, 'B');
    assertEquals((int) encodedInput[1].count, 2);

    assertEquals(encodedInput[2].byte, 'A');
    assertEquals((int) encodedInput[2].count, 1);

    assertEquals(encodedInput[3].byte, 'C');
    assertEquals((int) encodedInput[3].count, 4);

    assertEquals(encodedInput[4].byte, 'D');
    assertEquals((int) encodedInput[4].count, 1);

    setTestGroup("Encoding only part of the input vector.");
    encodedInput = encode(input, 5);
    assertEquals((size_t) 4, encodedInput.size());

}

int main() {
    try {
        testInput();
        testRleEncoding();
        std::cout << std::endl << "TESTS SUCCESSFUL" << std::endl;
    } catch (std::runtime_error &ex) {
        std::cerr << ex.what();
    }
    return 0;
}