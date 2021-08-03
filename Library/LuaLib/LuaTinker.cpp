#include "LuaTinker.h"

#include "UILib\WindowControl.h"
#include "CommonLib/FileManager.h"
#include "BaseLib/File.h"

#include <Shlwapi.h>

namespace three_kingdoms
{
std::once_flag CLuaTinker::s_onceFlag;
std::unique_ptr<CLuaTinker> CLuaTinker::s_luaTinker;

int CustomLuaRequire(lua_State *L)
{
    const char *str = lua_tostring(L, -1);

    if (!str)
    {
        return 0;
    }

    const auto len = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
    std::wstring filePath(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str, -1, &filePath[0], len);

    std::wstring workingDirectory(filePath);
    const size_t last_slash_idx = workingDirectory.rfind('/');
    if (std::string::npos != last_slash_idx)
    {
        workingDirectory = workingDirectory.substr(0, last_slash_idx) + L"/";
    }
    else
    {
        workingDirectory = L"";
    }

    CFile file;
    auto path = CFileManager::GetInstance().GetFilePath(filePath);
    file.Open(path);

    std::wstring originalWorkingPath = CFileManager::GetInstance().GetWorkingPath();
    CFileManager::GetInstance().SetWorkingPath(originalWorkingPath + workingDirectory);

    int length = WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, NULL, 0, NULL, NULL);
    char *buffer = new char[length + 1];
    WideCharToMultiByte(CP_UTF8, 0, path.c_str(), -1, buffer, length, NULL, NULL);
    lua_tinker::dobuffer(L, file.GetData(), file.GetSize(), buffer, true);
    delete[] buffer;

    CFileManager::GetInstance().SetWorkingPath(originalWorkingPath);

    return 1;
}

int CustomLuaMessage(lua_State *L)
{
    char c[128] = {
        0,
    };

    if (lua_isstring(L, -1))
    {
        std::string str(lua_tostring(L, -1));
        MessageBoxA(GetDesktopWindow(), str.c_str(), "Lua Print", 0);
    }
    else if (lua_isnumber(L, -1))
    {
        sprintf_s(c, sizeof(c), "%lf", lua_tonumber(L, -1));
        MessageBoxA(GetDesktopWindow(), c, "Lua Print", 0);
    }
    else if (lua_isnil(L, -1))
    {
        MessageBoxA(GetDesktopWindow(), "nil", "Lua Print", 0);
    }

    lua_pop(L, 1);

    return 0;
}

CLuaTinker::CLuaTinker()
{
    _luaState = luaL_newstate();
    luaL_openlibs(_luaState);
    lua_settop(_luaState, 0);

    RegisterFunction("require", CustomLuaRequire);
    RegisterFunction("print", CustomLuaMessage);
}

CLuaTinker::~CLuaTinker()
{
    if (_luaState)
    {
        lua_close(_luaState);
    }
    _luaState = nullptr;
}

lua_State *CLuaTinker::GetLuaState()
{
    return _luaState;
}

void CLuaTinker::Run(const char *fileName)
{
    lua_tinker::dofile(_luaState, fileName);
}

CLuaTinker &CLuaTinker::GetLuaTinker()
{
    std::call_once(s_onceFlag,
                   [] {
                       s_luaTinker = std::make_unique<three_kingdoms::CLuaTinker>();
                   });

    return *s_luaTinker;
}
} // namespace three_kingdoms
