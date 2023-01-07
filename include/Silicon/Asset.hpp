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
// Created by Matthew McCall on 1/6/23.
//

#ifndef SILICON_ASSET_HPP
#define SILICON_ASSET_HPP

#include <cstdint>
#include <memory>
#include <string>

#include "Silicon/Async.hpp"
#include "Silicon/Log.hpp"
#include "Silicon/Types.hpp"

namespace Si {

class Asset
{
public:
    template <typename T>
    static std::shared_ptr<T> GetNow(const std::string &asset_path)
    {
        if (auto asset = s_loadedAssets.find(asset_path); asset != s_loadedAssets.end())
        {
            if (auto asset_ptr = asset->second.lock())
            {
                return std::static_pointer_cast<T>(asset_ptr);
            }
            else
            {
                Engine::Trace("Cache miss for asset: {}", asset_path);
            }
        }

        auto asset = std::make_shared<T>(asset_path);
        s_loadedAssets[asset_path] = asset;
        return asset;
    }

    template <typename T>
    static Future<std::shared_ptr<T>> Get(const std::string &asset_path)
    {
        return Async<std::shared_ptr<T>>([asset_path]() {
            return GetNow<T>(asset_path);
        });
    }

    [[nodiscard]] const std::string &GetPath() const;
    Vector<std::uint8_t> &GetBytes();

    virtual ~Asset();

protected:
    explicit Asset(std::string path);

    Vector<std::uint8_t> m_data;

private:
    std::string m_path;

    static Si::HashMap<std::string, std::weak_ptr<Si::Asset>> s_loadedAssets;

};

class PlainTextAsset : public Asset
{
public:
    explicit PlainTextAsset(std::string path);

    [[nodiscard]] const std::string &GetText() const;

private:
    std::string m_text;

};

}

#endif // SILICON_ASSET_HPP
