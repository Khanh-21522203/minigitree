#include "core/restore_engine.h"
#include "core/snapshot_manager.h"
#include "utils/filesystem.h"

namespace minigitree::core {
    RestoreEngine::RestoreEngine(const Repository& repo, ObjectStorage& storage)
    : repo_(repo), storage_(storage) {}

    void RestoreEngine::restore_snapshot(const std::string& snapshot_hash) {
        files_restored_ = 0;

        // Get snapshot
        SnapshotManager snapshot_manager(repo_);
        models::Snapshot snapshot = snapshot_manager.get_snapshot(snapshot_hash);

        // Restore root tree
        restore_tree(snapshot.tree_hash(), repo_.root_path());
    }

    void RestoreEngine::restore_tree(const std::string& tree_hash, const fs::path& target_path) {
        // Read tree object
        std::string tree_content = storage_.read_tree(tree_hash);
        models::Tree tree = models::Tree::deserialize(tree_content);

        // Process each entry
        for (const auto& entry : tree.entries()) {
            fs::path entry_path = target_path / entry.name;

            if (entry.type == models::TreeEntry::Type::BLOB) {
                // Restore file
                auto content = storage_.read_blob(entry.hash);
                utils::FilesystemUtils::write_file_binary(entry_path, content);
                files_restored_++;

            } else if (entry.type == models::TreeEntry::Type::TREE) {
                // Create directory and restore recursively
                utils::FilesystemUtils::create_directories(entry_path);
                restore_tree(entry.hash, entry_path);
            }
        }
    }
}