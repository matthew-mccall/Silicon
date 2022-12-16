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
// Created by Matthew McCall on 10/9/22.
//

#ifndef SILICON_NODE_HPP
#define SILICON_NODE_HPP

#include <initializer_list>

#include "Types.hpp"

namespace Si {

class Node;

using NodeGraph = Graph<NotNull<Node *>, GraphList>;

/*
 * A Node is a base class for all objects that can be added to a NodeGraph.
 */
class Node
{
public:

    class ChildIterator
    {
    public:
        explicit ChildIterator(NodeGraph::adjacency_iterator);

        Node& operator*();
        Node* operator->();

        ChildIterator& operator++();
        ChildIterator& operator--();
        const ChildIterator operator++(int);
        const ChildIterator operator--(int);

        bool operator==(const ChildIterator &other);
        bool operator!=(const ChildIterator &other);

    private:
        NodeGraph::adjacency_iterator m_itr;
    };

    /**
     * @brief Construct a new Node object
     */
    Node();
    Node(const Node &) = delete;
    Node(Node &&) = delete;

    /**
     * @brief Create a new Node object from a list of children
     */
    Node(std::initializer_list<Node *> children);

    /**
     * Add a child to this node
     *
     * @param node The child to add
     */
    void addChild(NotNull<Node *>);

    /**
     * Add a child to this node
     *
     * @param node The child to add
     */
    void addChild(Node &);

    /**
     * Add a list of children to this node
     *
     * @param children The children to add
     */
    Node &addChildren(std::initializer_list<Node *> children);

    /**
     * Gets the begin iterator for the children of this node
     * @return The begin iterator
     */
    [[nodiscard]] ChildIterator begin() const;

    /**
     * Gets the end iterator for the children of this node
     * @return The end iterator
     */
    [[nodiscard]] ChildIterator end() const;

    /**
     * Destroys this node.
     */
    virtual ~Node();

protected:
    unsigned m_id = 0;
    NodeGraph::vertex_descriptor m_graphDescriptor;

    static unsigned s_currentID;
    static NodeGraph s_graph;
};

}

#endif //SILICON_NODE_HPP
