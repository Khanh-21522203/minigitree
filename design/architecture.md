# MiniGitree Architecture Diagrams

## 1. C4 Context — System Overview

```mermaid
flowchart TB
    subgraph boundary[MiniGitree System]
        S[("⚙️ minigitree\nCLI Tool")]
    end

    U[("👤 Developer")]
    FS[("🗄️ Filesystem\nWorking Directory")]
    OS[("🔑 OpenSSL\nSHA-256 Hashing")]

    U -->|"init / snapshot / restore / list / show"| S
    S -->|"Read working tree"| FS
    S -->|"Write .minigitree/ objects"| FS
    S -->|"Compute SHA-256 hashes"| OS
```

---

## 2. C4 Component — Internal Structure

```mermaid
flowchart TB
    subgraph cli[CLI Layer]
        CH[CommandHandler]
        IC[InitCommand]
        SC[SnapshotCommand]
        LC[ListCommand]
        SHC[ShowCommand]
        RC[RestoreCommand]
    end

    subgraph core[Core Layer]
        REPO[Repository]
        OBJ[ObjectStorage]
        TB[TreeBuilder]
        SM[SnapshotManager]
        RE[RestoreEngine]
    end

    subgraph models[Domain Models]
        BLOB[Blob]
        TREE[Tree]
        SNAP[Snapshot]
    end

    subgraph utils[Utilities]
        HASH[HashUtils\nSHA-256]
        FSUTIL[FilesystemUtils]
        STRUTIL[StringUtils]
    end

    CH --> IC & SC & LC & SHC & RC

    IC --> REPO
    SC --> REPO & OBJ & TB & SM
    LC --> SM
    SHC --> SM & OBJ
    RC --> RE & SM

    TB --> OBJ & BLOB & TREE
    SM --> SNAP & FSUTIL
    RE --> OBJ & TREE
    OBJ --> FSUTIL

    BLOB --> HASH
    TREE --> HASH
    SNAP --> HASH
    OBJ --> HASH
    REPO --> FSUTIL
    TB --> HASH & FSUTIL
```

---

## 3. Sequence — Snapshot Creation

```mermaid
sequenceDiagram
    participant U as Developer
    participant CH as CommandHandler
    participant SC as SnapshotCommand
    participant REPO as Repository
    participant TB as TreeBuilder
    participant OBJ as ObjectStorage
    participant SM as SnapshotManager

    U->>CH: minigitree snapshot "message"
    CH->>SC: execute(args)
    SC->>REPO: find_root(cwd)
    REPO-->>SC: repo_root

    loop For each file in working dir
        SC->>TB: build_tree(root_path)
        TB->>TB: compute_file_sha256(file)
        TB->>OBJ: object_exists(hash)
        OBJ-->>TB: bool
        alt New file
            TB->>OBJ: store_blob(content)
            Note right of OBJ: Written to .minigitree/objects/xx/...
        else Already stored
            Note right of TB: Reuse hash (deduplication)
        end
        TB->>TB: build Tree with sorted entries
        TB->>OBJ: store_tree(serialized_tree)
    end

    TB-->>SC: root_tree_hash + stats

    SC->>SM: create_snapshot(tree_hash, message)
    SM->>SM: serialize Snapshot
    SM->>SM: compute_hash()
    SM->>SM: write to snapshots/<hash>
    SM-->>SC: snapshot_hash

    SC-->>U: "Snapshot created: abc12def...\n3 files tracked, 1 new"
```

---

## 4. Sequence — Restore Snapshot

