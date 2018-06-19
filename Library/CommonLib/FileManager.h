#pragma once
#include "BaseLib/File.h"
#include "LuaLib/LuaTinker.h"

#include <memory>
#include <mutex>

namespace jojogame {
class CFileManager
{
public:
    static void RegisterFunctions(lua_State *L);

    CFile *LoadFile(std::wstring path);
    char ReadByte(CFile *file, int position);
    void WriteByte(CFile *file, int position, char byte);
    void CloseFile(CFile *file);
    void SetWorkingPath(std::wstring dir);

    std::wstring GetWorkingPath();
    std::wstring GetFilePath(std::wstring filePath);

    static CFileManager& GetInstance();
private:
    std::wstring _currentWorkingPath = L"script/";

    static std::once_flag s_onceFlag;
    static std::unique_ptr<CFileManager> s_sharedFileManager;
};
}
