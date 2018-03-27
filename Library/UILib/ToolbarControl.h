#pragma once

#include "LuaLib\LuaTinker.h"

#include <atlcoll.h>
#include <windows.h>
#include <commctrl.h>
#include <vector>
#include <string>

namespace jojogame {
class CWindowControl;
class CImageControl;
class CToolbarControl;

class CToolbarButton
{
public:
    static void RegisterFunctions(lua_State *L);

    CToolbarButton();
    virtual ~CToolbarButton();

    bool IsEnabled();
    int GetIndex();
    TBBUTTON& GetButtonStruct();
    CImageControl *GetImage();
    std::wstring GetText();
    std::wstring GetClickEvent();
    std::wstring GetTooltipText();
    CToolbarControl *GetParentToolbar();

    void SetEnabled(bool isEnabled);
    void SetIndex(int index);
    void SetImageIndex(int imageIndex);
    void SetImage(CImageControl *image);
    void SetText(std::wstring text);
    void SetClickEvent(std::wstring clickEvent);
    void SetTooltipText(std::wstring tooltipText);
    void SetParentToolbar(CToolbarControl *parent);

    bool Create();

    void Show();
    void Hide();

private:
    BYTE _state;
    bool _isEnabled = true;

    CToolbarControl *_parent = nullptr;
    CImageControl *_image = nullptr;

    TBBUTTON _button{};

    std::wstring _text = L"";
    std::wstring _clickEvent = L"";
    std::wstring _tooltipText = L"";
};


class CToolbarControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CToolbarControl();
    ~CToolbarControl();

    HWND GetHWnd() const;
    int GetHeight() const;

    bool Create(CWindowControl *parentWindow, int imageWidth, int imageHeight);
    void AddButton(CToolbarButton *button);
    void DeleteButton(CToolbarButton *button);

    void AutoSize();

    void Show();
    void Hide();

private:
    HWND _hWnd = nullptr;
    HIMAGELIST _hImageList = nullptr;
    std::vector<CToolbarButton *> _buttons;
    std::vector<CImageControl *> _imageList;
};
}
