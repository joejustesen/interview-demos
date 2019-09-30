#pragma once
#include "stdafx.h"
#include <unordered_map>
#include <vector>

using Indices = std::vector<unsigned int>;

class InMemoryIndex {
    struct Trie {
        using Leaves = std::unordered_map<char, std::unique_ptr<Trie>>;
        using Index = std::unordered_map<std::string, unsigned int>;

        Leaves  d_leaves;
        Index   d_index;
    };

    std::unique_ptr<Trie> d_head;
    Indices allLeaves(Trie * ptr) const;

public:
    InMemoryIndex();
    InMemoryIndex(InMemoryIndex && other);

    void insert(const char * str, unsigned int index);
    std::tuple<bool, Indices> search(const char * str) const;
};

void validateTrie(); 