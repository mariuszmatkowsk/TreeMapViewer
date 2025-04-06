#include <print>
#include <filesystem>

#include <raylib.h>

#include "TreeMap.hpp"

constexpr int WIDTH                = 800;
constexpr int HEIGHT               = 600;
constexpr const char* WINDOW_TITLE = "TreeMapViewer";
constexpr Color BACGROUND_COLOR    = Color{.r = 28, .g = 28, .b = 28, .a = 1};

int main(int argc, char** argv) {

    InitWindow(WIDTH, HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

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

    std::println("Root size: {}", tree_map.root->size);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BACGROUND_COLOR);

        DrawLine(0, HEIGHT - 30, WIDTH, HEIGHT - 30, WHITE);
        DrawText("File name", 5, HEIGHT - 25, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
