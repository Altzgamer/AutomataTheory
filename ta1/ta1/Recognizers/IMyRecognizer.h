#pragma once
#include <string>
#include <unordered_map>
#include <vector>

class IMyRecognizer {
public:
    virtual ~IMyRecognizer() = default;
    virtual bool processCommand(const std::string& command, std::unordered_map<std::string, std::vector<std::string>>& commandKeys) const = 0;
};
