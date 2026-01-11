#include "core/repository.h"
#include "utils/filesystem.h"
#include <stdexcept>

namespace minigitree::core {
    Repository::Repository(const fs::path &root_path) {
        if (!is_directory(root_path)) {
            throw std::runtime_error("Not a MiniGitree repository: " + root_path.string());
        }
    }

    void Repository::init(const fs::path &path) {
        fs::path minigitree_dir = path / ".minigitree";
        if (utils::FilesystemUtils::exists(minigitree_dir)) {
            throw std::runtime_error("Repository already exists");
        }

        utils::FilesystemUtils::create_directories(minigitree_dir / "objects");
        utils::FilesystemUtils::create_directories(minigitree_dir / "snapshots");

        fs::path config_file = minigitree_dir / "config";
        utils::FilesystemUtils::write_file(config_file, "version=1\nhash_algorithm=sha256\n");
    }

    bool Repository::is_repository(const fs::path &path) {
        fs::path minigitree_dir = path / ".minigitree";
        return utils::FilesystemUtils::exists(minigitree_dir) &&
            utils::FilesystemUtils::is_directory(minigitree_dir);
    }

    fs::path Repository::find_root(const fs::path &start_path) {
        return utils::FilesystemUtils::find_repository_root(start_path);
    }
}
