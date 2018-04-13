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

    CFile *LoadFile(const char *path);
    char ReadByte(CFile *file, int position);
    void WriteByte(CFile *file, int position, char byte);
    void CloseFile(CFile *file);
    void SetWorkingPath(std::string dir);
    void SetWorkingPath(std::wstring dir);

    std::string GetWorkingPathA();
    std::wstring GetWorkingPathW();
    std::wstring GetFilePath(std::wstring filePath);
    std::string GetFilePath(std::string filePath);

    static CFileManager& GetInstance();
private:
    std::string _currentWorkingPath = "script/";
    std::wstring _currentWorkingPathW = L"script/";

    static std::once_flag s_onceFlag;
    static std::unique_ptr<CFileManager> s_sharedFileManager;
};
}
