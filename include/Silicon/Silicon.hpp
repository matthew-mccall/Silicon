#if !defined(ENGINE_CORE_HPP)
#define ENGINE_CORE_HPP

#include <string>

namespace Si
{
    bool Initialize();
    extern void Print(const std::string &message);
    void Deinitialize();
} // namespace Engine

#endif // ENGINE_CORE_HPP
