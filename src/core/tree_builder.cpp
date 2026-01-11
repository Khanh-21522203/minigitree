#include "core/tree_builder.h"
#include "models/blob.h"
#include "utils/filesystem.h"
#include "utils/hash_utils.h"
#include <algorithm>

namespace minigitree::core {
    TreeBuilder::TreeBuilder(const Repository& repo, ObjectStorage& storage)
    : repo_(repo), storage_(storage) {}

    std::string TreeBuilder::build_tree(const fs::path &path) {
        files_tracked_ = 0;
        files_changed_ = 0;
        files_reused_ = 0;

        return build_tree_recursive(path);
    }

    bool TreeBuilder::should_ignore(const fs::path &path) const {
        std::string filename = path.filename().string();

        if (filename == ".minigitree") {
            return true;
        }

        // TODO: Add more ignore patterns here if needed

        return false;
    }

    std::string TreeBuilder::build_tree_recursive(const fs::path& path) {
        models::Tree tree;

        auto entries = utils::FilesystemUtils::list_directory(path);
        std::sort(entries.begin(), entries.end());

        for (const auto& entry : entries) {
            if (should_ignore(entry)) continue;

            std::string name = entry.filename().string();
            if (utils::FilesystemUtils::is_regular_file(entry)) {
                // Handle file (blob)
                std::string hash = utils::HashUtils::compute_file_sha256(entry.string());

                bool existed = storage_.object_exists(hash);
                if (!existed) {
                    // Store new blob
                    auto content = utils::FilesystemUtils::read_file_binary(entry);
                    storage_.store_blob(content);
                    files_changed_++;
                } else {
                    files_reused_++;
                }

                files_tracked_++;
                tree.add_entry(models::TreeEntry::Type::BLOB, hash, name);
            } else if (utils::FilesystemUtils::is_directory(entry)) {
                // Handle directory (tree) - recursive
                std::string subtree_hash = build_tree_recursive(entry);
                tree.add_entry(models::TreeEntry::Type::TREE, subtree_hash, name);
            }
        }

        // Compute tree hash and store
        tree.compute_hash();
        std::string tree_content = tree.serialize();
        storage_.store_tree(tree_content);

        return tree.hash();
    }
}
