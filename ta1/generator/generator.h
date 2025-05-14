#pragma once

#include <string>
#include <random>

class CommandGenerator {
private:
    std::mt19937 gen;
    std::uniform_int_distribution<int> letterDist;
    std::uniform_int_distribution<int> lenDist;
    std::uniform_int_distribution<int> keyCountDist;

    const std::string commandChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";
    const std::string keyChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

public:
    CommandGenerator();
    char randomChar(const std::string& chars);
    std::string randomString(int len, const std::string& chars);
    std::string generateValidCommand(bool longMode);
    std::string generateInvalidCommand(bool longMode);
};
