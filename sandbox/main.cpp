#include "core_memory.hpp"
#include "core_window.hpp"

#include <iostream>

int main()
{
    Core::Memory window=nullptr;
    Core::Window_Create(&window);
    char znak;
    while(Core::Window_Is_Alive(&window))
    {
        std::cin>>znak;
        if(znak=='x')
        {
            Core::Window_Destroy(&window);
        }
    }
    std::cin.get();
    Core::Window_Destroy(&window);

    //###########_THE_####_END_##########//
    std::cin.get();
}