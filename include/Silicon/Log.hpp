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

#ifndef SILICON_LOG_HPP
#define SILICON_LOG_HPP

#include "spdlog/logger.h"

namespace Si {

namespace Log {
    void Initialize();

    void Deinitialize();

    std::shared_ptr<spdlog::logger> GetEngineLogger();

    std::shared_ptr<spdlog::logger> GetClientLogger();
}

template <typename... Args>
void Trace(Args&&... args)
{
    Log::GetClientLogger()->trace(std::forward<Args>(args)...);
}

template <typename... Args>
void Info(Args&&... args)
{
    Log::GetClientLogger()->info(std::forward<Args>(args)...);
}

template <typename... Args>
void Debug(Args&&... args)
{
    Log::GetClientLogger()->debug(std::forward<Args>(args)...);
}

template <typename... Args>
void Warn(Args&&... args)
{
    Log::GetClientLogger()->warn(std::forward<Args>(args)...);
}

template <typename... Args>
void Error(Args&&... args)
{
    Log::GetClientLogger()->error(std::forward<Args>(args)...);
}

template <typename... Args>
void Critical(Args&&... args)
{
    Log::GetClientLogger()->critical(std::forward<Args>(args)...);
}

namespace Engine {
    template <typename... Args>
    void Trace(Args&&... args)
    {
        Log::GetEngineLogger()->trace(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Info(Args&&... args)
    {
        Log::GetEngineLogger()->info(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Debug(Args&&... args)
    {
        Log::GetEngineLogger()->debug(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Warn(Args&&... args)
    {
        Log::GetEngineLogger()->warn(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Error(Args&&... args)
    {
        Log::GetEngineLogger()->error(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Critical(Args&&... args)
    {
        Log::GetEngineLogger()->critical(std::forward<Args>(args)...);
    }
}
}

#endif // SILICON_LOG_HPP
