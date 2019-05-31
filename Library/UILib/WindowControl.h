#pragma once

#include "BaseControl.h"
#include "LuaLib\LuaTinker.h"

#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <vector>

namespace jojogame {
class CMoviePlayer;
class CMenu;
class CLayoutControl;
class CToolbarControl;

class CWindowControl : public CBaseControl
{
public:
    static void RegisterFunctions(lua_State* L);

    CWindowControl();
    virtual ~CWindowControl();

    bool IsControlBox() const;
    bool IsMaxButton() const;
    bool IsMinButton() const;
    bool IsTitleBar() const;
    bool IsSizable() const;
    int GetX() const;
    int GetY() const;
    int GetLuaHeight() const;
    std::wstring GetTitleName() const;
    int GetActiveEvent() const;
    int GetCloseEvent() const;
    int GetSizeEvent() const;
    int GetKeyDownEvent() const;
    int GetKeyUpEvent() const;
    COLORREF GetBackgroundColor();
    HBRUSH GetBackgroundBrush();
    CMenu* GetMenu();
    CToolbarControl* GetToolbar();

    void SetY(int y) override;
    void SetX(int x) override;
    void SetWidth(int width) override;
    void SetHeight(int height) override;
    void SetLuaHeight(int height);
    void SetControlBox(bool isControlBox);
    void SetMaxButton(bool isMaxButton);
    void SetMinButton(bool isMinButton);
    void SetTitlebar(bool isTitlebar);
    void SetSizable(bool isSizable);
    void SetTitleName(std::wstring title);
    void SetActiveEvent();
    void SetCloseEvent();
    void SetSizeEvent();
    void SetKeyDownEvent();
    void SetKeyUpEvent();
    void SetIcon(std::wstring iconFilePath);
    void SetBackgroundColor(COLORREF backColor);
    void SetMenu(CMenu* menu);
    void SetParentWindow(CWindowControl* parent);
	void SetToolbar(CToolbarControl* toolbar);

    void AddLayout(CLayoutControl* layout, bool isShow);
    void DeleteLayout(CLayoutControl* layout);

    bool Create() override;

    int ShowModalWindow();
    void Close() const;

    void Clear();

    void SetDialogResult(int value) const;

    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    bool _isMaxButton = false;
    bool _isMinButton = true;
    bool _isControlBox = true;
    bool _isTitlebar = true;
    bool _isSizable = false;
    int* _dialogResult = nullptr;

    std::wstring _titleName = L"";

    int _activeEvent = LUA_NOREF;
    int _closeEvent = LUA_NOREF;
    int _sizeEvent = LUA_NOREF;
    int _keyDownEvent = LUA_NOREF;
    int _keyUpEvent = LUA_NOREF;

    HICON _icon = nullptr;
    COLORREF _backgroundColor = GetSysColor(COLOR_3DFACE);
    HBRUSH _backBrush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));

    std::vector<CLayoutControl *> _layouts;
    CMenu* _menu = nullptr;
	CToolbarControl* _toolbar = nullptr;
};
}