```mermaid
sequenceDiagram
    participant U as Developer
    participant RC as RestoreCommand
    participant SM as SnapshotManager
    participant RE as RestoreEngine
    participant OBJ as ObjectStorage
    participant FS as Filesystem

    U->>RC: minigitree restore abc12def
    RC->>SM: get_snapshot("abc12def")
    SM->>SM: find_snapshot_by_prefix("abc12def")
    SM-->>RC: Snapshot{tree_hash, message, ...}

    RC->>RE: restore_snapshot(snapshot_hash)
    RE->>SM: get_snapshot(snapshot_hash)
    SM-->>RE: Snapshot

    RE->>RE: restore_tree(root_tree_hash, root_path)

    loop Recursively for each tree entry
        RE->>OBJ: read_tree(tree_hash)
        OBJ-->>RE: Tree{entries}
        alt Entry is BLOB
            RE->>OBJ: read_blob(hash)
            OBJ-->>RE: file content
            RE->>FS: write_file(path, content)
        else Entry is TREE (subdirectory)
            RE->>FS: create_directories(subdir_path)
            RE->>RE: restore_tree(subtree_hash, subdir_path)
        end
    end

    RE-->>RC: files_restored count
    RC-->>U: "5 files restored"
```

---

## 5. Class Diagram — Domain Models

```mermaid
classDiagram
    class Blob {
        -vector~uint8_t~ content_
        -string hash_
        +compute_hash() void
        +serialize() vector~uint8_t~
        +deserialize(bytes) Blob$
        +hash() string
        +content() vector~uint8_t~
    }

    class TreeEntry {
        +Type type
        +string hash
        +string name
    }

    class Tree {
        -vector~TreeEntry~ entries_
        -string hash_
        +add_entry(entry) void
        +sort_entries() void
        +compute_hash() void
        +serialize() string
        +deserialize(content) Tree$
        +hash() string
        +entries() vector~TreeEntry~
    }

    class Snapshot {
        -string tree_hash_
        -string message_
        -optional~time_t~ timestamp_
        -string hash_
        +compute_hash() void
        +serialize() string
        +deserialize(content) Snapshot$
        +hash() string
        +tree_hash() string
        +message() string
        +timestamp() time_t
    }

    Tree "1" *-- "*" TreeEntry : contains
    TreeEntry --> Blob : references (BLOB type)
    TreeEntry --> Tree : references (TREE type)
    Snapshot --> Tree : points to root
```

---

## 6. Flowchart — Object Storage Path Resolution

```mermaid
flowchart TD
    H[SHA-256 Hash\ne.g. 3ab4f5c7e2d1...]
    P[Extract prefix\nfirst 2 chars: '3a']
    S[Extract suffix\nremaining: 'b4f5c7e2d1...']
    PATH[".minigitree/objects/3a/b4f5c7e2d1..."]
    CHECK{Object exists?}
    REUSE[Reuse - deduplication]
    WRITE[Write object content to path]

    H --> P & S
    P & S --> PATH
    PATH --> CHECK
    CHECK -->|Yes| REUSE
    CHECK -->|No| WRITE
```

---

## 7. State Diagram — Repository Lifecycle

```mermaid
stateDiagram-v2
    [*] --> Uninitialized : cwd has no .minigitree/

    Uninitialized --> Initialized : minigitree init
    Initialized --> HasSnapshots : minigitree snapshot "..."
    HasSnapshots --> HasSnapshots : minigitree snapshot "..."

    HasSnapshots --> Restored : minigitree restore <hash>
    Restored --> HasSnapshots : minigitree snapshot "..."

    state Initialized {
        [*] --> ObjectsDir
        [*] --> SnapshotsDir
        [*] --> Config
    }

    state HasSnapshots {
        [*] --> RootTree
        RootTree --> SubTrees : recursive
        RootTree --> Blobs
        SubTrees --> Blobs
    }
```

---

## 8. ER Diagram — On-Disk Data Model

```mermaid
erDiagram
    SNAPSHOT ||--|| TREE : "points to root"
    TREE ||--|{ TREE_ENTRY : contains
    TREE_ENTRY }o--|| BLOB : "references (type=blob)"
    TREE_ENTRY }o--|| TREE : "references (type=tree)"

    SNAPSHOT {
        string hash PK
        string tree_hash FK
        string message
        int timestamp
    }

    TREE {
        string hash PK
        string serialized_entries
    }

    TREE_ENTRY {
        string parent_tree_hash FK
        string type "blob or tree"
        string child_hash FK
        string name
    }

    BLOB {
        string hash PK
        bytes content
    }
```
