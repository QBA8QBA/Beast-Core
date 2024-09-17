#pragma once
#include "types.h"

namespace EVENT
{
    static const int8 EMPTY=0;
    static const int8 APP=1;
    static const int8 KEY_PRESED=2;
    namespace KEY_CODES
    {
        static const int8 A='A';
        static const int8 B='B';
        static const int8 C='C';
        static const int8 D='D';
        static const int8 E='E';
        static const int8 F='F';
        static const int8 G='G';
        static const int8 H='H';
        static const int8 I='I';
        static const int8 J='J';
        static const int8 K='K';
        static const int8 L='L';
        static const int8 M='M';
        static const int8 N='N';
        static const int8 O='O';
        static const int8 P='P';
        static const int8 R='R';
        static const int8 S='S';
        static const int8 T='T';
        static const int8 U='U';
        static const int8 V='V';
        static const int8 W='W';
        static const int8 X='X';
        static const int8 Y='Y';
        static const int8 Z='Z';
    }
    namespace APP_CODES
    {
        static const int8 CLOSE=0;
        static const int8 RESIZE=1;
        static const int8 MOVE=2;
        static const int8 FOCUS=3;
    }
}

struct Event
{
    int8 type;
    int8 keyCode;
    int8 appCode;
};