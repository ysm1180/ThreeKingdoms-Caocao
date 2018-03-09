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

class  CWindowControl : public CBaseControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CWindowControl();
    virtual ~CWindowControl();

    bool IsControlBox() const;
    bool IsMaxButton() const;
    bool IsMinButton() const;
    bool IsTitleBar() const;
    std::wstring GetTitleName() const;
    std::wstring GetActiveEvent() const;
    std::wstring GetCloseEvent() const;
    CMenu *GetMenu();

    void SetY(int x) override;
    void SetX(int y) override;
    void SetWidth(int width) override;
    void SetHeight(int height) override;
    void SetControlBox(bool isControlBox);
    void SetMaxButton(bool isMaxButton);
    void SetMinButton(bool isMinButton);
    void SetTitlebar(bool isTitlebar);
    void SetTitleName(std::wstring title);
    void SetActiveEvent(std::wstring activeEvent);
    void SetCloseEvent(std::wstring closeEvent);
    void SetIcon(std::wstring iconFilePath);
    void SetBackColor(COLORREF backColor);
    void SetMenu(CMenu *menu);
    void SetParentWindow(CWindowControl *parent);

    void AddLayout(CLayoutControl *layout);

    bool Create() override;
    virtual void Destroy() override;

    int ShowModalWindow();
    void Close() const;

    void SetDialogResult(int value) const;

    void Refresh() const;
    void RefreshRegion(int left, int top, int right, int bottom);

    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    bool _isMaxButton = false;
    bool _isMinButton = true;
    bool _isControlBox = true;
    bool _isTitlebar = true;
    int *_dialogResult = nullptr;
    std::wstring _titleName = L"";
    std::wstring _activeEvent = L"";
    std::wstring _closeEvent = L"";
    HICON _icon = LoadIcon(nullptr, IDI_APPLICATION);
    HBRUSH _backBrush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));

    std::vector<CLayoutControl *> _layouts;
    CMenu *_menu = nullptr;
};
}
