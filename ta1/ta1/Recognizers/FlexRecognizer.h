#pragma once

#include "IMyRecognizer.h"
#include <unordered_map>
#include <vector>
#include <string>

class FlexRecognizer : public IMyRecognizer {
public:
    bool processCommand(const std::string& input, std::unordered_map<std::string, std::vector<std::string>>& commandKeys) const override;
};
