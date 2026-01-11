#pragma once
#include <filesystem>
#include <vector>

namespace minigitree::utils {
    namespace fs = std::filesystem;

    class FilesystemUtils {
    public:
        // Check if path exists
        static bool exists(const fs::path& path);

        // Check if path is directory
        static bool is_directory(const fs::path& path);

        // Check if path is regular file
        static bool is_regular_file(const fs::path& path);

        // Create directory (including parents)
        static void create_directories(const fs::path& path);

        // Read entire file into string
        static std::string read_file(const fs::path& path);

        // Read entire file into byte vector
        static std::vector<uint8_t> read_file_binary(const fs::path& path);

        // Write string to file
        static void write_file(const fs::path& path, const std::string& content);

        // Write bytes to file
        static void write_file_binary(const fs::path& path, const std::vector<uint8_t>& content);

        // List directory entries
        static std::vector<fs::path> list_directory(const fs::path& path);

        // Get file size
        static size_t file_size(const fs::path& path);

        // Find repository root (search upward for .minigitree/)
        static fs::path find_repository_root(const fs::path& start_path);
    };
}
