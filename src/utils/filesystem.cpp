#include "filesystem.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace minigitree::utils {

    bool FilesystemUtils::exists(const fs::path& path) {
        return fs::exists(path);
    }

    bool FilesystemUtils::is_directory(const fs::path& path) {
        return fs::is_directory(path);
    }

    bool FilesystemUtils::is_regular_file(const fs::path& path) {
        return fs::is_regular_file(path);
    }

    void FilesystemUtils::create_directories(const fs::path& path) {
        fs::create_directories(path);
    }

    std::string FilesystemUtils::read_file(const fs::path& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + path.string());
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::vector<uint8_t> FilesystemUtils::read_file_binary(const fs::path& path) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + path.string());
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<uint8_t> buffer(size);
        if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
            throw std::runtime_error("Failed to read file: " + path.string());
        }

        return buffer;
    }
    void FilesystemUtils::write_file(const fs::path& path, const std::string& content) {
        std::ofstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to create file: " + path.string());
        }
        file << content;
    }

    void FilesystemUtils::write_file_binary(const fs::path& path, const std::vector<uint8_t>& content) {
        std::ofstream file(path, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to create file: " + path.string());
        }
        file.write(reinterpret_cast<const char*>(content.data()), content.size());
    }

    std::vector<fs::path> FilesystemUtils::list_directory(const fs::path& path) {
        std::vector<fs::path> entries;
        for (const auto& entry : fs::directory_iterator(path)) {
            entries.push_back(entry.path());
        }
        return entries;
    }

    size_t FilesystemUtils::file_size(const fs::path& path) {
        return fs::file_size(path);
    }

    fs::path FilesystemUtils::find_repository_root(const fs::path& start_path) {
        fs::path current = fs::absolute(start_path);

        while (current.has_parent_path()) {
            fs::path repo_dir = current / ".minigitree";
            if (exists(repo_dir) && is_directory(repo_dir)) {
                return current;
            }
            current = current.parent_path();
        }

        throw std::runtime_error("Not in a MiniGitree repository");
    }
}