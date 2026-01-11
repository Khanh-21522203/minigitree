#pragma once

#include "core/repository.h"
#include <string>
#include <vector>
#include <filesystem>

namespace minigitree::core {
    namespace fs = std::filesystem;

    class ObjectStorage {
    private:
        const Repository& repo_;

        // Write object to disk
        void write_object(const std::string& hash, const std::vector<uint8_t>& content);
        void write_object(const std::string& hash, const std::string& content);

        // Read object from disk
        std::vector<uint8_t> read_object_binary(const std::string& hash) const;
        std::string read_object_text(const std::string& hash) const;
    public:
        explicit ObjectStorage(const Repository& repo);

        // Store blob object
        std::string store_blob(const std::vector<uint8_t>& content);

        // Store tree object
        std::string store_tree(const std::string& content);

        // Read blob object
        std::vector<uint8_t> read_blob(const std::string& hash) const;

        // Read tree object
        std::string read_tree(const std::string& hash) const;

        // Check if object exists
        bool object_exists(const std::string& hash) const;

        // Get object path from hash
        fs::path get_object_path(const std::string& hash) const;
    };
}