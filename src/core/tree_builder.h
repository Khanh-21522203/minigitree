#pragma once

#include "core/repository.h"
#include "core/object_storage.h"
#include "models/tree.h"
#include <filesystem>
#include <memory>

namespace minigitree::core {
    namespace fs = std::filesystem;

    class TreeBuilder {
    private:
        const Repository& repo_;
        ObjectStorage& storage_;

        size_t files_tracked_ = 0;
        size_t files_changed_ = 0;
        size_t files_reused_ = 0;

        // Check if path should be ignored
        bool should_ignore(const fs::path& path) const;

        // Build tree recursively
        std::string build_tree_recursive(const fs::path& path);
    public:
        TreeBuilder(const Repository& repo, ObjectStorage& storage);

        // Build tree from directory
        std::string build_tree(const fs::path& path);

        // Get statistics
        size_t files_tracked() const { return files_tracked_; }
        size_t files_changed() const { return files_changed_; }
        size_t files_reused() const { return files_reused_; }
    };
}