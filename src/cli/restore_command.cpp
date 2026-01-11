#include "cli/restore_command.h"
#include "core/repository.h"
#include "core/object_storage.h"
#include "core/restore_engine.h"
#include "utils/hash_utils.h"
#include <iostream>
#include <filesystem>

namespace minigitree::cli {
    int RestoreCommand::execute(const std::vector<std::string>& args) {
        namespace fs = std::filesystem;

        if (args.empty()) {
            std::cerr << "Usage: minigitree restore <snapshot_hash>" << std::endl;
            return 1;
        }

        try {
            // Find repository
            fs::path repo_root = core::Repository::find_root(fs::current_path());
            core::Repository repo(repo_root);

            // Restore snapshot
            core::ObjectStorage storage(repo);
            core::RestoreEngine engine(repo, storage);
            engine.restore_snapshot(args[0]);

            // Print result
            std::string short_hash = utils::HashUtils::shorten_hash(args[0]);
            std::cout << "Restored snapshot " << short_hash << std::endl;
            std::cout << engine.files_restored() << " files restored" << std::endl;

            return 0;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
    }
}