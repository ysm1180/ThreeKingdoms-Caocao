#pragma once

#include "LuaLib\LuaTinker.h"

#include <Windows.h>
#include <windowsx.h>
#include <memory>
#include <mutex>
#include <vector>

namespace jojogame
{
class CWindowControl;
class CMoviePlayerControl;
class CButtonControl;
class CMenu;
class CMenuItem;
class CImageControl;
class CToolbarControl;
class CToolbarButton;
class CLayoutControl;
class CListViewControl;
class CListViewColumn;
class CListViewRow;
class CListViewItem;
class CStaticControl;
class CGroupBoxControl;
class CCheckBoxControl;
class CRadioButtonControl;
class CComboBoxControl;
class CAudioPlayerControl;
class CGraphicText;

class CControlManager
{
public:
    static void RegisterFunctions(lua_State *L);

    CControlManager();
    virtual ~CControlManager();

    void Init(HINSTANCE hInstance);

    CWindowControl *CreateWindowForm(CWindowControl *parent);
    CMoviePlayerControl *CreateMoviePlayer(CWindowControl *parent, std::wstring fileName);
    CButtonControl *CreateButton(CWindowControl *parent);
    CMenu *CreateMenu();
    CMenuItem *CreateMenuItem();
    CImageControl *CreateImage();
    CToolbarControl *CreateToolbar();
    CToolbarButton *CreateToolbarButton();
    CLayoutControl *CreateLayout();
    CListViewControl *CreateListView(CWindowControl *parent);
    CListViewColumn *CreateListViewColumn();
    CListViewRow *CreateListViewRow();
    CListViewItem *CreateListViewItem();
    CStaticControl *CreateStatic(CWindowControl *parent);
    CGroupBoxControl *CreateGroupBox(CWindowControl *parent);
    CCheckBoxControl *CreateCheckBox(CWindowControl *parent);
    CRadioButtonControl *CreateRadioButton(CWindowControl *parent, bool isGroupStart);
    CComboBoxControl *CreateComboBox(CWindowControl *parent);
    CAudioPlayerControl *CreateAudioPlayer();
    CGraphicText *CreateGraphicText();

    std::vector<CLayoutControl *> GetLayouts();
    HINSTANCE GetHInstance();

    static CControlManager &GetInstance();

protected:
    HINSTANCE _hInstance;

    std::vector<CLayoutControl *> _layouts;

    static std::once_flag s_onceFlag;
    static std::unique_ptr<CControlManager> s_controlManager;
};
} // namespace jojogame
