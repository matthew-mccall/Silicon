#include "Core/Core.hpp"

#include "Extension.hpp"
    
namespace Extension
{
    void Initialize()
    {
        Engine::Print("Initialized Extension!");
    }

    void Deinitialize()
    {
        Engine::Print("Deinitalized Extension!");
    }
} // namespace Extension
