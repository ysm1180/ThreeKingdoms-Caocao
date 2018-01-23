#pragma once

namespace jojogame {
class CWindowControl;

class CControlManager
{
public:
    static void RegisterFunctions(lua_State* L);

    CControlManager();
    virtual ~CControlManager();

    void Init(HINSTANCE hInstance);

    CWindowControl* CreateWindowForm(CWindowControl *parent);
    // void CreatePopupWindow(CWindowControl *parent);
    // void CreateMoviePlayer(CWindowControl *parent);

    HINSTANCE GetHInstance();

    void SetHInstance(HINSTANCE hInstance);

    static CControlManager& GetInstance();
protected:
    HINSTANCE _hInstance;

    static std::once_flag s_onceFlag;
    static std::unique_ptr<CControlManager> s_controlManager;
};
}
