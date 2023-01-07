// BSD 2-Clause License
//
// Copyright (c) 2023, Matthew McCall
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
// Created by Matthew McCall on 1/7/23.
//

#include <utility>

#include "SDL_RWops.h"

#include "Silicon/Asset.hpp"

namespace Si {

const std::string &Asset::GetPath() const
{
    return m_path;
}

Vector<std::uint8_t> &Asset::GetBytes()
{
    return m_data;
}

Asset::Asset(std::string path)
    : m_path(std::move(path))
{
    Engine::Trace("Loading asset: {}", m_path);

    // Load entire file into m_data using SDL.
    SDL_RWops *file = SDL_RWFromFile(m_path.c_str(), "rb");
    if (!file)
    {
        Engine::Error("Failed to open file: {}", m_path);
        throw std::runtime_error("Failed to open file: " + m_path);
    }

    // Get file size.
    Sint64 size = SDL_RWsize(file);
    if (size < 0)
    {
        Engine::Error("Failed to get file size: {}", m_path);
        throw std::runtime_error("Failed to get file size: " + m_path);
    }

    // Read file into m_data.
    m_data.resize(static_cast<std::size_t>(size));
    std::size_t read = SDL_RWread(file, m_data.data(), 1, static_cast<std::size_t>(size));

    if (read != static_cast<std::size_t>(size))
    {
        Engine::Error("Failed to read file: {}", m_path);
        throw std::runtime_error("Failed to read file: " + m_path);
    }

    SDL_RWclose(file);
}

Asset::~Asset()
{
    Engine::Trace("Unloading asset: {}", m_path);
}

PlainTextAsset::PlainTextAsset(std::string path)
    : Asset(std::move(path))
{
    // Convert m_data to a string.
    m_text = std::string(m_data.begin(), m_data.end());
}

const std::string &PlainTextAsset::GetText() const
{
    return m_text;
}
}