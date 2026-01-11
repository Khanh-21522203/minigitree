#include "cli/command_handler.h"
#include "cli/init_command.h"
#include "cli/snapshot_command.h"
#include "cli/list_command.h"
#include "cli/show_command.h"
#include "cli/restore_command.h"
#include <iostream>

namespace minigitree::cli {
    int CommandHandler::execute(int argc, char* argv[]) {
        if (argc < 2) {
            print_usage();
            return 1;
        }

        std::string command = argv[1];
        std::vector<std::string> args;

        for (int i = 2; i < argc; i++) {
            args.emplace_back(argv[i]);
        }

        try {
            if (command == "init") {
                return cmd_init(args);
            } else if (command == "snapshot" || command == "snap") {
                return cmd_snapshot(args);
            } else if (command == "list" || command == "ls") {
                return cmd_list(args);
            } else if (command == "show") {
                return cmd_show(args);
            } else if (command == "restore" || command == "checkout") {
                return cmd_restore(args);
            } else if (command == "--help" || command == "-h") {
                return cmd_help();
            } else if (command == "--version" || command == "-v") {
                return cmd_version();
            } else {
                std::cerr << "Unknown command: " << command << std::endl;
                print_usage();
                return 1;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }

    int CommandHandler::cmd_init(const std::vector<std::string>& args) {
        return InitCommand::execute(args);
    }

    int CommandHandler::cmd_snapshot(const std::vector<std::string>& args) {
        return SnapshotCommand::execute(args);
    }

    int CommandHandler::cmd_list(const std::vector<std::string>& args) {
        return ListCommand::execute(args);
    }

    int CommandHandler::cmd_show(const std::vector<std::string>& args) {
        return ShowCommand::execute(args);
    }

    int CommandHandler::cmd_restore(const std::vector<std::string>& args) {
        return RestoreCommand::execute(args);
    }

    int CommandHandler::cmd_help() {
        print_usage();
        return 0;
    }

    int CommandHandler::cmd_version() {
        std::cout << "MiniGitree version 1.0.0" << std::endl;
        return 0;
    }

    void CommandHandler::print_usage() {
        std::cout << "MiniGitree - A lightweight version control system\n\n";
        std::cout << "Usage: minigitree <command> [options]\n\n";
        std::cout << "Commands:\n";
        std::cout << "  init              Initialize a new repository\n";
        std::cout << "  snapshot [msg]    Create a snapshot\n";
        std::cout << "  list              List all snapshots\n";
        std::cout << "  show <hash>       Show snapshot details\n";
        std::cout << "  restore <hash>    Restore a snapshot\n\n";
        std::cout << "Options:\n";
        std::cout << "  --help, -h        Show this help message\n";
        std::cout << "  --version, -v     Show version information\n";
    }
}