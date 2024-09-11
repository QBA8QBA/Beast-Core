#include "Qdrawable.h"

#include <iostream>

BackGround::BackGround(uint8 r,uint8 g,uint8 b,uint8 a)
{
    this->r=r;
    this->g=g;
    this->b=b;
    this->a=a;
}

void BackGround::Draw(void* buffer,uint32 buffer_width,uint32 buffer_height,uint8 buffer_data_size)
{
    uint8* row=(uint8*)buffer;
    int32 rowSize=buffer_width*buffer_data_size;
    for (int32 Y=0;Y<buffer_height;++Y)
    {
        uint8* pixel=(uint8*)row;
        for (int32 X=0;X<buffer_width;++X)
        {
            *pixel=this->b;
            ++pixel;
            *pixel=this->g;
            ++pixel;
            *pixel=this->r;
            ++pixel;
            *pixel=this->a;
            ++pixel;
        }
        row+=rowSize;
    }
    return;
}

Square::Square(int32 x,int32 y,int32 width,int32 height,uint8 r,uint8 g,uint8 b,uint8 a)
{
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
    this->r=r;
    this->g=g;
    this->b=b;
    this->a=a;
}

void Square::Draw(void* buffer,uint32 buffer_width,uint32 buffer_height,uint8 buffer_data_size)
{
    if ((this->x+this->width<0)||(this->y+this->height<0)||(this->x>buffer_width)||(this->y>buffer_height)||(this->width<0)||(this->height<0))
    {
        //std::cout << "OUT OF VIEW\n";
    }
    else
    {
        if (this->x<0)
        {
            this->width+=this->x;
            this->x=0;
        }
        if (this->y<0)
        {
            this->height+=this->y;
            this->y=0;
        }
        if (this->x+this->width>buffer_width)
        {
            this->width=buffer_width;
        }
        if (this->y+this->height>buffer_height)
        {
            this->height=buffer_height;
        }
        uint8* row=(uint8*)buffer;
        int32 rowSize=buffer_width*buffer_data_size;
        for (int32 Y=0;Y<buffer_height;++Y)
        {
            uint8* pixel=(uint8*)row;
            for (int32 X=0;X<buffer_width;++X)
            {
                if ((X>=this->x)&&(X<this->x+this->width)&&(Y>=this->y)&&(Y<this->y+this->height))
                {
                    *pixel=this->b;
                    ++pixel;
                    *pixel=this->g;
                    ++pixel;
                    *pixel=this->r;
                    ++pixel;
                    *pixel=this->a;
                    ++pixel;
                }
                else
                {
                    pixel+=4;
                }
            }
            row+=rowSize;
        }
    }
    return;
}

void Square::Move(int32 x,int32 y)
{
    this->x+=x;
    this->y+=y;
    return;
}