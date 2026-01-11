#pragma once

#include <string>
#include <vector>

namespace minigitree::cli {
    class CommandHandler {
    private:
        // Command implementations
        static int cmd_init(const std::vector<std::string>& args);
        static int cmd_snapshot(const std::vector<std::string>& args);
        static int cmd_list(const std::vector<std::string>& args);
        static int cmd_show(const std::vector<std::string>& args);
        static int cmd_restore(const std::vector<std::string>& args);
        static int cmd_help();
        static int cmd_version();

        // Helper functions
        static void print_usage();
    public:
        static int execute(int argc, char* argv[]);
    };
}