#include "ME5File.h"
#include "FileManager.h"

namespace three_kingdoms
{
void CME5File::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CME5File, "_Me5File");

    LUA_METHOD(GetGroupStartItemIndex);
    LUA_METHOD(GetGroupEndItemIndex);
    LUA_METHOD(GetGroupName);
    LUA_METHOD(GetGroupCount);
    LUA_METHOD(FindGroupIndexByName);
    LUA_METHOD(FindItemIndexByName);
}

CME5File::CME5File()
{
}

CME5File::~CME5File()
{
    Close();
}

bool CME5File::IsEncoding()
{
    BYTE enc;
    _ReadByteArr(&enc, 0, 1);
    return enc == 1;
}

int CME5File::GetItemCount()
{
    return _ReadInt(1);
}

int CME5File::GetGroupCount()
{
    return _ReadInt(5);
}

int CME5File::GetGroupStartItemIndex(int groupIndex)
{
    return _ReadInt(GROUP_HEADER_START_OFFSET + 4 + groupIndex * GROUP_HEADER_SIZE);
}

int CME5File::GetGroupEndItemIndex(int groupIndex)
{
    return _ReadInt(GROUP_HEADER_START_OFFSET + 8 + groupIndex * GROUP_HEADER_SIZE);
}

int CME5File::GetOffset(int index)
{
    int groupCount = GetGroupCount();
    return _ReadInt(index * HEADER_SIZE + groupCount * GROUP_HEADER_SIZE + GROUP_HEADER_START_OFFSET);
}

int CME5File::GetOffset(int groupIndex, int itemIndex)
{
    int startIndex = GetGroupStartItemIndex(groupIndex);
    return GetOffset(startIndex + itemIndex);
}

size_t CME5File::GetGroupNameLength(int groupIndex)
{
    return _ReadUnsignedInt(GROUP_HEADER_START_OFFSET + groupIndex * GROUP_HEADER_SIZE);
}

size_t CME5File::GetNameLength(int index)
{
    int groupCount = GetGroupCount();
    return _ReadUnsignedInt(index * HEADER_SIZE + groupCount * GROUP_HEADER_SIZE + GROUP_HEADER_START_OFFSET + 4);
}

size_t CME5File::GetNameLength(int groupIndex, int itemIndex)
{
    int startIndex = GetGroupStartItemIndex(groupIndex);
    return GetNameLength(startIndex + itemIndex);
}

size_t CME5File::GetItemByteSize(int index)
{
    int groupCount = GetGroupCount();
    return _ReadUnsignedInt(index * HEADER_SIZE + groupCount * GROUP_HEADER_SIZE + GROUP_HEADER_START_OFFSET + 8);
}

size_t CME5File::GetItemByteSize(int groupIndex, int itemIndex)
{
    int startIndex = GetGroupStartItemIndex(groupIndex);
    return GetItemByteSize(startIndex + itemIndex);
}

void CME5File::GetItemByteArr(BYTE *dest, int index)
{
    auto offset = GetOffset(index);
    auto size = GetItemByteSize(index);
    auto length = GetNameLength(index);

    _ReadByteArr(dest, offset + length, size);
}

void CME5File::GetItemByteArr(BYTE *dest, int groupIndex, int itemIndex)
{
    int startIndex = GetGroupStartItemIndex(groupIndex);

    GetItemByteArr(dest, startIndex + itemIndex);
}

std::string CME5File::GetGroupName(int groupIndex)
{
    int offset = GetOffset(groupIndex, 0);
    size_t length = GetGroupNameLength(groupIndex);
    auto *name = new BYTE[length + 1];

    offset -= length;
    _ReadByteArr(name, offset, length);
    name[length] = 0;

    std::string str = std::string((char *)name);
    delete name;

    return str;
}

int CME5File::FindGroupIndexByName(std::string name)
{
    int count = GetGroupCount();
    for (int i = 0; i < count; ++i)
    {
        auto groupName = GetGroupName(i);
        if (groupName == name)
        {
            return i;
        }
    }

    return -1;
}

std::string CME5File::GetItemName(int index)
{
    auto offset = GetOffset(index);
    auto length = GetNameLength(index);
    auto *name = new BYTE[length + 1];
    _ReadByteArr(name, offset, length);
    name[length] = 0;

    std::string str = std::string((char *)name);
    delete name;

    return str;
}

std::string CME5File::GetItemName(int groupIndex, int itemIndex)
{
    int startIndex = GetGroupStartItemIndex(groupIndex);
    return GetItemName(startIndex + itemIndex);
}

int CME5File::FindItemIndexByName(int groupIndex, std::string name)
{
    int startIndex = GetGroupStartItemIndex(groupIndex);
    int endIndex = GetGroupEndItemIndex(groupIndex);

    for (int i = startIndex; i <= endIndex; ++i)
    {
        if (GetItemName(i) == name)
        {
            return i;
        }
    }

    return -1;
}

int CME5File::_ReadInt(int offset)
{
    BYTE temp[4];
    int result = 0;
    fseek(_file, offset, FILE_BEGIN);
    fread_s(temp, 4, sizeof(BYTE), 4, _file);
    for (int i = 0; i < 4; i++)
    {
        result += temp[i] << (i * 8);
    }
    return result;
}

unsigned int CME5File::_ReadUnsignedInt(int offset)
{
    BYTE temp[4];
    unsigned int result = 0;
    fseek(_file, offset, FILE_BEGIN);
    fread_s(temp, 4, sizeof(BYTE), 4, _file);
    for (int i = 0; i < 4; i++)
    {
        result += temp[i] << (i * 8);
    }
    return result;
}

void CME5File::_ReadByteArr(BYTE *dest, int offset, size_t count)
{
    fseek(_file, offset, FILE_BEGIN);
    fread_s(dest, count, sizeof(char), count, _file);
}

bool CME5File::Open(std::wstring filePath)
{
    auto path = CFileManager::GetInstance().GetFilePath(filePath);

    auto error = _wfopen_s(&_file, path.c_str(), L"r+b");
    return error == 0;
}

void CME5File::Close()
{
    if (_file)
    {
        fclose(_file);
    }
}

void CME5File::Dispose()
{
}

} // namespace jojogame
