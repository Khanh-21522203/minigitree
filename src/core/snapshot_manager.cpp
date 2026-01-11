#include "core/snapshot_manager.h"
#include "utils/filesystem.h"
#include "utils/hash_utils.h"
#include <algorithm>

namespace minigitree::core {
    SnapshotManager::SnapshotManager(const Repository& repo) : repo_(repo) {}

    std::string SnapshotManager::create_snapshot(const std::string &tree_hash, const std::string &message) {
        models::Snapshot snapshot(tree_hash, message);

        fs::path snapshot_path = repo_.snapshots_dir() / snapshot.hash();
        utils::FilesystemUtils::write_file(snapshot_path, snapshot.serialize());

        return snapshot.hash();
    }

    std::vector<models::Snapshot> SnapshotManager::list_snapshots() const {
        std::vector<models::Snapshot> snapshots;

        if (!utils::FilesystemUtils::exists(repo_.snapshots_dir())) {
            return snapshots;
        }

        auto entries = utils::FilesystemUtils::list_directory(repo_.snapshots_dir());
        for (const auto& entry : entries) {
            if (utils::FilesystemUtils::is_regular_file(entry)) {
                std::string content = utils::FilesystemUtils::read_file(entry);
                models::Snapshot snapshot = models::Snapshot::deserialize(content);
                snapshots.push_back(snapshot);
            }
        }

        // Sort by timestamp (newest first)
        std::sort(snapshots.begin(), snapshots.end(),
                  [](const models::Snapshot& a, const models::Snapshot& b) {
                      if (!a.timestamp().has_value() || !b.timestamp().has_value()) {
                          return false;
                      }
                      return a.timestamp().value() > b.timestamp().value();
                  });

        return snapshots;
    }

    models::Snapshot SnapshotManager::get_snapshot(const std::string& hash_or_prefix) const {
        std::string full_hash = hash_or_prefix;
        if (hash_or_prefix.length() < 64) {
            full_hash = find_snapshot_by_prefix(hash_or_prefix);
        }

        fs::path snapshot_path = repo_.snapshots_dir() / full_hash;

        if (!utils::FilesystemUtils::exists(snapshot_path)) {
            throw std::runtime_error("Snapshot not found: " + hash_or_prefix);
        }

        std::string content = utils::FilesystemUtils::read_file(snapshot_path);
        return models::Snapshot::deserialize(content);
    }

    bool SnapshotManager::snapshot_exists(const std::string& hash) const {
        fs::path snapshot_path = repo_.snapshots_dir() / hash;
        return utils::FilesystemUtils::exists(snapshot_path);
    }

    std::string SnapshotManager::find_snapshot_by_prefix(const std::string& prefix) const {
        auto snapshots = list_snapshots();
        std::vector<std::string> matches;

        for (const auto& snapshot : snapshots) {
            if (utils::HashUtils::hash_matches_prefix(snapshot.hash(), prefix)) {
                matches.push_back(snapshot.hash());
            }
        }

        if (matches.empty()) {
            throw std::runtime_error("Snapshot not found: " + prefix);
        }

        if (matches.size() > 1) {
            throw std::runtime_error("Ambiguous hash prefix: " + prefix);
        }

        return matches[0];
    }
}
