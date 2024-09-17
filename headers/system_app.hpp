#pragma once
#include "types.h"

class App
{
private:
    struct App_core* app_core;
    bool8 app_runing;
    bool8 app_focus;
    uint32 app_width;
    uint32 app_height;
    uint32 app_positionX;
    uint32 app_positionY;
private:
    void MessageToEvent(struct Event& event_obj);
public:
    App();
    ~App();
    bool8 GetEvent(struct Event& event_obj);
    bool8 IsRuning()const;
    uint32 GetWidth()const; 
    uint32 GetHeight()const; 
    uint32 GetPositionX()const; 
    uint32 GetPositionY()const; 
};