/**
 * RLE-encoder
 */

#include <iostream>
#include <string>
#include "input.hpp"

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