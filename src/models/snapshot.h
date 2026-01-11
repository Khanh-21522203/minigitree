#pragma once

#include <string>
#include <ctime>
#include <optional>

namespace minigitree::models {
    class Snapshot {
    private:
        std::string tree_hash_;
        std::string message_;
        std::optional<time_t> timestamp_;
        std::string hash_;
    public:
        Snapshot() = default;
        Snapshot(const std::string& tree_hash, const std::string& message);
        Snapshot(const std::string& tree_hash, const std::string& message, time_t timestamp);

        // Getters
        const std::string& tree_hash() const { return tree_hash_; }
        const std::string& message() const { return message_; }
        const std::string& hash() const { return hash_; }
        std::optional<time_t> timestamp() const { return timestamp_; }

        void compute_hash();

        // Serialization
        std::string serialize() const;
        static Snapshot deserialize(const std::string& data);
    };
}