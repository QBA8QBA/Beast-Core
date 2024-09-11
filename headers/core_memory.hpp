#pragma once
#include "types.h"
#include <mutex>

namespace Core
{
    typedef void* Memory;
    void Memory_Allocate(void** empty_buffer,uint64 size_to_allocate);
    void Memory_Release(void** buffer);
    void Memory_Copy(void** empty_buffer,void** source_buffer,uint64 source_buffer_size);
    void Memory_Zero(void** buffer,uint64 buffer_size);
}