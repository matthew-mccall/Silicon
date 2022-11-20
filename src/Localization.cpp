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
// Created by Matthew McCall on 11/20/22.
//

#include <filesystem>
#include <unordered_map>
#include <vector>

#include "spdlog/fmt/fmt.h"

#include "boost/assert.hpp"

#include "tinyxml2.h"

#include "Silicon/Localization.hpp"

namespace {

std::unordered_map<Si::Locale, std::vector<std::filesystem::path>> s_localizationFiles;
std::unordered_map<std::string, std::string> s_localeTable;

}

namespace Si {

void SetLocale(Locale locale)
{
    for (const auto &filePath : s_localizationFiles[locale]) {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(filePath.c_str());

        BOOST_ASSERT_MSG(!doc.Error(), doc.ErrorStr());

        tinyxml2::XMLElement *plist = doc.FirstChildElement("plist");
        BOOST_ASSERT_MSG(plist, "plist not found in localization file!");

        tinyxml2::XMLElement *rootDict = plist->FirstChildElement("dict");
        BOOST_ASSERT_MSG(rootDict, "Dictionary not found in localization file!");

        tinyxml2::XMLElement *keyElement = rootDict->FirstChildElement("key");

        while (keyElement) {
            tinyxml2::XMLText *keyTextNode = keyElement->FirstChild()->ToText();
            tinyxml2::XMLElement *stringElement = keyElement->NextSiblingElement("string");

            BOOST_ASSERT_MSG(stringElement, fmt::format("String for key '{}' not found in localization file!", keyTextNode->Value()).c_str());

            tinyxml2::XMLText *stringTextNode = stringElement->FirstChild()->ToText();

            s_localeTable[keyTextNode->Value()] = stringTextNode->Value();

            keyElement = stringElement->NextSiblingElement("key");
        }
    }
}

std::string GetLocalized(const std::string &key)
{
    if (s_localeTable.find(key) == s_localeTable.end()) {
        return key;
    }

    return s_localeTable.at(key);
}

bool AddLocalizationFile(Locale locale, const std::string &filename)
{
    std::filesystem::path filePath {filename};

    if (!std::filesystem::exists(filePath)) {
        return false;
    }

    s_localizationFiles[locale].push_back(filePath);

    return true;
}

}
