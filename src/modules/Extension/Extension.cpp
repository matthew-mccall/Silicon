#include "Silicon/Silicon.hpp"

#include "Extension.hpp"
    
namespace Extension
{
    void Initialize()
    {
        Si::Print("Initialized Extension!");
    }

    void Deinitialize()
    {
        Si::Print("Deinitalized Extension!");
    }
} // namespace Extension
