#include "cli/list_command.h"
#include "core/repository.h"
#include "core/snapshot_manager.h"
#include "utils/hash_utils.h"
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <ctime>

namespace minigitree::cli {
    int ListCommand::execute(const std::vector<std::string>& args) {
        namespace fs = std::filesystem;

        try {
            // Find repository
            fs::path repo_root = core::Repository::find_root(fs::current_path());
            core::Repository repo(repo_root);

            // List snapshots
            core::SnapshotManager snapshot_manager(repo);
            auto snapshots = snapshot_manager.list_snapshots();

            if (snapshots.empty()) {
                std::cout << "No snapshots found" << std::endl;
                return 0;
            }

            // Print snapshots
            for (const auto& snapshot : snapshots) {
                std::string short_hash = utils::HashUtils::shorten_hash(
                    snapshot.hash());

                std::cout << short_hash << " - " << snapshot.message();

                if (snapshot.timestamp().has_value()) {
                    time_t timestamp = snapshot.timestamp().value();
                    std::tm* tm = std::localtime(&timestamp);

                    std::cout << " ("
                              << std::put_time(tm, "%Y-%m-%d %H:%M:%S")
                              << ")";
                }

                std::cout << std::endl;
            }

            return 0;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }
}