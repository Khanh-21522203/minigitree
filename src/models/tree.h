#pragma once

#include <string>
#include <vector>
#include <memory>

namespace minigitree::models {
    struct TreeEntry {
        enum class Type {BLOB, TREE};

        Type type;
        std::string hash;
        std::string name;

        TreeEntry(Type t, const std::string& h, const std::string& n)
            : type(t), hash(h), name(n) {}
    };

    class Tree {
    private:
        std::vector<TreeEntry> entries_;
        std::string hash_;
    public:
        Tree() = default;

        void add_entry(TreeEntry::Type type, const std::string& hash, const std::string& name);

        // Getters
        const std::vector<TreeEntry>& entries() const { return entries_; }
        const std::string& hash() const { return hash_; }

        void compute_hash();
        void sort_entries();

        // Serialization
        std::string serialize() const;
        static Tree deserialize(const std::string& data);
    };
}