#pragma once

#include <vector>
#include <string>

namespace minigitree::cli {
    class RestoreCommand {
    public:
        static int execute(const std::vector<std::string>& args);
    };
}