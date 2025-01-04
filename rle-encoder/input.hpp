/**
 * Handles command line argument input and parsing.
 */

#ifndef INPUT_HPP
#define INPUT_HPP

#include <string>

/**
 * Operation mode.
 */
enum class Mode {
    COMPRESS,
    DECOMPRESS
};

/**
 * Parsed input arguments.
 */
struct ParsedArguments {
    Mode mode;
    std::string inputFile;
    std::string outputFile;
};

/**
 * Represents an exception thrown during argument parsing when malformed input arguments are provided.
 */
class CmdArgumentException : public std::runtime_error { 
public:
    explicit CmdArgumentException(const std::string &message)
        : std::runtime_error(message) { }
};

/**
 * Parses the given input arguments as they appear on the command line input.
 * 
 * @args input arguments as they appear in 'char* argv[]' in the program's main function. Note that
 * the first element of the array (the first cmd argument) os the name of the executable. 
 * 
 * @throws CmdArgumentException when parsing encounters an invalid or malformed input.
 */
inline ParsedArguments parseArguments(const std::vector<std::string> &args) {
    ParsedArguments parsedArgs;
    bool modeSet = false;

    // Note: the first argument is the name of the executable, so we skip its parsing.
    for (int i = 1; i < args.size(); ++i) {
        std::string arg = args[i];

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
            if (i + 1 >= args.size()) {
                throw CmdArgumentException("Missing output file name after -o.");
            }
            parsedArgs.outputFile = args[i + 1];
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

    // default output file name
    if (parsedArgs.outputFile.empty()) {
        parsedArgs.outputFile = parsedArgs.inputFile + ".out";
    }

    return parsedArgs;
}

#endif