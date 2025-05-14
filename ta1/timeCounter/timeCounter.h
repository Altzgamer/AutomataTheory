#pragma once

#include <chrono>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "../generator/generator.h"
#include "../ta1/Recognizers/IMyRecognizer.h"
#include "../ta1/Recognizers/RegexRecognizer.h"
#include "../ta1/Recognizers/FlexRecognizer.h"
#include "../ta1/Recognizers/SmcRecognizer.h"

class TimeAnalyzer {
public:
    static double timeAnalysis(IMyRecognizer& recognizer, const std::vector<std::string>& strings) {
        size_t numStrings = strings.size();
        auto totalStart = std::chrono::high_resolution_clock::now();
        for (const auto& str : strings) {
            std::unordered_map<std::string, std::vector<std::string>> commandKeys;
            recognizer.processCommand(str, commandKeys);
        }
        auto totalEnd = std::chrono::high_resolution_clock::now();
        auto totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(totalEnd - totalStart);
        return static_cast<double>(totalDuration.count()) / numStrings;
    }
};

