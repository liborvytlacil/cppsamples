/**
 * Implementation of Run Length Encoding algorithm
 */

#include <vector>
#include <string>
#include <fstream>
#include <limits>

struct Repetition {
    char byte;
    uint8_t count;
};

// open input
// open output
// repeat:
// read block of bytes
// encode the block
// write encoded block to the output file

// encodes input bytes
inline std::vector<Repetition> encode(const std::vector<char> &decodedInput, size_t count) {
    // number of bytes of the input to to encode
    if (count > decodedInput.size()) {
        count = decodedInput.size();
    }

    std::vector<Repetition> result;
    size_t inputPos = 0;

    // keep reading sequence of bytes
    while (inputPos < count) {
        // start reading a new sequence

        // read the first byte
        char byte = decodedInput[inputPos];

        // the first byte is trivially a byte sequence of length 1
        uint8_t seqLen = 1;

        // keep reading repeated bytes
        ++inputPos;
        while (inputPos < count && byte == decodedInput[inputPos] && seqLen < std::numeric_limits<uint8_t>::max()) {
            ++seqLen;
            ++inputPos;
        }

        result.push_back({byte, seqLen});
    }

    return result;
}

// writes encoded input as a sequence of bytes
inline std::vector<char> toByteSequence(const std::vector<Repetition> &encodedInput) {
    std::vector<char> result;

    for (const Repetition &rep : encodedInput) {
        result.push_back(static_cast<char>(rep.count));
        result.push_back(rep.byte);
    }

    return result;
}

/*
// decodes encoded input
inline std::vector<char> decode(const std::vector<Repetition> &encodedInput) {

}

// reads encoded input
inline std::vector<Repetition> fromByteSequence(const std::vector<char> &encodedInput) {

}

// entry method
inline bool perform() {

}
*/

// entry encoding method
inline void performEncoding(std::ifstream &ifs, std::ofstream &ofs) {
    const int bufferSize = 512;

    std::vector<char> buffer(bufferSize);
    while (ifs.read(buffer.data(), bufferSize)) {
        std::streamsize bytesRead = ifs.gcount();

        std::vector<Repetition> encodedInput = encode(buffer, bytesRead);

        std::vector<char> encodedInputAsBytes = toByteSequence(encodedInput);

        if (ofs) {
            ofs.write(encodedInputAsBytes.data(), encodedInputAsBytes.size());
        }

        if (!ofs) {
            // err
        }
    }

    if (!ifs.eof()) {
        // err
    }
}





