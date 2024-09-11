#include "core_window.hpp"

#include "core_memory.hpp"

#include <atomic>
#include <mutex>
#include <windows.h>
#include <thread>

struct window
{
    std::thread window_thread;
    std::atomic<bool8> window_alive;
    std::mutex window_mutex;
    HWND window_hwnd;
};

void window_proces(window** window_ptr);

void Core::Window_Create(void** void_ptr)
{
    if((*void_ptr)!=nullptr){return;}
    Core::Memory_Allocate(void_ptr,sizeof(window));
    window* window_ptr=(window*)(*void_ptr);
    window_ptr->window_alive.store(1);
    window_ptr->window_thread=std::thread(window_proces,&window_ptr);
    window_ptr->window_thread.detach();
    return;
}

void Core::Window_Destroy(void** void_ptr)
{
    if((*void_ptr)==nullptr){return;}
    window* window_ptr=(window*)(*void_ptr);
    window_ptr->window_alive.store(0);
    (*void_ptr)=nullptr;
    return;
}

bool8 Core::Window_Is_Alive(void** void_ptr)
{
    if((*void_ptr)==nullptr){return false;}
    window* window_ptr=(window*)(*void_ptr);
    return window_ptr->window_alive.load();
}

void window_proces(window** window_ptr)
{
    WNDCLASSA wc={};
    wc.hInstance=0;
    wc.lpszClassName="window_class";
    wc.lpfnWndProc=DefWindowProcA;
    RegisterClassA(&wc);

    (*window_ptr)->window_hwnd=CreateWindowA
    (
        "window_class",
        "name",
        WS_OVERLAPPEDWINDOW,
        69,69,
        420,420,
        0,
        0,
        0,
        0
    );

    ShowWindow((*window_ptr)->window_hwnd,1);

    MSG msg={};
    while((*window_ptr)->window_alive.load())
    {
        while(PeekMessageA(&msg,(*window_ptr)->window_hwnd,0,0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);   
        }
    }

    DestroyWindow((*window_ptr)->window_hwnd);
    Core::Memory_Release((void**)(window_ptr));
    return;
}