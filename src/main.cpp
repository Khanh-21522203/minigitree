#include "cli/command_handler.h"

int main(int argc, char* argv[]) {
    return minigitree::cli::CommandHandler::execute(argc, argv);
}