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

#include <array>
#include <memory>

#include "spdlog/sinks/ringbuffer_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "Silicon/Config.hpp"
#include "Silicon/Localization.hpp"
#include "Silicon/Log.hpp"

class LoggerManager
{
public:
    [[nodiscard]] static const LoggerManager &GetInstance()
    {
        static LoggerManager instance;
        return instance;
    }

    [[nodiscard]] const std::shared_ptr<spdlog::logger> &getEngineLogger() const
    {
        return s_engineLogger;
    }
    [[nodiscard]] const std::shared_ptr<spdlog::logger> &getClientLogger() const
    {
        return s_clientLogger;
    }

private:
    LoggerManager()
    {
        s_engineLogHistory = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(64);
        s_clientLogHistory = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(64);

        std::array<spdlog::sink_ptr, 2> engineSinks = {
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
            s_engineLogHistory};

        std::array<spdlog::sink_ptr, 2> clientSinks = {
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
            s_clientLogHistory};

        s_engineLogger = std::make_shared<spdlog::logger>(Si::GetLocalized("Engine"), engineSinks.begin(), engineSinks.end());
        s_clientLogger = std::make_shared<spdlog::logger>(Si::GetLocalized("Client"), clientSinks.begin(), clientSinks.end());

        if constexpr (SI_BUILD_CONFIG == Si::BuildConfig::Debug)
        {
            s_engineLogger->set_level(spdlog::level::debug);
            s_clientLogger->set_level(spdlog::level::debug);
        }

        s_engineLogger->set_pattern("%Y-%m-%dT%T [%n] %^%8l%$ %v");
        s_clientLogger->set_pattern("%Y-%m-%dT%T [%n] %^%8l%$ %v");

        s_engineLogHistory->set_pattern("%v");
        s_clientLogHistory->set_pattern("%v");
    }

    std::shared_ptr<spdlog::logger> s_engineLogger;
    std::shared_ptr<spdlog::logger> s_clientLogger;

    std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> s_engineLogHistory;
    std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> s_clientLogHistory;
};

namespace Si {

std::shared_ptr<spdlog::logger> Log::GetEngineLogger()
{
    return LoggerManager::GetInstance().getEngineLogger();
}

std::shared_ptr<spdlog::logger> Log::GetClientLogger()
{
    return LoggerManager::GetInstance().getClientLogger();
}

}
