#include <print>
#include <filesystem>

#include "TreeMap.hpp"


int main(int argc, char** argv) {

    std::filesystem::path current_path{};

    if (argc == 2) {
        current_path = std::filesystem::path{argv[1]};
    } else {
        current_path = std::filesystem::current_path();
    }

    TreeMapBuilder tree_map_builder{current_path};

    auto tree_map = tree_map_builder.build();

    std::println("All leaves: ");
    unsigned leaves_count{};
    for (const auto& [node, is_leaf] : tree_map.leaves) {
        if (is_leaf) {
            std::println("{}", node->name);
            ++leaves_count;
        }
    }

    std::println("Leaves count: {}", leaves_count);

    for (const auto& node : tree_map.nodes) {
        std::println("{} size: {}", node->name, node->size);
    }

    return 0;
}
