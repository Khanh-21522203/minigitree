#include "cli/snapshot_command.h"
#include "core/repository.h"
#include "core/object_storage.h"
#include "core/tree_builder.h"
#include "core/snapshot_manager.h"
#include "utils/hash_utils.h"
#include <iostream>
#include <filesystem>

namespace minigitree::cli {
    int SnapshotCommand::execute(const std::vector<std::string>& args) {
        namespace fs = std::filesystem;

        try {
            // Find repository
            fs::path repo_root = core::Repository::find_root(fs::current_path());
            core::Repository repo(repo_root);

            // Get message
            std::string message = args.empty() ? "Snapshot" : args[0];

            // Build tree
            core::ObjectStorage storage(repo);
            core::TreeBuilder builder(repo, storage);
            std::string tree_hash = builder.build_tree(repo_root);

            // Create snapshot
            core::SnapshotManager snapshot_manager(repo);
            std::string snapshot_hash = snapshot_manager.create_snapshot(tree_hash,
                                                                          message);

            // Print result
            std::cout << "Snapshot created: " << snapshot_hash << std::endl;
            std::cout << builder.files_tracked() << " files tracked";

            if (builder.files_changed() > 0 || builder.files_reused() > 0) {
                std::cout << " (" << builder.files_changed() << " changed, "
                          << builder.files_reused() << " reused)";
            }

            std::cout << std::endl;

            return 0;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }
}