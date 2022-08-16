#include <iostream>

#include "Core/Core.hpp"

#include "Modules.hpp"

namespace Engine
{
    bool Initialize()
    {
        Print("Initializing Modules!");
        InitializeModules();
        Print("Initialized Modules!");
        
        return true;
    }

    void Print(const std::string &message)
    {
        std::cout << message << '\n';
    }

    void Deinitialize()
    {
        Print("Deinitializing Modules!");
        DeinitializeModules();
        Print("Deinitialized Modules!");
    }
} // namespace Engine
