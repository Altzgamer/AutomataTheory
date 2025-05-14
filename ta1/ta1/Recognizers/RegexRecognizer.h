#ifndef REGEX_RECOGNIZER_H
#define REGEX_RECOGNIZER_H
#include <string>
#include <unordered_map>
#include <vector>
#include <regex>
#include "IMyRecognizer.h"


class RegexRecognizer : public IMyRecognizer {
public:
    bool processCommand(const std::string& command, std::unordered_map<std::string, std::vector<std::string>>& commandKeys) const override;
};

#endif