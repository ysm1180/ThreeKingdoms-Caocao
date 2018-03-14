#include "ControlManager.h"

#include "BaseLib\MemoryPool.h"

#include "BaseControl.h"
#include "TextFont.h"
#include "WindowControl.h"
#include "MoviePlayerControl.h"
#include "ButtonControl.h"
#include "MenuControl.h"
#include "ImageControl.h"
#include "ToolbarControl.h"
#include "LayoutControl.h"
#include "ListviewControl.h"

namespace jojogame {
std::once_flag CControlManager::s_onceFlag;
std::unique_ptr<CControlManager> CControlManager::s_controlManager;

void CControlManager::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CControlManager, "_ControlManager");

    LUA_METHOD(CreateWindowForm);
    LUA_METHOD(CreateMoviePlayer);
    LUA_METHOD(CreateButton);
    LUA_METHOD(CreateMenu);
    LUA_METHOD(CreateMenuItem);
    LUA_METHOD(CreateImage);
    LUA_METHOD(CreateToolbar);
    LUA_METHOD(CreateToolbarButton);
    LUA_METHOD(CreateLayout);
    LUA_METHOD(CreateListView);
    LUA_METHOD(CreateListViewColumn);
}

CControlManager::CControlManager()
{
}

CControlManager::~CControlManager()
{
}

void CControlManager::Init(HINSTANCE hInstance)
{
    _hInstance = hInstance;

    CLuaTinker::GetLuaTinker().RegisterClassToLua<CControlManager>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CBaseControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CTextFont>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CWindowControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CMoviePlayerControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CButtonControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CMenu>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CMenuItem>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CImageControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CToolbarControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CToolbarButton>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CLayoutControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CListViewControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CListViewColumn>();
}

CWindowControl *CControlManager::CreateWindowForm(CWindowControl *parent)
{
    auto newWindow = CMemoryPool<CWindowControl>::GetInstance().New();
    newWindow->SetParentWindow(parent);
    return newWindow;
}

CMoviePlayerControl *CControlManager::CreateMoviePlayer(CWindowControl *parent, std::string fileName)
{
    auto newMoviePlayer = CMemoryPool<CMoviePlayerControl>::GetInstance().New(parent, fileName);
    return newMoviePlayer;
}

CButtonControl * CControlManager::CreateButton(CWindowControl *parent)
{
    auto newButton = CMemoryPool<CButtonControl>::GetInstance().New();
    newButton->SetParentWindow(parent);
    return newButton;
}

CMenu * CControlManager::CreateMenu()
{
    return CMemoryPool<CMenu>::GetInstance().New();
}

CMenuItem * CControlManager::CreateMenuItem()
{
    return CMemoryPool<CMenuItem>::GetInstance().New();;
}

CImageControl *CControlManager::CreateImage()
{
    return CMemoryPool<CImageControl>::GetInstance().New();
}

CToolbarControl *CControlManager::CreateToolbar()
{
    return CMemoryPool<CToolbarControl>::GetInstance().New();
}

CToolbarButton *CControlManager::CreateToolbarButton()
{
    return CMemoryPool<CToolbarButton>::GetInstance().New();
}

CLayoutControl *CControlManager::CreateLayout()
{
    return CMemoryPool<CLayoutControl>::GetInstance().New();
}

CListViewControl* CControlManager::CreateListView(CWindowControl* parent)
{
    auto newListView = CMemoryPool<CListViewControl>::GetInstance().New();
    newListView->SetParentWindow(parent);
    return newListView;
}

CListViewColumn* CControlManager::CreateListViewColumn()
{
    auto newColumn = CMemoryPool<CListViewColumn>::GetInstance().New();
    return newColumn;
}

HINSTANCE CControlManager::GetHInstance()
{
    return _hInstance;
}

CControlManager& CControlManager::GetInstance()
{
    std::call_once(s_onceFlag, []
    {
        s_controlManager = std::make_unique<jojogame::CControlManager>();
    });

    return *s_controlManager;
}
}