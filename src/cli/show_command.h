#pragma once

#include <vector>
#include <string>
#include <filesystem>

namespace minigitree::cli {
    class ShowCommand {
    private:
        static void print_tree(const std::string& tree_hash, const std::filesystem::path& repo_root, int indent = 0);
    public:
        static int execute(const std::vector<std::string>& args);
    };
}