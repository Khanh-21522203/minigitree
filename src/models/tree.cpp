#include "models/tree.h"
#include "utils/hash_utils.h"
#include "utils/string_utils.h"
#include <algorithm>
#include <sstream>

namespace minigitree::models {
    void Tree::add_entry(TreeEntry::Type type, const std::string &hash, const std::string &name) {
        entries_.emplace_back(type, hash, name);
    }

    void Tree::sort_entries() {
        std::sort(entries_.begin(), entries_.end(),
            [](const TreeEntry& entry1, const TreeEntry& entry2) {
                return entry1.name < entry2.name;
            });
    }

    void Tree::compute_hash() {
        sort_entries();
        std::string content = serialize();
        hash_ = utils::HashUtils::compute_sha256(content);
    }

    std::string Tree::serialize() const {
        std::stringstream ss;

        for (const auto& entry : entries_) {
            std::string type_str = (entry.type == TreeEntry::Type::BLOB) ? "blob" : "tree";
            ss << type_str << " " << entry.hash << " " << entry.name << "\n";
        }

        return ss.str();
    }

    Tree Tree::deserialize(const std::string& data) {
        Tree tree;
        auto lines = utils::StringUtils::split(data, '\n');

        for (const auto& line : lines) {
            if (line.empty()) continue;

            auto parts = utils::StringUtils::split(line, ' ');
            if (parts.size() < 3) continue;

            TreeEntry::Type type = (parts[0] == "blob") ? TreeEntry::Type::BLOB : TreeEntry::Type::TREE;
            std::string hash = parts[1];
            std::string name = parts[2];

            for (size_t i = 3; i < parts.size(); ++i) {
                name += " " + parts[i];
            }

            tree.add_entry(type, hash, name);
        }

        tree.compute_hash();
        return tree;
    }
}
