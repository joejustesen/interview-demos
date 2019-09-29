#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

using Indices = std::vector<uint>;

class InMemoryIndex {
    struct Trie {
        using Leaves = std::unordered_map<char, std::unique_ptr<Trie>>;
        using Index = std::unordered_map<std::string, uint>;

        Leaves  d_leaves;
        Index   d_index;
    };

    std::unique_ptr<Trie> d_head;
    Indices allLeaves(Trie * ptr);

public:
    InMemoryIndex();

    void insert(const char * str, uint index);
    auto search(const char * str);
};

void validateTrie();