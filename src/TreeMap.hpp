#pragma once

#include <cassert>
#include <filesystem>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>
#include <print>

// struct Vec2 {
//     int x{};
//     int y{};
// };
//
// struct Color {
//     uint8_t r{};
//     uint8_t g{};
//     uint8_t b{};
//     uint8_t a{};
// };
//
// struct DisplayTreeNode {   
//     Vec2  corner{};
//     Vec2  size{};
//     Color color{};
// };
//
// struct DisplayTreeMap {
//     std::vector<DisplayTreeNode> nodes{};
// };

struct TreeNode {
    using pointer = TreeNode*;

    std::string            name{};
    TreeNode*              parent{};
    std::vector<TreeNode*> childrens{};
    unsigned               size{};
    bool                   is_leaf{true};
    bool                   is_file{false};
};

struct TreeMap {
    TreeNode::pointer                           root{};
    std::vector<TreeNode::pointer>              nodes{};
    std::unordered_map<TreeNode::pointer, bool> leaves{};

    TreeNode::pointer add_node(std::string name, TreeNode::pointer parent, bool is_file) {
        if (nodes.empty()) {
            assert(!parent && "When adding first node there is no parent");
        } else {
            assert(parent && "Parent should be set when adding not first node");
        }

        auto node = new TreeNode{
            .name = std::move(name),
            .parent = parent,
            .childrens = {},
            .size = 0,
            .is_leaf = true,
            .is_file = is_file};

        if (nodes.empty()) {
            root = node;
        }

        nodes.push_back(node);

        if (parent) {
            parent->childrens.push_back(node);
            parent->is_leaf = false;
            leaves[parent] = false;
        } 
            
        leaves[node] = true;

        return node;
    }
};

class TreeMapBuilder {
    std::filesystem::path                              root_path_{};
    std::unordered_map<std::string, TreeNode::pointer> added_nods_{};

    void add_directory_node(TreeMap& tree_map, std::filesystem::path directory_path) {
        const auto parent_name = directory_path.parent_path().string(); 
        const auto name        = directory_path.string();
        const auto it_parent   = added_nods_.find(parent_name);

        TreeNode::pointer parent_node{nullptr};
        if (it_parent != std::end(added_nods_)) {
            parent_node = it_parent->second;
        } else {
            std::println("Parent was not found for: {}", name);
            parent_node = tree_map.root;
        }

        auto node = tree_map.add_node(name, parent_node, false);
        added_nods_.emplace(name, node);
        std::println("Directory node added: {}", name);
    }

    void add_file_node(TreeMap& tree_map, std::filesystem::path file_path) {
        const auto parent_name = file_path.parent_path().string();
        const auto name        = file_path.string();
        const auto it_parent   = added_nods_.find(parent_name);

        TreeNode::pointer parent_node{nullptr};
        if (it_parent != std::end(added_nods_)) {
            parent_node = it_parent->second;
        } else {
            std::println("Parent was not found for: {}", name);
        }

        auto node = tree_map.add_node(name, parent_node, true);
        
        const auto size = std::filesystem::file_size(file_path);
        node->size = size;

        std::println("File node added: {}", name);
    }

    void compute_sizes(TreeMap& tree_map) {
        for (const auto& node : std::ranges::views::reverse(tree_map.nodes)) {
            if (node->parent) {
                node->parent->size += node->size;
            }
        }
    }

public:
    explicit TreeMapBuilder(std::filesystem::path root_path)
        : root_path_{root_path} {}

    TreeMap build() {
        TreeMap tree_map{}; 

        auto node = tree_map.add_node(root_path_.string(), nullptr, true);
        added_nods_.emplace(root_path_.string(), node);

        for (const auto& entry : std::filesystem::recursive_directory_iterator(root_path_)) {
            entry.is_directory()
                ? add_directory_node(tree_map, entry.path())
                : add_file_node(tree_map, entry.path());
        } 

        compute_sizes(tree_map);

        return tree_map;
    }
};
