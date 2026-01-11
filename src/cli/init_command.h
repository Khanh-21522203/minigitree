#pragma once

#include <vector>
#include <string>

namespace minigitree::cli {
    class InitCommand {
    public:
        static int execute(const std::vector<std::string>& args);
    };
}