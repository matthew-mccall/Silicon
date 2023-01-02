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
// Created by Matthew McCall on 8/20/22.
//

#ifndef SILICON_EVENT_HPP
#define SILICON_EVENT_HPP

#include <algorithm>
#include <cstdint>
#include <functional>

#include "SDL_events.h"

#include "Types.hpp"

namespace Si {
namespace Event {

    /**
     * Published when the Engine begins it's shutdown procedures.
     *
     * This is sent before the Engine begins detaching nodes.
     */
    struct AppQuit {};
    
    struct WindowResize {};
    
    void Process();

}

template <typename T>
void Pub(const T& data);

/**
 * The Subscriber class allows you to listen in on published events of type T.
 * @tparam T The type of event to listen to.
 */
template <typename T>
class Sub
{
    friend void Si::Pub<T>(const T& data);
    using Callback = std::function<void(const T&)>;

public:
    /**
     * Creates a new subscriber with the function to call on an event publish.
     * @param func The function to run when the event is called.
     */
    explicit Sub(Callback func)
        : Func(func)
    {
        Sub<T>::Subscribers.push_back(NotNull<Sub<T>*>(this));
    }

    ~Sub()
    {
        const auto& i = std::find(Sub<T>::Subscribers.begin(), Sub<T>::Subscribers.end(), this);

        if (i != Sub<T>::Subscribers.end())
            Sub<T>::Subscribers.erase(i);
    }

private:
    static std::vector<NotNull<Sub<T>*>> Subscribers;
    Callback Func;
};

/**
 * Publishes an event of type T to all subscribers immediately.
 * @tparam  T       The type of event to publish.
 * @param   data    An instance of the event to broadcast to all subscribers.
 */
template <class T>
void Pub(const T& data)
{
    for (Sub<T>* i : Sub<T>::Subscribers) {
        i->Func(data);
    }
}

template <typename T>
std::vector<NotNull<Sub<T>*>> Sub<T>::Subscribers;

}

#endif // SILICON_EVENT_HPP
