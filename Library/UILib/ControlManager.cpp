#include "ControlManager.h"

#include "BaseLib\MemoryPool.h"

#include "BaseControl.h"
#include "WindowChildControl.h"
#include "TextFont.h"
#include "WindowControl.h"
#include "MoviePlayerControl.h"
#include "ButtonControl.h"
#include "MenuControl.h"
#include "ImageControl.h"
#include "ToolbarControl.h"
#include "LayoutControl.h"
#include "ListviewControl.h"
#include "StaticControl.h"
#include "GroupBoxControl.h"
#include "CheckBoxControl.h"
#include "RadioButtonControl.h"
#include "ComboBoxControl.h"
#include "AudioPlayer.h"
#include "GraphicText.h"

namespace jojogame {
std::once_flag CControlManager::s_onceFlag;
std::unique_ptr<CControlManager> CControlManager::s_controlManager;

void CControlManager::RegisterFunctions(lua_State* L)
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
    LUA_METHOD(CreateListViewRow);
    LUA_METHOD(CreateListViewItem);
    LUA_METHOD(CreateStatic);
    LUA_METHOD(CreateGroupBox);
    LUA_METHOD(CreateCheckBox);
    LUA_METHOD(CreateRadioButton);
    LUA_METHOD(CreateComboBox);
    LUA_METHOD(CreateAudioPlayer);
    LUA_METHOD(CreateGraphicText);
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
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CWindowChildControl>();
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
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CListViewRow>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CListViewItem>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CStaticControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CGroupBoxControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CCheckBoxControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CRadioButtonControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CComboBoxControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CAudioPlayerControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CGraphicText>();

}

CWindowControl* CControlManager::CreateWindowForm(CWindowControl* parent)
{
    auto control = CMemoryPool<CWindowControl>::GetInstance().New();
    control->SetParentWindow(parent);
    return control;
}

CAudioPlayerControl* CControlManager::CreateAudioPlayer()
{
    auto control = CMemoryPool<CAudioPlayerControl>::GetInstance().New();
    return control;
}

CGraphicText* CControlManager::CreateGraphicText()
{
    auto control = CMemoryPool<CGraphicText>::GetInstance().New();
    return control;
}

CMoviePlayerControl* CControlManager::CreateMoviePlayer(CWindowControl* parent, std::wstring fileName)
{
    auto control = CMemoryPool<CMoviePlayerControl>::GetInstance().New(parent, fileName);
    return control;
}

CButtonControl* CControlManager::CreateButton(CWindowControl* parent)
{
    auto control = CMemoryPool<CButtonControl>::GetInstance().New();
    control->SetParentWindow(parent);
    return control;
}

CMenu* CControlManager::CreateMenu()
{
    return CMemoryPool<CMenu>::GetInstance().New();
}

CMenuItem* CControlManager::CreateMenuItem()
{
    return CMemoryPool<CMenuItem>::GetInstance().New();;
}

CImageControl* CControlManager::CreateImage()
{
    return CMemoryPool<CImageControl>::GetInstance().New();
}

CToolbarControl* CControlManager::CreateToolbar()
{
    return CMemoryPool<CToolbarControl>::GetInstance().New();
}

CToolbarButton* CControlManager::CreateToolbarButton()
{
    return CMemoryPool<CToolbarButton>::GetInstance().New();
}

CLayoutControl* CControlManager::CreateLayout()
{
    return CMemoryPool<CLayoutControl>::GetInstance().New();
}

CListViewControl* CControlManager::CreateListView(CWindowControl* parent)
{
    auto control = CMemoryPool<CListViewControl>::GetInstance().New();
    control->SetParentWindow(parent);
    return control;
}

CListViewColumn* CControlManager::CreateListViewColumn()
{
    auto control = CMemoryPool<CListViewColumn>::GetInstance().New();
    return control;
}

CListViewRow* CControlManager::CreateListViewRow()
{
    auto control = CMemoryPool<CListViewRow>::GetInstance().New();
    return control;
}

CListViewItem* CControlManager::CreateListViewItem()
{
    auto control = CMemoryPool<CListViewItem>::GetInstance().New();
    return control;
}

CStaticControl* CControlManager::CreateStatic(CWindowControl* parent)
{
    auto control = CMemoryPool<CStaticControl>::GetInstance().New();
    control->SetParentControl(parent);
    return control;
}

CGroupBoxControl* CControlManager::CreateGroupBox(CWindowControl* parent)
{
    auto control = CMemoryPool<CGroupBoxControl>::GetInstance().New();
    control->SetParentControl(parent);
    return control;
}

CCheckBoxControl* CControlManager::CreateCheckBox(CWindowControl* parent)
{
    auto control = CMemoryPool<CCheckBoxControl>::GetInstance().New();
    control->SetParentControl(parent);
    return control;
}

CRadioButtonControl* CControlManager::CreateRadioButton(CWindowControl* parent, bool isGroupStart)
{
    auto control = CMemoryPool<CRadioButtonControl>::GetInstance().New(isGroupStart);
    control->SetParentControl(parent);
    return control;
}

CComboBoxControl* CControlManager::CreateComboBox(CWindowControl* parent)
{
    auto control = CMemoryPool<CComboBoxControl>::GetInstance().New();
    control->SetParentControl(parent);
    return control;
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
