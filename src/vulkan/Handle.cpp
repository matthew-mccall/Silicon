/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2022 Matthew McCall
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Handle.hpp"

namespace Si::Vulkan {

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

HandleBase::HandleBase(const HandleBase &other)
    : m_mutex()
{
    for (HandleBase *dependency : other.m_dependencies) {
        addDependency(*dependency);
    }
}

void HandleBase::create()
{
    Vector<std::reference_wrapper<HandleBase>> destroyedDependents;
    destroyedDependents.reserve(m_dependents.size());

    for (HandleBase *dependent : m_dependents) {
        if (dependent->isCreated()) {
            destroyedDependents.emplace_back(*dependent);
        }
    }

    this->destroy();

    for (HandleBase *dependency : m_dependencies) {
        if (!dependency->isCreated()) {
            dependency->create();
        }
    }

    assert(!m_created); // If this has been created during dependency creation we may have a loop.
    m_created = this->createImpl();

    for (HandleBase &destroyedDependent : destroyedDependents) {
        destroyedDependent.create();
    }
}

void HandleBase::destroy()
{
    for (HandleBase *dependent : m_dependents) {
        dependent->destroy();
    }

    if (m_created) {
        this->destroyImpl();
    }

    m_created = false;
}

#pragma clang diagnostic pop

bool HandleBase::isCreated() const
{
    return m_created;
}

void HandleBase::addDependent(HandleBase &handle)
{
    m_dependents.emplace_back(&handle);
}

void HandleBase::removeDependent(HandleBase &handle)
{
    auto i = std::find_if(m_dependents.begin(), m_dependents.end(), [&handle](auto other) {
        return &handle == other;
    });

    if (i != m_dependents.end()) {
        m_dependents.erase(i);
    }
}

void HandleBase::addDependency(HandleBase &handle)
{
    handle.addDependent(*this);
    m_dependencies.emplace_back(&handle);
}

void HandleBase::removeDependency(HandleBase &handle)
{
    handle.removeDependent(*this);

    auto i = std::find_if(m_dependencies.begin(), m_dependencies.end(), [&handle](auto other) {
        return other == &handle;
    });

    if (i != m_dependencies.end()) {
        m_dependencies.erase(i);
    }
}

HandleBase::~HandleBase()
{
    while (m_dependencies.begin() != m_dependencies.end()) {
        removeDependency(*m_dependencies.front());
    }
}

HandleBase &HandleBase::operator=(const HandleBase &other)
{
    for (HandleBase *dependency : other.m_dependencies) {
        addDependency(*dependency);
    }

    return *this;
}
std::recursive_mutex &HandleBase::getMutex()
{
    return m_mutex;
}

} // namespace Si::graphics
