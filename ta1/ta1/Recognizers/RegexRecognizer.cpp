#include "RegexRecognizer.h"
#include <regex>
#include <unordered_map>
#include <vector>

static const std::regex pattern(R"(^([a-zA-Z0-9./]+)\s*(?:\s+(-[a-zA-Z0-9]|--[a-zA-Z0-9]+))*$)", std::regex::optimize);
static const std::regex keyPattern(R"(-[a-zA-Z0-9]|--[a-zA-Z0-9]+)", std::regex::optimize);

bool RegexRecognizer::processCommand(const std::string& command, std::unordered_map<std::string, std::vector<std::string>>& commandKeys) const {
    std::smatch match;
    if (!std::regex_search(command, match, pattern)) {
        return false;
    }

    std::string cmdName = match[1].str();

    std::vector<std::string> newKeys{
        std::sregex_token_iterator(command.begin(), command.end(), keyPattern),
        std::sregex_token_iterator()
    };

    auto& keys = commandKeys[cmdName];
    keys.insert(keys.end(), newKeys.begin(), newKeys.end());

    return true;
}
