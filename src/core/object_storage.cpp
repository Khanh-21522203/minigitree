#include "core/object_storage.h"
#include "utils/filesystem.h"
#include "utils/hash_utils.h"
#include <stdexcept>

namespace minigitree::core {
    ObjectStorage::ObjectStorage(const Repository& repo) : repo_(repo) {}

    std::string ObjectStorage::store_blob(const std::vector<uint8_t>& content) {
        std::string hash = utils::HashUtils::compute_sha256(content);

        if (!object_exists(hash)) {
            write_object(hash, content);
        }

        return hash;
    }

    std::string ObjectStorage::store_tree(const std::string& content) {
        std::string hash = utils::HashUtils::compute_sha256(content);

        if (!object_exists(hash)) {
            write_object(hash, content);
        }

        return hash;
    }

    std::vector<uint8_t> ObjectStorage::read_blob(const std::string& hash) const {
        return read_object_binary(hash);
    }

    std::string ObjectStorage::read_tree(const std::string& hash) const {
        return read_object_text(hash);
    }

    bool ObjectStorage::object_exists(const std::string& hash) const {
        return utils::FilesystemUtils::exists(get_object_path(hash));
    }

    fs::path ObjectStorage::get_object_path(const std::string& hash) const {
        if (hash.length() < 2) {
            throw std::invalid_argument("Invalid hash: " + hash);
        }

        std::string prefix = hash.substr(0, 2);
        std::string suffix = hash.substr(2);

        return repo_.objects_dir() / prefix / suffix;
    }

    void ObjectStorage::write_object(const std::string& hash, const std::vector<uint8_t>& content) {
        fs::path object_path = get_object_path(hash);

        utils::FilesystemUtils::create_directories(object_path.parent_path());
        utils::FilesystemUtils::write_file_binary(object_path, content);
    }

    void ObjectStorage::write_object(const std::string& hash, const std::string& content) {
        fs::path object_path = get_object_path(hash);

        utils::FilesystemUtils::create_directories(object_path.parent_path());
        utils::FilesystemUtils::write_file(object_path, content);
    }

    std::vector<uint8_t> ObjectStorage::read_object_binary(const std::string& hash) const {
        fs::path object_path = get_object_path(hash);

        if (!utils::FilesystemUtils::exists(object_path)) {
            throw std::runtime_error("Object not found: " + hash);
        }

        return utils::FilesystemUtils::read_file_binary(object_path);
    }

    std::string ObjectStorage::read_object_text(const std::string& hash) const {
        fs::path object_path = get_object_path(hash);

        if (!utils::FilesystemUtils::exists(object_path)) {
            throw std::runtime_error("Object not found: " + hash);
        }

        return utils::FilesystemUtils::read_file(object_path);
    }
}