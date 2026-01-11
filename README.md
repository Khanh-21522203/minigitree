# MiniGitree

A lightweight version control system inspired by Git, built to demonstrate how Merkle Trees work in real systems.

## Overview

MiniGitree is an educational project that implements a minimal version control system using content-addressable storage and Merkle Trees. It allows you to snapshot your working directory, track changes through cryptographic hashes, and restore previous states—all without the complexity of branching, merging, or remote repositories.

## Motivation

Understanding Git's internals can be challenging. MiniGitree strips away the complexity to focus on the core concepts:

- **Content-addressable storage**: Files and directories are identified by their SHA-256 hash
- **Merkle Trees**: Hierarchical hash structures that efficiently detect changes
- **Immutable snapshots**: Each commit is a complete, verifiable state of your project
- **Hash reuse**: Unchanged files share the same hash, saving storage and computation

This project is ideal for:
- Learning how version control systems work under the hood
- Understanding Merkle Tree applications in real systems
- Exploring C++ systems programming
- Building a foundation before diving into Git's source code

## High-Level Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    Working Directory                     │
│                  (User's actual files)                   │
└────────────────────────┬────────────────────────────────┘
                         │
                         │ snapshot
                         ▼
┌─────────────────────────────────────────────────────────┐
│                   MiniGitree Engine                      │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐  │
│  │ Tree Builder │→ │ Hash Computer│→ │Object Storage│  │
│  └──────────────┘  └──────────────┘  └──────────────┘  │
└────────────────────────┬────────────────────────────────┘
                         │
                         ▼
┌─────────────────────────────────────────────────────────┐
│              .minigitree/ Repository                     │
│  ┌──────────┐  ┌──────────┐  ┌──────────────────────┐  │
│  │ objects/ │  │snapshots/│  │ config               │  │
│  │ (blobs,  │  │ (commits)│  │                      │  │
│  │  trees)  │  │          │  │                      │  │
│  └──────────┘  └──────────┘  └──────────────────────┘  │
└─────────────────────────────────────────────────────────┘
```

### Core Components

1. **Tree Builder**: Recursively scans the working directory and builds a Merkle Tree
2. **Hash Computer**: Computes SHA-256 hashes for files and directories
3. **Object Storage**: Stores blobs and trees in content-addressable format
4. **Snapshot Manager**: Creates and manages snapshots (commits)
5. **Restore Engine**: Reconstructs working directory from a snapshot