#pragma once

#include "types.h"

class Drawable
{
public:
    virtual void Draw(void* buffer,uint32 buffer_width,uint32 buffer_height,uint8 buffer_data_size)=0;
};

class BackGround : public Drawable
{
private:
    uint8 r,g,b,a;
public:
    BackGround(uint8 r,uint8 g,uint8 b,uint8 a);
    void Draw(void* buffer,uint32 buffer_width,uint32 buffer_height,uint8 buffer_data_size) override;
};

class Square : public Drawable
{
//TODO: zmienic sposob dosepu do zmienych
public:
    int32 x;
    int32 y;
    int32 width;
    int32 height;
    uint8 r,g,b,a;
public:
    Square(int32 x,int32 y,int32 width,int32 height,uint8 r,uint8 g,uint8 b,uint8 a);
    void Draw(void* buffer,uint32 buffer_width,uint32 buffer_height,uint8 buffer_data_size) override;
    void Move(int32 x,int32 y);
};