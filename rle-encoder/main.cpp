/**
 * RLE-encoder
 */

#include <iostream>
#include <string>

enum class Mode {
    COMPRESS,
    DECOMPRESS
};

struct ParsedArguments {
    Mode mode;
    std::string inputFile;
    std::string outputFile;
};

class CmdArgumentException : public std::runtime_error { 
public:
    explicit CmdArgumentException(const std::string &message)
        : std::runtime_error(message) { }
};

ParsedArguments parseArguments(int argc, char* argv[]) {
    ParsedArguments parsedArgs;
    bool modeSet = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-c") {
            if (modeSet) {
                throw CmdArgumentException("Multiple operation modes specified. Use either -c or -d.");
            }
            parsedArgs.mode = Mode::COMPRESS;
            modeSet = true;
        } else if (arg == "-d") {
            if (modeSet) {
                throw CmdArgumentException("Multiple operation modes specified. Use either -c or -d.");
            }
            parsedArgs.mode = Mode::DECOMPRESS;
            modeSet = true;
        } else if (arg == "-o") {
            if (i + 1 >= argc) {
                throw CmdArgumentException("Missing output file name after -o.");
            }
            parsedArgs.outputFile = argv[i + 1];
            ++i;
        } else if (parsedArgs.inputFile.empty()) {
            parsedArgs.inputFile = arg;
        } else {
            throw CmdArgumentException("Unexpected argument: " + arg);
        }
    }

    if (!modeSet) {
        throw CmdArgumentException("Operation mode not specified. Use either -c or -d.");
    }

    if (parsedArgs.inputFile.empty()) {
        throw CmdArgumentException("Input file not specified.");
    }

    return parsedArgs;

}

int main(int argc, char* argv[]) {
    using namespace std;

    try {
        parseArguments(argc, argv);
    } catch (const CmdArgumentException &ex) {
        cerr << ex.what() << endl;
        cout << "Usage: " << argv[0] << " -c|-d [-o outputfile] inputfile" << endl;
        return 1;
    }

    return 0;
}

void testArgumentParsing() {
    // missing mode
    // multiple modes specified
    // missing input file
    
}