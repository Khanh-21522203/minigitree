#include "string_utils.h"
#include <sstream>
#include <algorithm>

namespace minigitree::utils {

    std::vector<std::string> StringUtils::split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::istringstream tokenStream(str);
        std::string token;

        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }

        return tokens;
    }

    std::string StringUtils::join(const std::vector<std::string>& strs, const std::string& delimiter) {
        if (strs.empty()) return "";

        std::string result = strs[0];
        for (size_t i = 1; i < strs.size(); i++) {
            result += delimiter + strs[i];
        }
        return result;
    }

    bool StringUtils::starts_with(const std::string& str, const std::string& prefix) {
        if (prefix.length() > str.length()) return false;
        return str.substr(0, prefix.length()) == prefix;
    }

    bool StringUtils::ends_with(const std::string& str, const std::string& suffix) {
        if (suffix.length() > str.length()) return false;
        return str.substr(str.length() - suffix.length()) == suffix;
    }
}