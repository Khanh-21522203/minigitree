#include "cli/show_command.h"
#include "core/repository.h"
#include "core/snapshot_manager.h"
#include "core/object_storage.h"
#include "models/tree.h"
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <ctime>

namespace minigitree::cli {
    int ShowCommand::execute(const std::vector<std::string>& args) {
        namespace fs = std::filesystem;

        if (args.empty()) {
            std::cerr << "Usage: minigitree show <snapshot_hash>" << std::endl;
            return 1;
        }

        try {
            // Find repository
            fs::path repo_root = core::Repository::find_root(fs::current_path());
            core::Repository repo(repo_root);

            // Get snapshot
            core::SnapshotManager snapshot_manager(repo);
            models::Snapshot snapshot = snapshot_manager.get_snapshot(args[0]);

            // Print snapshot info
            std::cout << "Snapshot: " << snapshot.hash() << std::endl;
            std::cout << "Message: " << snapshot.message() << std::endl;

            if (snapshot.timestamp().has_value()) {
                time_t timestamp = snapshot.timestamp().value();
                std::tm* tm = std::localtime(&timestamp);
                std::cout << "Date: "
                          << std::put_time(tm, "%Y-%m-%d %H:%M:%S")
                          << std::endl;
            }

            std::cout << "\nTree structure:" << std::endl;

            // Print tree
            print_tree(snapshot.tree_hash(), repo_root, 0);

            return 0;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }

    void ShowCommand::print_tree(const std::string& tree_hash,
                                const std::filesystem::path& repo_root,
                                int indent) {
        core::Repository repo(repo_root);
        core::ObjectStorage storage(repo);

        // Read tree
        std::string tree_content = storage.read_tree(tree_hash);
        models::Tree tree = models::Tree::deserialize(tree_content);

        // Print entries
        for (const auto& entry : tree.entries()) {
            // Print indentation
            for (int i = 0; i < indent; i++) {
                std::cout << "  ";
            }

            // Print entry
            std::cout << entry.name;

            if (entry.type == models::TreeEntry::Type::BLOB) {
                std::cout << " (blob: " << entry.hash << ")";
            } else {
                std::cout << "/ (tree: " << entry.hash << ")";
            }

            std::cout << std::endl;

            // Recurse for trees
            if (entry.type == models::TreeEntry::Type::TREE) {
                print_tree(entry.hash, repo_root, indent + 1);
            }
        }
    }
}