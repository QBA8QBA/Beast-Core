#include "core_memory.hpp"

#include <memoryapi.h>

void Core::Memory_Allocate(void** void_ptr,uint64 size)
{
    if((*void_ptr)!=nullptr){return;}
    *void_ptr=VirtualAlloc(0,size,MEM_COMMIT,PAGE_READWRITE);
    return;
}

void Core::Memory_Release(void** void_ptr)
{
    if((*void_ptr)==nullptr){return;}
    VirtualFree(*void_ptr,0,MEM_RELEASE);
    *void_ptr=nullptr;
    return;
}

void Core::Memory_Copy(void** void_ptr1,void** void_ptr2,uint64 size)
{
    if((*void_ptr1)!=nullptr){return;}
    if((*void_ptr2)==nullptr){return;}
    Core::Memory_Allocate(void_ptr1,size);
    char* index1=(char*)void_ptr1;
    char* index2=(char*)void_ptr2;
    for(uint64 i=0;i<size;i++)
    {
        *index1++=*index2++;
    }
    return;
}

void Core::Memory_Zero(void** void_ptr,uint64 size)
{
    if((*void_ptr)==nullptr){return;}
    char* index=(char*)void_ptr;
    for(uint64 i=0;i<size;i++)
    {
        *index++=0;
    }
    return;
}