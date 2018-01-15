#pragma once
#include "base/Ref.h"

namespace jojogame {
class __declspec(dllexport) LuaTime : public Ref
{
public:
    LuaTime();
    ~LuaTime();

    void delay(int);
    void stopDelay();

    static LuaTime* getInstance();
private:
    static LuaTime* s_sharedTime;
};
}
