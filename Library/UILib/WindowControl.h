#pragma once

#include "BaseControl.h"
#include "LuaLib\LuaTinker.h"

namespace jojogame {
class CMenubar;

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
    CMenubar* GetMenu();

    void SetControlBox(bool isControlBox);
    void SetMaxButton(bool isMaxButton);
    void SetMinButton(bool isMinButton);
    void SetTitleBar(bool isTitleBar);
    void SetTitleName(std::wstring title);
    void SetActiveEvent(std::wstring activeEventName);
    void SetCloseEvent(std::wstring closeEventName);
    void SetIcon(std::wstring iconFilePath);
    void SetBackColor(COLORREF backColor);
    void SetMenu(CMenubar* menu);
    void SetParentWindow(CWindowControl *parent);

    /**
     * \brief 윈도우 창을 생성합니다.
     */
    void Create() override;

    /**
     * \brief 생성된 창을 Modal 타입으로 나타냅니다.
     * \return setDialogResult 함수로 설정된 값을 반환합니다.
     */
    int ShowModalWindow();

    /**
     * \brief showModal 함수가 반환하는 값을 설정합니다.
     * \param value
     */
    void SetDialogResult(int value) const;

    /**
     * \brief 생성된 Form window 를 강제로 소멸시킵니다.
     */
    virtual void Destroy() override;

    /**
     * \brief Form 을 종료합니다.
     */
    void Close() const;

    /**
     * \brief Form 을 다시 그립니다.
     */
    void Refresh() const;

    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    bool _isMaxButton = false;
    bool _isMinButton = true;
    bool _isControlBox = true;
    bool _isTitleBar = true;
    int* _dialogResult = nullptr;
    std::wstring _titleName = L"";
    std::wstring _activeEvent = L"";
    std::wstring _closeEvent = L"";
    HICON _icon = LoadIcon(nullptr, IDI_APPLICATION);
    HBRUSH _backBrush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
    CMenubar* _menu = nullptr;
};
}
