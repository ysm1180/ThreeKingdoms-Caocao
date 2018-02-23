#pragma once

#include "LuaLib\LuaTinker.h"

#include <Windows.h>
#include <windowsx.h>
#include <memory>
#include <mutex>

namespace jojogame {
class CWindowControl;
class CMoviePlayerControl;
class CButtonControl;
class CMenu;
class CMenuItem;

class  CControlManager
{
public:
    static void RegisterFunctions(lua_State *L);

    CControlManager();
    virtual ~CControlManager();

    void Init(HINSTANCE hInstance);

    CWindowControl *CreateWindowForm(CWindowControl *parent);
    CMoviePlayerControl *CreateMoviePlayer(CWindowControl *parent, std::string fileName);
    CButtonControl *CreateButton(CWindowControl *parent);
    CMenu *CreateMenu();
    CMenuItem *CreateMenuItem();

    HINSTANCE GetHInstance();

    void SetHInstance(HINSTANCE hInstance);

    static CControlManager& GetInstance();
protected:
    HINSTANCE _hInstance;

    static std::once_flag s_onceFlag;
    static std::unique_ptr<CControlManager> s_controlManager;
};
}
