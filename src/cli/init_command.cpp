#include "cli/init_command.h"
#include "core/repository.h"
#include <iostream>
#include <filesystem>

namespace minigitree::cli {
    int InitCommand::execute(const std::vector<std::string>& args) {
        namespace fs = std::filesystem;

        fs::path path = fs::current_path();

        try {
            core::Repository::init(path);
            std::cout << "Initialized empty MiniGitree repository in " << (path / ".minigitree").string() << std::endl;
            return 0;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }
}