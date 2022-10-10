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

#include "Silicon/Node.hpp"

namespace Si
{

unsigned Node::s_currentID = 0;
NodeGraph Node::s_graph;

Node::Node()
    : m_id(s_currentID++)
    , m_graphDescriptor(boost::add_vertex(NotNull<Node*>(this), s_graph))
{
}

void Node::addChild(NotNull<Node*> node)
{
    boost::add_edge(m_graphDescriptor, node->m_graphDescriptor, s_graph);
    node->m_parent = this;
}

void Node::addChild(Node& node)
{
    addChild(NotNull<Node*>(&node));
}

Node::~Node()
{
    for (Node& i : *this)
    {
        i.m_parent = nullptr;
    }

    boost::clear_vertex(m_graphDescriptor, s_graph);
    boost::remove_vertex(m_graphDescriptor, s_graph);
}

Node::ChildIterator Node::begin() const
{
    auto [begin, end] = boost::adjacent_vertices(m_graphDescriptor, s_graph);
    return Node::ChildIterator { begin };
}

Node::ChildIterator Node::end() const
{
    auto [begin, end] = boost::adjacent_vertices(m_graphDescriptor, s_graph);
    return Node::ChildIterator { end };
}

Node::ChildIterator::ChildIterator(NodeGraph::adjacency_iterator itr)
: m_itr(itr) { }

Node& Node::ChildIterator::operator*()
{
    return *s_graph[static_cast<NodeGraph::vertex_descriptor>(*m_itr)];
}

Node* Node::ChildIterator::operator->()
{
    return s_graph[static_cast<NodeGraph::vertex_descriptor>(*m_itr)];
}
Node::ChildIterator& Node::ChildIterator::operator++()
{
    ++m_itr;
    return *this;
}
Node::ChildIterator& Node::ChildIterator::operator--()
{
    --m_itr;
    return *this;
}
Node::ChildIterator Node::ChildIterator::operator++(int)
{
    auto tmp = *this;
    ++*this;
    return tmp;
}
Node::ChildIterator Node::ChildIterator::operator--(int)
{
    auto tmp = *this;
    --*this;
    return tmp;
}
bool Node::ChildIterator::operator==(const ChildIterator& other)
{
    return m_itr == other.m_itr;
}
bool Node::ChildIterator::operator!=(const ChildIterator& other)
{
    return !(*this == other);
}

}

