// BSD 2-Clause License
//
// Copyright (c) 2022, Matthew McCall
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//
// Created by Matthew McCall on 8/19/22.
//

#ifndef SILICON_TYPES_HPP
#define SILICON_TYPES_HPP

#include <list>
#include <vector>

#include "boost/graph/adjacency_list.hpp"
#include "gsl/pointers"

#include "Allocator.hpp"

namespace Si {

template <typename T>
#ifdef NDEBUG
using NotNull = gsl::not_null<T>;
#else
using NotNull = gsl::strict_not_null<T>;
#endif

template <typename T>
using Vector = std::vector<T, Allocator<T>>;

template <typename T>
using List = std::list<T, Allocator<T>>;

/**
 * A STL map with a Silicon allocator.
 */
template <typename Key, typename Value, typename Compare = std::less<Key>>
using Map = std::map<Key, Value, Compare, Allocator<std::pair<const Key, Value>>>;

/**
 * A STL unordered map with a Silicon allocator.
 */
template <typename Key, typename Value, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
using HashMap = std::unordered_map<Key, Value, Hash, KeyEqual, Allocator<std::pair<const Key, Value>>>;

/**
 * Vector for use in Graphs.
 */
struct GraphVector {
};

/**
 * List for use in Graphs.
 */
struct GraphList {
};

}

/// @cond

namespace boost {

template <class T>
struct container_gen<Si::GraphVector, T> {
    typedef Si::Vector<T> type;
};

template <>
struct parallel_edge_traits<Si::GraphVector> {
    typedef allow_parallel_edge_tag type;
};

template <class T>
struct container_gen<Si::GraphList, T> {
    typedef Si::List<T> type;
};

template <>
struct parallel_edge_traits<Si::GraphList> {
    typedef allow_parallel_edge_tag type;
};

}

/// @endcond

namespace Si {

/**
 * Container for representing relationships between objects.
 */
template <typename T, typename ContainerT = GraphVector>
using Graph = boost::adjacency_list<ContainerT, ContainerT, boost::bidirectionalS, T>;
}

#endif // SILICON_TYPES_HPP
