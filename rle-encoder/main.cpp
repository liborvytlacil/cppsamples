/**
 * RLE-encoder
 */

#include <iostream>
#include <string>
#include <fstream>
#include "input.hpp"
#include "rle.hpp"


/**
 * Entry method that calls particualr algorithm based on parsed arguments.
 * 
 * @param args parsed arguments
 */
inline void perform(const ParsedArguments &args) {
    std::ifstream ifs {args.inputFile};
    std::ofstream ofs {args.outputFile};
    
    if (!ifs) {
        throw RLEException {"Unable to open the input file."};
    }

    if (!ofs) {
        throw RLEException {"Unable to open the output file."};
    }

    if (args.mode == Mode::COMPRESS) {
        performEncoding(ifs, ofs);
    } else if (args.mode == Mode::DECOMPRESS) {
        performDecoding(ifs, ofs);
    }
}

int main(int argc, char* argv[]) {
    using namespace std;

    try {
        vector<string> args;
        for (int i = 0; i < argc; ++i) {
            args.push_back(string {argv[i]});
        }

        ParsedArguments parsedArguments = parseArguments(args);
        perform(parsedArguments);
    } catch (const CmdArgumentException &ex) {
        cerr << ex.what() << endl;
        cout << "Usage: " << argv[0] << " -c|-d [-o outputfile] inputfile" << endl;
        return 1;
    } catch (const RLEException &ex) {
        cerr << ex.what() << endl;
        return 1;
    }

    return 0;
}
