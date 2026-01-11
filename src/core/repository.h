#pragma once

#include <filesystem>
#include <string>

namespace minigitree::core {
    namespace fs = std::filesystem;

    class Repository {
    private:
        fs::path root_path_;
    public:
        explicit Repository(const fs::path& root_path);

        // Initialize new repository
        static void init(const fs::path& path);

        // Check if path is in a repository
        static bool is_repository(const fs::path& path);

        // Find repository root from current directory
        static fs::path find_root(const fs::path& start_path);

        // Getters
        const fs::path& root_path() const { return root_path_; }
        fs::path minigitree_dir() const { return root_path_ / ".minigitree"; }
        fs::path objects_dir() const { return minigitree_dir() / "objects"; }
        fs::path snapshots_dir() const { return minigitree_dir() / "snapshots"; }
        fs::path config_file() const { return minigitree_dir() / "config"; }
    };
}