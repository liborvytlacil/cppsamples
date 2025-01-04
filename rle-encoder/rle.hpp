/**
 * Implementation of Run Length Encoding algorithm.
 */

#ifndef RLE_HPP
#define RLE_HPP

#include <vector>
#include <string>
#include <fstream>
#include <limits>

/**
 * Represents a repetition of a single byte.
 */
struct Repetition {
    uint8_t count;
    char byte;
};

/**
 * Exception to be thrown when encoding or decoding algorithm encounters an error caused
 * primarily by invalid input data.
 */
class RLEException : public std::runtime_error {
public:
    explicit RLEException(const std::string &message)
    : std::runtime_error(message) { }
};

/**
 * Reads the given number bytes from the given vector and encodes them using RLE.
 * 
 * @param decodedInput vector of bytes representing decoded input
 * @param count number of bytes from the vector to actually encode. If count is greater than the vector size, all bytes are encoded.
 * 
 * @return encoded input
 */
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

        result.push_back({seqLen, byte});
    }

    return result;
}

/**
 * Converst encoded input from its intermediate representation to a vector of bytes that can be
 * directly written to an output stream.
 * 
 * @param encodedInput encoded input in an intermediate representation
 * 
 * @return encoded data as a vector of bytes
 */
inline std::vector<char> toByteSequence(const std::vector<Repetition> &encodedInput) {
    std::vector<char> result;

    for (const Repetition &rep : encodedInput) {
        result.push_back(static_cast<char>(rep.count));
        result.push_back(rep.byte);
    }

    return result;
}

/**
 * Decodes encoded input into a vector of bytes that can be directly written to an output stream.
 * 
 * @param encodedInput encoded input in an intermediate form
 * 
 * @return decoded data as a vector of bytes
 */
inline std::vector<char> decode(const std::vector<Repetition> &encodedInput) {
    std::vector<char> result;
    for (const Repetition &rep : encodedInput) {
        for (uint8_t i = 0; i < rep.count; ++i) {
            result.push_back(rep.byte);
        }
    }

    return result;
}

/**
 * Parses encoded input into an intermediate form that can then be decoded.
 * 
 * @param encodedInput input to parse as a vector of bytes
 * @param count number of bytes to actually parse from the input vector. If count is greater than the size
 * of the vector, the whole vector is parsed.
 * 
 * @result parsed encoded input
 * 
 * @throws RLEException when the count of the input bytes does not match the expected structure (must be even)
 */
inline std::vector<Repetition> parseEncodedInput(const std::vector<char> &encodedInput, size_t count) {
    // number of bytes of the input to to encode
    if (count > encodedInput.size()) {
        count = encodedInput.size();
    }

    std::vector<Repetition> result;
    size_t inputPos = 0;
    while (inputPos < count) {
        Repetition rep;

        rep.count = static_cast<uint8_t>(encodedInput[inputPos]);
        ++inputPos;
        
        if (inputPos < count) {
            rep.byte = encodedInput[inputPos];
            result.push_back(rep);
            ++inputPos;
        } else {
            throw RLEException("Encoded input must contain even number of bytes.");
        }
    }

    return result;
}


const int bufferSize = 512;

/**
 * Reads the raw bytes from the input stream, encodes them using RLE and writes the encoded result
 * into the given output stream. Input is encoded in chunks of 512 bytes.
 * 
 * @param ifs input stream containing decoded data, expected to be in OK state
 * @param ofs output stream to write encoded data, expected to be in OK state
 */
inline void performEncoding(std::ifstream &ifs, std::ofstream &ofs) {
    std::vector<char> buffer(bufferSize);

    while (!ifs.eof()) {
        ifs.read(buffer.data(), bufferSize);
        std::streamsize bytesRead = ifs.gcount();

        if (!ifs.eof() && (ifs.fail() || ifs.bad())) {
            throw RLEException {"Input stream is in a failed state."};
        }

        if (bytesRead > 0) {
            std::vector<Repetition> encodedInput = encode(buffer, bytesRead);
            std::vector<char> encodedInputAsBytes = toByteSequence(encodedInput);

            if (ofs) {
                ofs.write(encodedInputAsBytes.data(), encodedInputAsBytes.size());
            }

            if (!ofs) {
                throw RLEException {"Output stream is in a failed state."};
            }
        }
    }
}

/**
 * Reads the raw bytes from the input stream, decodes them using RLE and writes the decoded result
 * into the given output stream. Input is decoded in chunks of 512 bytes.
 * 
 * @param ifs input stream containing encoded data, expected to be in OK state
 * @param ofs output stream to write decoded data, expected to be in OK state
 */
inline void performDecoding(std::ifstream &ifs, std::ofstream &ofs) {
    std::vector<char> buffer(bufferSize);

    while (!ifs.eof()) {
        ifs.read(buffer.data(), bufferSize);
        std::streamsize bytesRead = ifs.gcount();

        if (!ifs.eof() && (ifs.fail() || ifs.bad())) {
            throw RLEException {"Input stream is in a failed state."};
        }

        if (bytesRead > 0) {
            std::vector<Repetition> encodedInput = parseEncodedInput(buffer, bytesRead);
            std::vector<char> decodedInputAsBytes = decode(encodedInput);

            if (ofs) {
                ofs.write(decodedInputAsBytes.data(), decodedInputAsBytes.size());
            }

            if (!ofs) {
                throw RLEException {"Output stream is in a failed state."};
            }
        }
    }
}

#endif




