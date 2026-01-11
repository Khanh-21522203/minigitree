#include "blob.h"
#include "utils/hash_utils.h"

namespace minigitree::models {
    Blob::Blob(const std::vector<uint8_t> &content) : content_(content) {
        compute_hash();
    }

    Blob::Blob(const std::string &content) {
        content_.assign(content.begin(), content.end());
        compute_hash();
    }

    void Blob::compute_hash() {
        hash_ = utils::HashUtils::compute_sha256(content_);
    }

    std::vector<uint8_t> Blob::serialize() const {
        return content_;
    }

    Blob Blob::deserialize(const std::vector<uint8_t>& data) {
        return Blob(data);
    }
}
