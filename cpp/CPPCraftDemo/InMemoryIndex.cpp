#include "InMemoryIndex.h"
#include <stack>
#include <algorithm>
#include <iostream>
#include <cassert>


/****************************************************************************
****************************************************************************/
InMemoryIndex::InMemoryIndex() : 
    d_head(std::make_unique<Trie>())
{
}


InMemoryIndex::InMemoryIndex(InMemoryIndex && other) : d_head{std::move(other.d_head)}
{
}


/****************************************************************************
 * Insert a string into the Trie
****************************************************************************/
void InMemoryIndex::insert(const char * str, uint index)
{
	Trie * ptr = d_head.get();
	
    while (*str) {
        auto ch = *str;

        if (ptr->d_leaves.find(ch) == ptr->d_leaves.end()) {
            ptr->d_leaves[ch] = std::make_unique<Trie>();
        }

        ptr = ptr->d_leaves[ch].get();
		++str;
	}

    ptr->d_index[str] = index;
}

template<typename T, typename C>
void extractValues(const T & from, C & to) {
    std::transform(std::begin(from), std::end(from), std::back_inserter(to), [](const auto & el) { 
        return el.second; 
    });
}

template<typename M, typename S, typename FN>
void pushLeavesOntoStack(const M & map, S & stack, FN fn) {
    for (auto const & [first, second] : map) {
        stack.push(fn(second));
    }
}


/****************************************************************************
 * Return all indicies for the current leave and all leaves below the 
 * current leaf in the tree.
****************************************************************************/
Indices InMemoryIndex::allLeaves(Trie * ptr) const
{
    auto indices = Indices{};
    auto nodes = std::stack<Trie *, std::vector<Trie *>>{};

    if (!ptr) {
        return indices;
    }

    while (ptr) {
        extractValues(ptr->d_index, indices);

        pushLeavesOntoStack(ptr->d_leaves, nodes, [](const auto & value) { 
            return value.get(); 
        });

        if (!nodes.empty()) {
            ptr = nodes.top();
            nodes.pop();
        } else {
            ptr = nullptr;
        }
    }

    return indices;
}


/****************************************************************************
 * Return a list of indies that match the string.
****************************************************************************/
std::tuple<bool, Indices> InMemoryIndex::search(const char* str) const
{
	Trie * ptr = d_head.get();

	while (*str) {
        auto ch = *str;
    
		ptr = ptr->d_leaves[ch].get();
		if (ptr == nullptr)
			return std::make_tuple(false, Indices{});

		++str;
	}
	
    if (ptr->d_index.empty() && ptr->d_leaves.empty()) {
        return std::make_tuple(false, Indices{});
    }
    
    return std::make_tuple(true, allLeaves(ptr));

}


/****************************************************************************
 * unit test Trie
****************************************************************************/
void validateTrie()
{
    std::cout << "validating Trie functions\n";
    
    auto sut = InMemoryIndex{};

    sut.insert("test1", 34);
    sut.insert("g", 1223);
    sut.insert("test2", 583);
    sut.insert("test892", 349);

    {
        auto [found, results] = sut.search("not-found");

        if (found || results.size() != 0) {
            std::cout << "failure not-found\n";
        }
    }
    {
        auto [found, results] = sut.search("test1");

        if (!found || results.size() != 1 || results[0] != 34u) {
            std::cout << "failure 34\n";
        }
    }
    {
        auto [found, results] = sut.search("test2");

        if (!found || results.size() != 1 || results[0] != 583u) {
            std::cout << "failure 34\n";
        }
    }
    {
        auto [found, results] = sut.search("test");

        if (!found || results.size() != 3) {
            std::cout << "failure test\n";
        }

        if (std::find(std::begin(results), std::end(results), 34u) == results.end()) {
            std::cout << "failure test\n";
        }
        if (std::find(std::begin(results), std::end(results), 583u) == results.end()) {
            std::cout << "failure test\n";
        }
        if (std::find(std::begin(results), std::end(results), 349u) == results.end()) {
            std::cout << "failure test\n";
        }
    }
}