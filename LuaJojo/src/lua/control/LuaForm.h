#pragma once

#include "LuaControl.h"

#include <string>

namespace jojogame {
class LuaMenubar;

class __declspec(dllexport) LuaForm : public LuaControl
{
public:
    explicit LuaForm(bool popup);
    virtual ~LuaForm();

    LuaForm(const LuaForm&) = delete;
    LuaForm& operator=(const LuaForm&) = delete;

    bool controlBox() const;
    bool maxButton() const;
    bool minButton() const;
    std::wstring titleName() const;
    bool titleBar() const;
    std::wstring activeFunction() const;
    std::wstring closeFunction() const;
    HBRUSH backBrush() const;
    LuaMenubar* menu();

    void setControlBox(bool controlBox);
    void setMaxButton(bool maxButton);
    void setMinButton(bool minButton);
    void setTitleName(std::wstring title);
    void setTitleBar(bool titleBar);
    void setActiveFunction(std::wstring activeFunction);
    void setCloseFunction(std::wstring closeFunction);
    void setIcon(std::wstring iconFilePath);
    void setBackColor(COLORREF backColor);
    void setMenu(LuaMenubar* menu);

    /**
     * \brief Form window 을 생성합니다.
     */
    void create();

    /**
     * \brief 부모 Form 에 종속되어 종료될 때까지 부모 Form 의 조작을 금지하는 Form window 를 생성합니다.
     * \param parentForm 종속될 부모 Form control
     */
    void createModal(LuaForm& parentForm);

    /**
     * \brief createModal 함수를 통해 생성된 window 를 나타냅니다.
     * \return setDialogResult 함수로 설정된 값을 반환합니다.
     */
    int showModal();

    /**
     * \brief showModal 함수가 반환하는 값을 설정합니다.
     * \param value
     */
    void setDialogResult(int value) const;

    /**
     * \brief 생성된 Form window 를 강제로 소멸시킵니다.
     */
    virtual void destroy() override;

    /**
     * \brief Form 을 종료합니다.
     */
    void close() const;

    /**
     * \brief Form 을 다시 그립니다.
     */
    void refresh() const;

    static void registerToLua();
private:
    bool _maxButton = false;
    bool _minButton = true;
    bool _controlBox = true;
    bool _titleBar = true;
    int* _dialogResult = nullptr;
    std::wstring _titleName = L"";
    std::wstring _activeFunction = L"";
    std::wstring _closeFunction = L"";
    HICON _icon = LoadIcon(nullptr, IDI_APPLICATION);
    HBRUSH _backBrush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
    LuaMenubar* _menu;
};
}
