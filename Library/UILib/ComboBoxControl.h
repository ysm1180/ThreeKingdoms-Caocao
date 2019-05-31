#pragma once
#include "WindowChildControl.h"
#include "TextFont.h"

#include <vector>

namespace jojogame
{
class CComboBoxControl : public CWindowChildControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CComboBoxControl();
    virtual ~CComboBoxControl();

    void AddItem(std::wstring item);

    bool Create() override;

    static WNDPROC GetOriginalProc();
    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    CTextFont _font;
    std::vector<std::wstring> _items;

    static WNDPROC s_originalProc;
};
} // namespace jojogame
