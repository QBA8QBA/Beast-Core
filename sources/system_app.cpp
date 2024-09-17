#include "system_app.hpp"

#include "system_event.hpp"

#include <windows.h>

inline LRESULT CALLBACK WindowProcedure(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

#define APP_SIZE WM_USER+1
#define APP_CLOSE WM_USER+2
#define APP_MOVE WM_USER+3
#define APP_SETFOCUS WM_USER+4
#define APP_KILLFOCUS WM_USER+5

struct App_core
{
    HWND hwnd;
    MSG msg;
};

App::App()
{
    this->app_core=new App_core;
    WNDCLASSA wc={};
    wc.hInstance=0;
    wc.lpszClassName="qlib_class";
    wc.lpfnWndProc=WindowProcedure;
    RegisterClassA(&wc);

    this->app_core->hwnd=CreateWindowA
    (
        "qlib_class",
        "window_name",
        WS_OVERLAPPEDWINDOW,
        69,69,
        420,420,
        0,
        0,
        0,
        0
    );

    ShowWindow(this->app_core->hwnd,1);
    return;
}

App::~App()
{
    if(this->app_core->hwnd){DestroyWindow(this->app_core->hwnd);}
    this->app_runing=false;
    this->app_core->hwnd=nullptr;
    delete this->app_core;
    return;
}

bool8 App::IsRuning()const
{
    return this->app_runing;
}

bool8 App::GetEvent(Event& event_obj)
{
    if(PeekMessageA(&(this->app_core->msg),0,0,0,PM_REMOVE))
    {
        TranslateMessage(&(this->app_core->msg));
        DispatchMessageA(&(this->app_core->msg));
        this->MessageToEvent(event_obj);
        return true;
    }
    event_obj.type=EVENT::EMPTY;
    event_obj.appCode=0;
    event_obj.keyCode=0;
    return false;
}

inline LRESULT CALLBACK WindowProcedure(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    switch(msg)
    {
        case WM_SETFOCUS:
            PostMessage(hwnd,APP_SETFOCUS,wParam,lParam);
        break;
        case WM_KILLFOCUS:
            PostMessage(hwnd,APP_KILLFOCUS,wParam,lParam);
        break;
        case WM_SIZE:
            PostMessage(hwnd,APP_SIZE,wParam,lParam);
        break;
        case WM_MOVE:
            PostMessage(hwnd,APP_MOVE,wParam,lParam);
        break;
        case WM_CLOSE:
            PostMessage(hwnd,APP_CLOSE,wParam,lParam);
        break;
        default:
            return DefWindowProcA(hwnd,msg,wParam,lParam);
        break;
    }
    return 0;
}

void App::MessageToEvent(Event& event_obj)
{
    switch(this->app_core->msg.message)
    {
        case APP_SETFOCUS:
        {
            this->app_focus=true;
            event_obj.type=EVENT::APP;
            event_obj.appCode=EVENT::APP_CODES::FOCUS;
            event_obj.keyCode=0;
        }    
        break;
        case APP_KILLFOCUS:
        {
            this->app_focus=false;
            event_obj.type=EVENT::APP;
            event_obj.appCode=EVENT::APP_CODES::FOCUS;
            event_obj.keyCode=0;
        }    
        break;
        case APP_SIZE:
        {
            RECT window;
            GetClientRect(this->app_core->hwnd,&window);
            this->app_width=window.right-window.left;
            this->app_height=window.bottom-window.top;
            event_obj.type=EVENT::APP;
            event_obj.appCode=EVENT::APP_CODES::RESIZE;
            event_obj.keyCode=0;
        }    
        break;
        case APP_MOVE:
        {
            RECT window;
            GetWindowRect(this->app_core->hwnd,&window);
            this->app_positionX=window.left;
            this->app_positionY=window.top;
            event_obj.type=EVENT::APP;
            event_obj.appCode=EVENT::APP_CODES::MOVE;
            event_obj.keyCode=0;
        }    
        break;
        case APP_CLOSE:
        {
            this->app_runing=false;
            event_obj.type=EVENT::APP;
            event_obj.appCode=EVENT::APP_CODES::CLOSE;
            event_obj.keyCode=0;
            PostQuitMessage(0);
        }
        break;
        case WM_KEYDOWN:
        {
            event_obj.type=EVENT::KEY_PRESED;
            event_obj.appCode=0;
            event_obj.keyCode=this->app_core->msg.wParam;
        }
        break;
        default:
        {
            event_obj.type=EVENT::EMPTY;
            event_obj.appCode=0;
            event_obj.keyCode=0;
        }
        break;
    }
}