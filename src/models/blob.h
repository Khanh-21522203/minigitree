#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace minigitree::models {
    class Blob {
    private:
        std::vector<uint8_t> content_;
        std::string hash_;
    public:
        Blob() = default;
        explicit Blob(const std::vector<uint8_t>& content);
        explicit Blob(const std::string& content);

        // Getters
        const std::vector<uint8_t>& content() const { return content_; }
        const std::string& hash() const { return hash_; }
        size_t size() const { return content_.size(); }

        void compute_hash();

        // Serialization
        std::vector<uint8_t> serialize() const;
        static Blob deserialize(const std::vector<uint8_t>& data);
    };
}