#pragma once

#include <string>
#include <vector>

namespace minigitree::utils {
    class StringUtils {
    public:
        // Split string by delimiter
        static std::vector<std::string> split(const std::string& str, char delimiter);

        // Trim whitespace from both ends
        static std::string trim(const std::string& str);

        // Join strings with delimiter
        static std::string join(const std::vector<std::string>& strings, const std::string& delimiter);

        // Check if string starts with prefix
        static bool starts_with(const std::string& str, const std::string& prefix);

        // Check if string ends with suffix
        static bool ends_with(const std::string& str, const std::string& suffix);
    };
}