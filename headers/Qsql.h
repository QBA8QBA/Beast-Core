#pragma once

#include "types.h"
#include <vector>
#include <string>

struct Table
{
    std::vector<std::vector<std::string>> ColumnVec;
};

void ConectToDataBase
(
    const char* serverName,
    const char* dataBaseName,
    const char* userName,
    const char* userPassword
);
void ReadTableFromDataBase(Table &table,const char* name);
void DisconectFromDataBase();
int8 IsReadable();