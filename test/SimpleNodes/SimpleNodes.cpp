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

#include <utility>

#include "Silicon/Log.hpp"
#include "Silicon/Node.hpp"
#include "Silicon/Silicon.hpp"

class MessageNode : public Si::Node
{
public:
    explicit MessageNode(std::string message)
        : m_message(std::move(message))
    {
    }

    void printMessage() const
    {
        Si::Info(m_message);

        for (const Node &i : *this) {
            dynamic_cast<const MessageNode &>(i).printMessage();
        }
    }

private:
    std::string m_message;
};

int main(int argc, char **argv)
{
    if (!Si::Initialize()) {
        return EXIT_FAILURE;
    }
    {
        MessageNode root {"Root Node"};
        MessageNode nodeA {"Node A"};
        MessageNode nodeB {"Node B"};
        MessageNode nodeC {"Node C"};

        root.addChild(nodeA);
        root.addChild(nodeB);

        nodeA.addChild(nodeC);

        root.printMessage();
    }

    Si::Deinitialize();
}