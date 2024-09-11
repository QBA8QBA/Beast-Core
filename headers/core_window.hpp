#pragma once
#include "types.h"

namespace Core
{
    typedef void* Window;
    void Window_Create(void** empty_window);
    void Window_Destroy(void** window);
    bool8 Window_Is_Alive(void** window);
}