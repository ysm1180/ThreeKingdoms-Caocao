#pragma once

#include "BaseControl.h"
#include "LuaLib\LuaTinker.h"

#include <Windows.h>
#include <windowsx.h>

#include <string>
#include <vector>

namespace jojogame {
class CMoviePlayer;
class CMenubar;

struct DrawingImageInfo
{
    int x;
    int y;
    int width;
    int height;
    HDC memDC;
};

class CWindowControl : public CBaseControl
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
    HBRUSH GetBackBrush() const;
    CMenubar *GetMenu();

    void SetControlBox(bool isControlBox);
    void SetMaxButton(bool isMaxButton);
    void SetMinButton(bool isMinButton);
    void SetTitleBar(bool isTitleBar);
    void SetTitleName(std::wstring title);
    void SetActiveEvent(std::wstring activeEvent);
    void SetCloseEvent(std::wstring closeEvent);
    void SetIcon(std::wstring iconFilePath);
    void SetBackColor(COLORREF backColor);
    void SetMenu(CMenubar *menu);
    void SetParentWindow(CWindowControl *parent);

    void AddDrawingImage(HDC srcDC, int x, int y, int width, int height);

    bool Create() override;
    virtual void Destroy() override;

    int ShowModalWindow();
    void Close() const;

    void SetDialogResult(int value) const;

    void Refresh() const;
    void RefreshByRegion(int x, int y, int width, int height);

    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    bool _isMaxButton = false;
    bool _isMinButton = true;
    bool _isControlBox = true;
    bool _isTitleBar = true;
    int *_dialogResult = nullptr;
    std::wstring _titleName = L"";
    std::wstring _activeEvent = L"";
    std::wstring _closeEvent = L"";
    HICON _icon = LoadIcon(nullptr, IDI_APPLICATION);
    HBRUSH _backBrush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));

    std::vector<DrawingImageInfo> _images;
    CMenubar *_menu = nullptr;

};
}
