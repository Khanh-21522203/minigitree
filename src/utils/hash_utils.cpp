#include "hash_utils.h"

#include <algorithm>
#include <openssl/sha.h>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace minigitree::utils {

    std::string HashUtils::compute_sha256(const std::string& data) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, data.c_str(), data.size());
        SHA256_Final(hash, &sha256);

        std::stringstream ss;
        for (const unsigned char i : hash) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
        }
        return ss.str();
    }

    std::string HashUtils::compute_sha256(const std::vector<uint8_t>& data) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, data.data(), data.size());
        SHA256_Final(hash, &sha256);

        std::stringstream ss;
        for (const unsigned char i : hash) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
        }
        return ss.str();
    }

    std::string HashUtils::compute_file_sha256(const std::string& filepath) {
        std::ifstream file(filepath, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filepath);
        }

        SHA256_CTX sha256;
        SHA256_Init(&sha256);

        char buffer[8192];
        while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
            SHA256_Update(&sha256, buffer, file.gcount());
        }

        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_Final(hash, &sha256);

        std::stringstream ss;
        for (const unsigned char i : hash) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);
        }
        return ss.str();
    }

    std::string HashUtils::shorten_hash(const std::string& hash) {
        return hash.substr(0, 8);
    }

    bool HashUtils::is_valid_hash(const std::string &hash) {
        if (hash.length() != 64) return false;
        return std::all_of(hash.begin(), hash.end(), [](char c) {
            return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
        });
    }

    bool HashUtils::hash_matches_prefix(const std::string &full_hash, const std::string &prefix) {
        if (prefix.length() > full_hash.length()) return false;
        return full_hash.substr(0, prefix.length()) == prefix;
    }
}
