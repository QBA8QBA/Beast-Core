/*#include "Qwindow.h"

#include <atomic>
#include <Windows.h>
#include <mutex>
#include <thread>

#pragma region Declarations
inline LRESULT CALLBACK _Win_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
inline void _Thread_Work(uint32 width,uint32 height,const char* name);
inline void _Present_Buffers();
inline void _Resize_Buffers(uint32 width,uint32 height);
inline void _Input(UINT msg,WPARAM wParam,LPARAM lParam);
#pragma endregion





#pragma region Local_Variables
static std::thread Thread;
static std::mutex buffer_lock;

static struct
{
    void* primary_buffer;
    void* secondary_buffer;
    uint32 width;
    uint32 height;
    const int8 data_size=4;
    int8 isDrawable;
}Buffers={0};

static struct
{
    const char* name;
    HWND memory;
    uint32 width;
    uint32 height;
    std::atomic<int8> isRuning;
}Window={0};

static struct
{
    std::atomic<int8> key_w;
    std::atomic<int8> key_a;
    std::atomic<int8> key_s;
    std::atomic<int8> key_d;
}Keyboard={0};
#pragma endregion





#pragma region Functions
void SpawnWindow(uint32 width,uint32 height,const char* name)
{
    //TODO:for now thread runing = window runing
    Window.isRuning.store(1);
    Thread=std::thread(_Thread_Work,width,height,name);
    Thread.detach();
    return;
}

void DespawnWindow()
{
    Window.isRuning.store(0);
    return;
}

int8 IsWindowWorking()
{
    return Window.isRuning.load();
}

int8 IsKeyPresed(uint8 key_id)
{
    int8 state=0;
    switch(key_id)
    {
        case 'W':
        {
            state=Keyboard.key_w.load();
        }
        break;
        case 'A':
        {
            state=Keyboard.key_a.load();
        }
        break;
        case 'S':
        {
            state=Keyboard.key_s.load();
        }
        break;
        case 'D':
        {
            state=Keyboard.key_d.load();
        }
        break;
    }
    return state;
}

void WindowDrawBuffer(Drawable& obj)
{
    std::lock_guard<std::mutex> guard(buffer_lock);

    obj.Draw(Buffers.secondary_buffer,Buffers.width,Buffers.height,Buffers.data_size);

    return;
}

void WindowSwapBuffer()
{
    std::lock_guard<std::mutex> guard(buffer_lock);

    void* swaper=Buffers.primary_buffer;
    Buffers.primary_buffer=Buffers.secondary_buffer;
    Buffers.secondary_buffer=swaper;
    swaper=0;

    return;
}
#pragma endregion





#pragma region Inlines
inline LRESULT CALLBACK _Win_Proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    switch (msg)
    {
        case WM_CLOSE:
        {
            Window.isRuning.store(0);
            PostQuitMessage(0);
            return 0;
        }
        break;
        case WM_MOVE:
        {
            RECT rect;
            GetClientRect(Window.memory,&rect);
            if(((rect.bottom-rect.top)<=0)||((rect.right-rect.left)<=0)){Buffers.isDrawable=0;}
            else{Buffers.isDrawable=1;}
        }
        break;
        default:
        {
            return DefWindowProc(hwnd,msg,wParam,lParam);
        }
        break;
    }
    return 0;
}

inline void _Thread_Work(uint32 width,uint32 height,const char* name)
{
    WNDCLASSA wc={0};
    wc.hInstance=0;
    wc.lpfnWndProc=_Win_Proc;
    wc.lpszClassName="qlib_class";
    RegisterClassA(&wc);


    Window.memory=CreateWindowA
    (
        "qlib_class",
        name,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        69,69,
        width,height,
        0,
        0,
        0,
        0
    );
    Window.width=width;
    Window.height=height;
    Window.isRuning.store(1);

    _Resize_Buffers(width,height);
    Buffers.isDrawable=1;

    ShowWindow(Window.memory,1);
    

    MSG msg={0};
    while(Window.isRuning.load()){
        msg={0};
        while(PeekMessageA(&msg,0,0,0,PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
            _Input(msg.message,msg.wParam,msg.lParam);
        }
        if(Buffers.isDrawable){_Present_Buffers();}

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    if(Window.memory){DestroyWindow(Window.memory);}
    if(Buffers.primary_buffer){VirtualFree(Buffers.primary_buffer,0,MEM_RELEASE);}
    if(Buffers.secondary_buffer){VirtualFree(Buffers.secondary_buffer,0,MEM_RELEASE);}

    return;
}

inline void _Present_Buffers()
{
    std::lock_guard<std::mutex> guard(buffer_lock);

    HDC hdc=GetDC(Window.memory);
    BITMAPINFO bmih={0};
    bmih.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    bmih.bmiHeader.biWidth=Window.width;
    bmih.bmiHeader.biHeight=-(Window.height);
    bmih.bmiHeader.biPlanes=1;
    bmih.bmiHeader.biBitCount=32;
    bmih.bmiHeader.biCompression=BI_RGB;
    StretchDIBits(
        hdc,
        0,0,Window.width,Window.height,
        0,0,Buffers.width,Buffers.height,
        Buffers.primary_buffer,
        &bmih,
        DIB_RGB_COLORS,
        SRCCOPY
    );

    ReleaseDC(Window.memory,hdc);
    return;
}

inline void _Resize_Buffers(uint32 width,uint32 height)
{
    std::lock_guard<std::mutex> guard(buffer_lock);

    if(Buffers.primary_buffer){VirtualFree(Buffers.primary_buffer,0,MEM_RELEASE);}
    if(Buffers.secondary_buffer){VirtualFree(Buffers.secondary_buffer,0,MEM_RELEASE);}

    Buffers.width=width;
    Buffers.height=height;

    Buffers.primary_buffer=VirtualAlloc
    (
        0,
        Buffers.width*Buffers.height*Buffers.data_size,
        MEM_COMMIT,
        PAGE_READWRITE
    );

    Buffers.secondary_buffer=VirtualAlloc
    (
        0,
        Buffers.width*Buffers.height*Buffers.data_size,
        MEM_COMMIT,
        PAGE_READWRITE
    );

    return;
}

inline void _Input(UINT msg,WPARAM wParam,LPARAM lParam)
{
    if(msg==WM_KEYDOWN)
    {
        switch (wParam)
        {
            case 'W':
            {
                Keyboard.key_w.store(1);
            }
            break;
            case 'A':
            {
                Keyboard.key_a.store(1);
            }
            break;
            case 'S':
            {
                Keyboard.key_s.store(1);
            }
            break;
            case 'D':
            {
                Keyboard.key_d.store(1);
            }
            break;
        }
    }
    else if(msg==WM_KEYUP)
    {
        switch (wParam)
        {
            case 'W':
            {
                Keyboard.key_w.store(0);
            }
            break;
            case 'A':
            {
                Keyboard.key_a.store(0);
            }
            break;
            case 'S':
            {
                Keyboard.key_s.store(0);
            }
            break;
            case 'D':
            {
                Keyboard.key_d.store(0);
            }
            break;
        }
    }
    return;
}
#pragma endregion*/