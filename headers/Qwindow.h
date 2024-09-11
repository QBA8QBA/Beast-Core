#pragma once

#include "types.h"
#include "Qdrawable.h"

void SpawnWindow(uint32 width,uint32 height,const char* name);
void DespawnWindow();

int8 IsWindowWorking();
int8 IsKeyPresed(uint8 key_id);

//TODO:kilka wersji
void WindowDrawBuffer(Drawable& obj);
void WindowSwapBuffer();