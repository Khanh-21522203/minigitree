cmake_minimum_required(VERSION 4.1)
project(minigitree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(minigitree
        src/main.cpp
        src/utils/hash_utils.h
        src/utils/hash_utils.cpp
        src/utils/filesystem.h
        src/utils/filesystem.cpp
        src/utils/string_utils.h
        src/utils/string_utils.cpp
        src/models/blob.h
        src/models/blob.cpp
        src/models/tree.h
        src/models/tree.cpp
        src/models/snapshot.cpp
        src/models/snapshot.h
        src/core/repository.h
        src/core/repository.cpp
        src/core/object_storage.h
        src/core/object_storage.cpp
        src/core/tree_builder.h
        src/core/tree_builder.cpp
        src/core/snapshot_manager.h
        src/core/snapshot_manager.cpp
        src/core/restore_engine.h
        src/core/restore_engine.cpp
        src/cli/command_handler.h
        src/cli/command_handler.cpp
        src/cli/init_command.h
        src/cli/init_command.cpp
        src/cli/snapshot_command.h
        src/cli/snapshot_command.cpp
        src/cli/list_command.h
        src/cli/list_command.cpp
        src/cli/show_command.h
        src/cli/show_command.cpp
        src/cli/restore_command.h
        src/cli/restore_command.cpp
)

target_include_directories(minigitree PRIVATE ${CMAKE_SOURCE_DIR}/src)

find_package(OpenSSL REQUIRED)
target_link_libraries(minigitree PRIVATE OpenSSL::Crypto OpenSSL::SSL)

install(TARGETS minigitree DESTINATION bin)
