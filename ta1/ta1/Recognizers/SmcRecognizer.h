#pragma once


#include "IMyRecognizer.h"
#include "SmcRecognizer_sm.h"
#include <unordered_map>
#include <vector>
#include <string>

class SmcRecognizer : public IMyRecognizer {
public:
    bool processCommand(const std::string& command, std::unordered_map<std::string, std::vector<std::string>>& commandKeys) const override;

    void add_to_command_name(char c);
    void process_command();
    void add_single_modifier(char c);
    void add_multi_modifier(char c);
    void reset();
    bool isValidKey(const std::string& key) const;
    bool isValidCommandName(const std::string& name) const;
};
