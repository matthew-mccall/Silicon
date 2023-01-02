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

#if !defined(SI_VULKAN_HANDLE_HPP)
#define SI_VULKAN_HANDLE_HPP

/**
 * @file Handle.hpp
 *
 * @brief Contains the declarations for the Handle classes which form the basis of our Vulkan implementation.
 *
 * Credit to <a href="https://github.com/MarcasRealAccount/">Markus</a> for the Handle system.
 */

#include <functional>
#include <mutex>

#include "Silicon/Types.hpp"

namespace Si::Vulkan {

/**
 * @namespace Si::graphics
 *
 * @brief A base for a system that allows handles to be attached to other handles and recreating handles and their dependents.
 */
class HandleBase
{
public:
    HandleBase() = default;
    HandleBase(const HandleBase &other);

    /**
     * @brief Creates a handle.
     *
     * If a handle is is created, it will be destroyed, including its dependents, then recreated, including its dependents.
     */
    void create();

    /**
     * Destroys a handle, including its dependents.
     */
    void destroy();

    /**
     * Gets whether is handle was created.
     *
     * @return Whether is handle was created.
     */
    [[nodiscard]] bool isCreated() const;

    /**
     * Adds a handle to its dependents list.
     *
     * @param handle The handle to add to its dependents list
     */
    void addDependent(HandleBase &handle);

    /**
     * Removes a handle from its dependents list.
     *
     * @param handle The handle to remove from its dependents list.
     */
    void removeDependent(HandleBase &handle);

    [[nodiscard]] std::recursive_mutex &getMutex();

    virtual ~HandleBase();

    HandleBase &operator=(const HandleBase &other);

protected:
    virtual bool createImpl() = 0;

    void addDependency(HandleBase &handle);
    void removeDependency(HandleBase &handle);
    virtual void destroyImpl() = 0;

private:
    bool m_created = false;
    std::recursive_mutex m_mutex;

private:
    Vector<NotNull<HandleBase *>> m_dependents;
    Vector<NotNull<HandleBase *>> m_dependencies;
};

/**
 * A template wrapper that allows any type to be treated as handle.
 *
 * @tparam T The handle type.
 */
template <typename T>
class Handle : public HandleBase
{
public:
    /**
     * Gets the handle.
     *
     * @return The handle.
     */
    T getHandle()
    {
        return m_handle;
    }

    T *operator->()
    {
        if (!isCreated())
            create();
        return &m_handle;
    }

    T &operator*()
    {
        if (!isCreated())
            create();
        return m_handle;
    }

protected:
    T m_handle;
};

} // namespace Si::graphics

#endif
