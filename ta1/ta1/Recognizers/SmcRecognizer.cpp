#include "SmcRecognizer.h"
#include <cctype>
#include <iostream>

namespace {
    std::string currentCommandName;
    std::string currentModifier;
    std::vector<std::string> currentModifiers;
}

void SmcRecognizer::reset() {
    currentCommandName.clear();
    currentModifier.clear();
    currentModifiers.clear();
}

void SmcRecognizer::add_to_command_name(char c) {
    currentCommandName.push_back(c);
}

void SmcRecognizer::add_single_modifier(char c) {
    currentModifiers.push_back(std::string("-") + c);
}

void SmcRecognizer::add_multi_modifier(char c) {
    currentModifier.push_back(c);
}

void SmcRecognizer::process_command() {
    if (!currentModifier.empty()) {
        currentModifiers.push_back(std::string("--") + currentModifier);
        currentModifier.clear();
    }
}

bool SmcRecognizer::isValidKey(const std::string& key) const {
    if (key.empty()) return false;
    std::string core;
    if (key.size() > 1 && key[0] == '-' && key[1] == '-') {
        core = key.substr(2);
    }
    else if (key.size() > 0 && key[0] == '-') {
        core = key.substr(1);
    }
    else {
        core = key;
    }
    for (char c : core) {
        if (!std::isalnum(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

bool SmcRecognizer::isValidCommandName(const std::string& name) const {
    if (name.empty()) return false;
    for (char c : name) {
        if (!(std::isalnum(static_cast<unsigned char>(c)) || c == '.' || c == '/')) return false;
    }
    return true;
}

bool SmcRecognizer::processCommand(const std::string& command, std::unordered_map<std::string, std::vector<std::string>>& commandKeys) const {
    auto self = const_cast<SmcRecognizer*>(this);
    self->reset();

    size_t i = 0, n = command.size();
    if (n == 0) return false;

    while (i < n && !std::isspace(static_cast<unsigned char>(command[i]))) {
        char c = command[i];
        if (!(std::isalnum(static_cast<unsigned char>(c)) || c == '.' || c == '/')) return false;
        self->add_to_command_name(c);
        ++i;
    }

    if (!isValidCommandName(currentCommandName)) return false;

    while (i < n && std::isspace(static_cast<unsigned char>(command[i]))) ++i;
    if (i >= n) return false;

    while (i < n) {
        if (command[i] != '-') return false;
        ++i;
        if (i >= n) return false;

        if (command[i] == '-') {
            ++i;
            if (i >= n || std::isspace(static_cast<unsigned char>(command[i]))) return false;
            currentModifier.clear();
            while (i < n && std::isalnum(static_cast<unsigned char>(command[i]))) {
                self->add_multi_modifier(command[i]);
                ++i;
            }
            self->process_command();
        }
        else {
            char key = command[i];
            if (!std::isalnum(static_cast<unsigned char>(key))) return false;
            self->add_single_modifier(key);
            ++i;
        }

        while (i < n && std::isspace(static_cast<unsigned char>(command[i]))) ++i;
    }

    auto& existingKeys = commandKeys[currentCommandName];
    existingKeys.insert(existingKeys.end(), currentModifiers.begin(), currentModifiers.end());

    return true;
}
