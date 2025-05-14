#include "generator.h"

CommandGenerator::CommandGenerator()
    : gen(std::random_device{}()),
    letterDist(0, 61),
    lenDist(3, 8),
    keyCountDist(10, 500) {
}

char CommandGenerator::randomChar(const std::string& chars) {
    return chars[letterDist(gen) % chars.size()];
}

std::string CommandGenerator::randomString(int len, const std::string& chars) {
    std::string s;
    for (int i = 0; i < len; ++i) {
        s += randomChar(chars);
    }
    return s;
}

std::string CommandGenerator::generateValidCommand(bool longMode) {
    int keyCount = longMode ? keyCountDist(gen) : std::min(10, keyCountDist(gen));
    std::string command = randomString(lenDist(gen), commandChars);

    for (int i = 0; i < keyCount; ++i) {
        if (gen() % 2 == 0) {
            command += " -" + randomString(1, keyChars);
        }
        else {
            command += " --" + randomString(lenDist(gen), keyChars);
        }
    }
    return command;
}

std::string CommandGenerator::generateInvalidCommand(bool longMode) {
    std::string command = generateValidCommand(longMode);
    int errorCount = gen() % 5 + 1;  

    for (int i = 0; i < errorCount; ++i) {
        int errorPosition = gen() % command.size();
        char errorChar = randomChar("@#$%^&*()/. "); 

        if (gen() % 2 == 0) {
            command.insert(errorPosition, 1, errorChar);
        }
        else {
            command[errorPosition] = ' '; 
        }
    }
    command.push_back('!'); 
    return command;
}
