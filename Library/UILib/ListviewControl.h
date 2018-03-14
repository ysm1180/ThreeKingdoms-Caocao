#pragma once

#include "BaseControl.h"

#include <vector>
#include <CommCtrl.h>

namespace jojogame {
class CWindowControl;
class CListViewControl;

struct ListViewColorState
{
    COLORREF normal;
    COLORREF selected;
};

class CListViewColumn
{
public:
    static void RegisterFunctions(lua_State *L);

    CListViewColumn();
    ~CListViewColumn();

    CListViewControl *GetParentListView();
    int GetIndex();
    int GetAlign();
    int GetWidth();
    std::wstring GetText();

    void SetParentListView(CListViewControl* parent);
    void SetIndex(int index);
    void SetAlign(int align);
    void SetWidth(int width);
    void SetText(std::wstring text);

private:
    CListViewControl *_parentListView = nullptr;

    int _index = -1;
    int _align = LVCFMT_LEFT;
    int _width = 0;

    std::wstring _text = L"";
};

class CListViewItem
{
public:
    static void RegisterFunctions(lua_State *L);

    CListViewItem();
    ~CListViewItem();

private:

};

class CListViewControl : public CBaseControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CListViewControl();
    ~CListViewControl();

    bool IsShowColumn();
    bool IsSortClickedColumn();
    bool IsHotClick();
    bool IsTrackingSelect();
    CListViewColumn *GetColumn(int columnIndex);

    void SetShowColumn(bool isShowColumn);
    void SetSortClickedColumn(bool isSortClickedColumn);
    void SetHotClick(bool isHotClick);
    void SetTrackingSelect(bool isTrackingSelect);
    void SetParentWindow(CWindowControl *parent);

    void AddColumn(CListViewColumn *column);

    bool Create() override;
    void Destroy() override;

    static WNDPROC GetOriginalProc();
    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    bool _isShowColumn = true;
    bool _isSortClckedColumn = false;
    bool _isHotClick = false;
    bool _isTrackingSelect = false;

    std::vector<CListViewColumn *> _columns;

    static WNDPROC s_originalProc;
};
}
