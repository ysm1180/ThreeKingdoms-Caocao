#pragma once

#include "LuaControl.h"

namespace jojogame {
class CWindow;

class LuaMovie : public CControl
{
public:
    explicit LuaMovie(CWindow *parent);
    virtual ~LuaMovie();

    WNDPROC oldProc();

    void play();
    void stop();

    void create(std::wstring fileName);
    void destroy() override;

    static void registerToLua();
private:
    WNDPROC _oldProc;
};
}
