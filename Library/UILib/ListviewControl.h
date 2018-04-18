#pragma once

#include "TextFont.h"
#include "BaseControl.h"

#include <vector>
#include <CommCtrl.h>

namespace jojogame {
class CWindowControl;
class CListViewControl;

struct ListViewItemColorState
{
    COLORREF normal;
    COLORREF focused;
    COLORREF disabled;
    COLORREF disableFocused;
};

class CListViewColumn
{
public:
    static void RegisterFunctions(lua_State* L);

    CListViewColumn();
    ~CListViewColumn();

    bool IsAutoSizeFitItem() const;
    bool IsAutoSizeFitHeader() const;
    int GetIndex() const;
    int GetAlign() const;
    int GetWidth() const;
    std::wstring GetText() const;

    void SetParentListView(CListViewControl* parent);
    void SetAutoSizeFitItem(bool value);
    void SetAutoSizeFitHeader(bool value);
    void SetIndex(int index);
    void SetAlign(int align);
    void SetWidth(int width);
    void SetText(std::wstring text);

private:
    CListViewControl* _parentListView = nullptr;

    int _index = -1;
    int _align = LVCFMT_LEFT;
    int _width = 0;

    bool _isAutoSizeFitItem = false;
    bool _isAutoSizeFitHeader = false;

    std::wstring _text = L"";
};

class CListViewItem
{
public:
    static void RegisterFunctions(lua_State* L);

    CListViewItem();
    ~CListViewItem();

    std::wstring GetText();
    CTextFont* GetFont();
    COLORREF GetNormalBackgroundColor();
    COLORREF GetFocusedBackgroundColor();
    COLORREF GetDisabledBackgroundColor();
    COLORREF GetDisableFocusedBackgroundColor();
    COLORREF GetNormalTextColor();
    COLORREF GetFocusedTextColor();
    COLORREF GetDisabledTextColor();
    COLORREF GetDisableFocusedTextColor();
    int GetAlign();

    void SetParentListRow(CListViewRow* parent);
    void SetText(std::wstring text);
    void SetNormalBackgroundColor(COLORREF color);
    void SetFocusedBackgroundColor(COLORREF color);
    void SetDisabledBackgroundColor(COLORREF color);
    void SetDisableFocusedBackgroundColor(COLORREF color);
    void SetNormalTextColor(COLORREF color);
    void SetFocusedTextColor(COLORREF color);
    void SetDisabledTextColor(COLORREF color);
    void SetDisableFocusedTextColor(COLORREF color);
    void SetAlign(int align);
    void SetItemIndex(int itemIndex);

private:
    void _Update();

    CListViewRow* _parentListRow = nullptr;

    int _itemIndex = -1;
    std::wstring _text = L"";

    CTextFont _font;

    ListViewItemColorState _backgroundColor{
        RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x78, 0xD7), RGB(0xFF, 0xFF, 0xFF), RGB(0xE6, 0xE6, 0xE6)
    };
    ListViewItemColorState _textColor{
        RGB(0x00, 0x00, 0x00), RGB(0xFF, 0xFF, 0xFF), RGB(0x6D, 0x6D, 0x6D), RGB(0x6D, 0x6D, 0x6D)
    };

    int _align = 0; // 0 : LEFT, 1 : RIGHT, 2 : CENTER
};

class CListViewRow
{
public:
    static void RegisterFunctions(lua_State* L);

    CListViewRow();
    virtual ~CListViewRow();

    bool IsEnabled();
    CListViewControl* GetParentListView();
    int GetActiveEvent();
    CListViewItem* GetItem(int subIndex);
    int GetRowIndex();

    void SetEnabled(bool isEnabled);
    void SetParentListView(CListViewControl* parent);
    void SetNormalBackgroundColor(COLORREF color);
    void SetFocusedBackgroundColor(COLORREF color);
    void SetNormalTextColor(COLORREF color);
    void SetFocusedTextColor(COLORREF color);
    void SetActiveEvent();
    void SetItem(int subIndex, CListViewItem* item);
    void SetRowIndex(int index);

    void AddItem(CListViewItem* item);

private:

    bool _isEnabled = true;

    CListViewControl* _parentListView = nullptr;

    int _rowIndex = -1;
    int _activeEvent = LUA_NOREF;
    std::vector<CListViewItem *> _items;
};

class CListViewControl : public CBaseControl
{
public:
    static void RegisterFunctions(lua_State* L);

    CListViewControl();
    virtual ~CListViewControl();

    bool IsShowBorder() const;
    bool IsShowColumn() const;
    bool IsSortClickedColumn() const;
    bool IsOneClickItemActivated() const;
    bool IsTrackingSelect() const;
    bool IsTransparentBackground() const;
    CListViewColumn* GetColumn(int columnIndex);
    CListViewRow* GetRow(int rowIndex);
    int GetColumnCount();
    COLORREF GetBackgroundColor();
    HBRUSH GetBackgroundBrush();
    int GetRowHeight();

    void SetShowBoder(bool isShowBorder);
    void SetShowColumn(bool isShowColumn);
    void SetSortClickedColumn(bool isSortClickedColumn);
    void SetOneClickItemActivated(bool isHotClick);
    void SetTrackingSelect(bool isTrackingSelect);
    void SetTransparentBackground(bool isTransparentBackground);
    void SetParentWindow(CWindowControl* parent);
    void SetBackgroundColor(COLORREF color);
    void SetRowHeight(int rowHeight);

    void AddColumn(CListViewColumn* column);
    void AddRow(CListViewRow* row);

    bool Create() override;

    static WNDPROC GetOriginalProc();
    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    int _rowHeight = 19;

    long _exStyle;

    bool _isShowBorder = true;
    bool _isShowColumn = true;
    bool _isSortClckedColumn = false;
    bool _isOneClickItemActivated = false;
    bool _isTrackingSelect = false;
    bool _isTransparentBackground = false;

    COLORREF _backgroundColor = RGB(0xFF, 0xFF, 0xFF);
    HBRUSH _backgroundBrush = CreateSolidBrush(_backgroundColor);

    std::vector<CListViewColumn *> _columns;
    std::vector<CListViewRow *> _rows;

    static WNDPROC s_originalProc;
};
}
