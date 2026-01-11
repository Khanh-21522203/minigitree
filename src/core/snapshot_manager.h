#pragma once

#include "core/repository.h"
#include "models/snapshot.h"
#include <vector>
#include <string>

namespace minigitree::core {
    class SnapshotManager {
    private:
        const Repository& repo_;

        // Find snapshot by prefix
        std::string find_snapshot_by_prefix(const std::string& prefix) const;
    public:
        explicit SnapshotManager(const Repository& repo);

        // Create snapshot
        std::string create_snapshot(const std::string& tree_hash,
                                    const std::string& message);

        // List all snapshots
        std::vector<models::Snapshot> list_snapshots() const;

        // Get snapshot by hash (supports prefix matching)
        models::Snapshot get_snapshot(const std::string& hash_or_prefix) const;

        // Check if snapshot exists
        bool snapshot_exists(const std::string& hash) const;
    };
}