#include "models/snapshot.h"
#include "utils/hash_utils.h"
#include "utils/string_utils.h"
#include <sstream>

namespace minigitree::models {
    Snapshot::Snapshot(const std::string& tree_hash, const std::string& message)
    : tree_hash_(tree_hash), message_(message), timestamp_(std::time(nullptr)) {
        compute_hash();
    }

    Snapshot::Snapshot(const std::string& tree_hash, const std::string& message, time_t timestamp)
        : tree_hash_(tree_hash), message_(message), timestamp_(timestamp) {
        compute_hash();
    }

    void Snapshot::compute_hash() {
        std::string content = serialize();
        hash_ = utils::HashUtils::compute_sha256(content);
    }

    std::string Snapshot::serialize() const {
        std::stringstream ss;
        ss << "tree " << tree_hash_ << "\n";
        ss << "message " << message_ << "\n";

        if (timestamp_.has_value()) {
            ss << "timestamp " << timestamp_.value() << "\n";
        }

        return ss.str();
    }

    Snapshot Snapshot::deserialize(const std::string& data) {
        Snapshot snapshot;
        auto lines = utils::StringUtils::split(data, '\n');

        for (const auto& line : lines) {
            if (line.empty()) continue;

            auto parts = utils::StringUtils::split(line, ' ');
            if (parts.size() < 2) continue;

            if (parts[0] == "tree") {
                snapshot.tree_hash_ = parts[1];
            } else if (parts[0] == "message") {
                snapshot.message_ = parts[1];
                for (size_t i = 2; i < parts.size(); i++) {
                    snapshot.message_ += " " + parts[i];
                }
            } else if (parts[0] == "timestamp") {
                snapshot.timestamp_ = std::stoll(parts[1]);
            }
        }
    }
}
