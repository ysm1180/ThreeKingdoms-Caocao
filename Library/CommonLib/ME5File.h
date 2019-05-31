#pragma once

#include "LuaLib/LuaTinker.h"

#include <Windows.h>
#include <cstdio>
#include <string>

namespace jojogame
{
//4 Offset; 4 NameLength; 4 ItemSize;
const int HEADER_SIZE = 12;
//4 GroupNameLength; 4 StartIndex; 4 EndIndex;
const int GROUP_HEADER_SIZE = 12;

const int GROUP_HEADER_START_OFFSET = 9;

class CME5File
{
public:
    static void RegisterFunctions(lua_State *L);

    CME5File();
    ~CME5File();

    bool IsEncoding();
    int GetItemCount();
    int GetGroupCount();
    int GetGroupStartItemIndex(int groupIndex);
    int GetGroupEndItemIndex(int groupIndex);
    int GetOffset(int index);
    int GetOffset(int groupIndex, int itemIndex);
    size_t GetGroupNameLength(int groupIndex);
    size_t GetNameLength(int index);
    size_t GetNameLength(int groupIndex, int itemIndex);
    size_t GetItemByteSize(int index);
    size_t GetItemByteSize(int groupIndex, int itemIndex);
    void GetItemByteArr(BYTE *dest, int index);
    void GetItemByteArr(BYTE *dest, int groupIndex, int itemIndex);
    std::string GetGroupName(int groupIndex);
    int FindGroupIndexByName(std::string name);
    std::string GetItemName(int index);
    std::string GetItemName(int groupIndex, int itemIndex);
    int FindItemIndexByName(int groupIndex, std::string name);

    bool Open(std::wstring filePath);
    void Close();

    void Dispose();

private:
    int _ReadInt(int offset);
    unsigned int _ReadUnsignedInt(int offset);
    void _ReadByteArr(BYTE *dest, int offset, size_t count);

    FILE *_file = nullptr;
};
} // namespace jojogame
