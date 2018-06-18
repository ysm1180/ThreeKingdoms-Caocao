#include "LuaTinker.h"

#include "UILib\WindowControl.h"
#include "CommonLib/FileManager.h"
#include "BaseLib/File.h"

#include <Shlwapi.h>

namespace jojogame {
std::once_flag CLuaTinker::s_onceFlag;
std::unique_ptr<CLuaTinker> CLuaTinker::s_luaTinker;

int CustomLuaRequire(lua_State *L)
{
    const char *filePath = lua_tostring(L, -1);

    if (!filePath)
    {
        return 0;
    }
    
    std::string workingDirectory(filePath);
    const size_t last_slash_idx = workingDirectory.rfind('/');
    if (std::string::npos != last_slash_idx)
    {
        workingDirectory = workingDirectory.substr(0, last_slash_idx) + "/";
    }
    else
    {
        workingDirectory = "";
    }

    CFile file;
    auto path = CFileManager::GetInstance().GetFilePath(std::string(filePath));
    file.Open(path);

    std::string originalWorkingPath = CFileManager::GetInstance().GetWorkingPathA();
    CFileManager::GetInstance().SetWorkingPath(originalWorkingPath + workingDirectory);

    lua_tinker::dobuffer(L, file.GetData(), file.GetSize(), path.c_str(), true);

    CFileManager::GetInstance().SetWorkingPath(originalWorkingPath);

    return 1;
}

int CustomLuaMessage(lua_State *L)
{
    char c[128] = { 0, };

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

CLuaTinker& CLuaTinker::GetLuaTinker()
{
    std::call_once(s_onceFlag,
                   []
    {
        s_luaTinker = std::make_unique<jojogame::CLuaTinker>();
    });

    return *s_luaTinker;
}
}
