#pragma once


namespace jojogame {
class CBaseControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CBaseControl();
    virtual ~CBaseControl();

   

    virtual bool IsEnabled() const;
    virtual bool IsVisible() const;
    virtual int GetWidth() const;
    virtual int GetHeight() const;
    virtual int GetX() const;
    virtual int GetY() const;
    virtual LONG GetStyle() const;
    virtual std::wstring GetCreateEvent() const;
    virtual std::wstring GetDestroyEvent() const;
    virtual std::wstring GetClickEvent() const;
    virtual HWND GetHWnd() const;

    virtual void SetStyle(LONG style);
    virtual void SetVisible(bool isVisible);
    virtual void SetEnabled(bool isEnabled);
    virtual void SetY(int x);
    virtual void SetX(int y);
    virtual void SetWidth(int width);
    virtual void SetHeight(int height);
    virtual void SetCreateEvent(std::wstring createEventName);
    virtual void SetDestroyEvent(std::wstring destroyEventName);
    virtual void SetClickEvent(std::wstring clickEventName);

    virtual void Show();
    virtual void Hide();

    virtual void MoveToCenter();

    virtual void Create() = 0;
    virtual void Destroy() = 0;

    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { return ::DefWindowProc(hWnd, msg, wParam, lParam); }
protected:
    bool _isVisible = false;
    bool _isEnabled = true;
    HWND _hWnd = nullptr;
    HWND _parentHWnd = nullptr;
    SIZE _size;
    POINT _position;
    LONG _style = 0;

    std::wstring _createEvent = L"";
    std::wstring _destroyEvent = L"";
    std::wstring _clickEvent = L"";
};
}
