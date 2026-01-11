#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace minigitree::utils {
    class HashUtils {
    public:
        // Compute SHA-256 hash of data
        static std::string compute_sha256(const std::string& data);
        static std::string compute_sha256(const std::vector<uint8_t>& data);

        // Compute SHA-256 hash of file (streaming)
        static std::string compute_file_sha256(const std::string& filepath);

        // Convert hash to short form (first 8 characters)
        static std::string shorten_hash(const std::string& hash);

        // Validate hash format
        static bool is_valid_hash(const std::string& hash);

        // Check if short_hash is prefix of full_hash
        static bool hash_matches_prefix(const std::string& full_hash, const std::string& prefix);
    };
}