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

#include <utility>

#include "boost/assert.hpp"

#include "Silicon/Localization.hpp"
#include "Silicon/Log.hpp"
#include "Silicon/Silicon.hpp"

#include "Modules.hpp"

namespace {

std::function<bool()> s_loop;

}

namespace Si {

bool Initialize()
{
    if (!Si::AddLocalizationFile(Locale::en_US, "localizations/Silicon_en_US.plist"))
    {
        return false;
    }

    Si::SetLocale(Locale::en_US);

    Engine::Debug(Si::GetLocalized("Initializing Modules!"));
    InitializeModules();
    Engine::Debug(Si::GetLocalized("Initialized Modules!"));

    return true;
}

void Deinitialize()
{
    Engine::Debug(Si::GetLocalized("Deinitializing Modules!"));
    DeinitializeModules();
    Engine::Debug(Si::GetLocalized("Deinitialized Modules!"));
}

void SetLoop(std::function<bool()> loop)
{
    s_loop = std::move(loop);
}

bool Loop()
{
    BOOST_ASSERT_MSG(s_loop, Si::GetLocalized("Loop function not defined! Make sure you set one with Si::SetLoop()").c_str());

    return s_loop();
}
} // namespace Engine
