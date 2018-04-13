#include "FileManager.h"
#include "BaseLib/MemoryPool.h"

namespace jojogame {
std::once_flag CFileManager::s_onceFlag;
std::unique_ptr<CFileManager> CFileManager::s_sharedFileManager;

void CFileManager::RegisterFunctions(lua_State* L)
{
    LUA_BEGIN(CFileManager, "_FileManager");

    LUA_METHOD(LoadFile);
    LUA_METHOD(ReadByte);
    LUA_METHOD(WriteByte);
    LUA_METHOD(CloseFile);
}

CFile* CFileManager::LoadFile(const char *path)
{
    auto file = CMemoryPool<CFile>::GetInstance().New();
    file->Open(GetFilePath(path));
    return file;
}

char CFileManager::ReadByte(CFile* file, int position)
{
    if (position > file->GetSize() || position < 0)
    {
        return 0;
    }
    return file->GetData()[position];
}

void CFileManager::WriteByte(CFile* file, int position, char byte)
{
    char data[1] = { byte };
    file->Write(data, position, 1);
}

void CFileManager::CloseFile(CFile* file)
{
    CMemoryPool<CFile>::GetInstance().Delete(file);
}

void CFileManager::SetWorkingPath(std::string dir)
{
    _currentWorkingPath = dir;
}

void CFileManager::SetWorkingPath(std::wstring dir)
{
    _currentWorkingPathW = dir;
}

std::string CFileManager::GetWorkingPathA()
{
    return _currentWorkingPath;
}

std::wstring CFileManager::GetWorkingPathW()
{
    return _currentWorkingPathW;
}

std::wstring CFileManager::GetFilePath(std::wstring filePath)
{
    return _currentWorkingPathW + filePath;
}

std::string CFileManager::GetFilePath(std::string filePath)
{
    return _currentWorkingPath + filePath;
}

CFileManager& CFileManager::GetInstance()
{
    std::call_once(s_onceFlag,
                   []
                   {
                       s_sharedFileManager = std::make_unique<jojogame::CFileManager>();
                   });

    return *s_sharedFileManager;
}
}
