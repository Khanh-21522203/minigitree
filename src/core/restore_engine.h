#pragma once

#include "core/repository.h"
#include "core/object_storage.h"
#include "models/tree.h"
#include <filesystem>

namespace minigitree::core {
    class RestoreEngine {
    private:
        const Repository& repo_;
        ObjectStorage& storage_;

        size_t files_restored_ = 0;

        // Restore tree recursively
        void restore_tree(const std::string& tree_hash, const fs::path& target_path);
    public:
        RestoreEngine(const Repository& repo, ObjectStorage& storage);

        // Restore snapshot to working directory
        void restore_snapshot(const std::string& snapshot_hash);

        // Get statistics
        size_t files_restored() const { return files_restored_; }
    }
}