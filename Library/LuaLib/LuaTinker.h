#pragma once

#define LUA_BEGIN(classname, nameInLua) \
	typedef classname current_class; \
	lua_tinker::class_add<classname>(L, nameInLua);	\

#define LUA_BEGIN_CHILD(classname, nameInLua, baseclassname) \
	typedef classname current_class; \
	lua_tinker::class_add<classname>(L, nameInLua);	\
	lua_tinker::class_inh<classname, baseclassname>(L);	\

#define LUA_METHOD(func) \
	lua_tinker::class_def<current_class>(L, #func, &current_class::func);

#include "lua_tinker.h"

#include <memory>
#include <mutex>

namespace jojogame {
class CLuaTinker
{
public:
    CLuaTinker();
    virtual ~CLuaTinker();

    CLuaTinker(const CLuaTinker& src) = delete;
    CLuaTinker& operator=(const CLuaTinker& rhs) = delete;

    lua_State* GetLuaState();
    void Run(const char *fileName);

    static CLuaTinker& GetLuaTinker();
public:
    template<typename T>
    void RegisterClassToLua()
    {
        T::RegisterFunctions(_luaState);
    }

    template<typename T>
    void RegisterVariable(const char * name, T value)
    {
        lua_tinker::set(_luaState, name, value);
    }

    template<typename F>
    void RegisterFunction(const char * name, F func)
    {
        lua_tinker::def<F>(_luaState, name, func);
    }

    void Call(const wchar_t * functionName)
    {
        lua_tinker::call<void>(_luaState, functionName);
    }

    template<typename T1>
    void Call(const wchar_t * functionName, T1 arg1)
    {
        return lua_tinker::call<void>(_luaState, functionName, arg1);
    }

    template< typename T1, typename T2>
    void Call(const wchar_t * functionName, T1 arg1, T2 arg2)
    {
        return lua_tinker::call<void>(_luaState, functionName, arg1, arg2);
    }

    template<typename T1, typename T2, typename T3>
    void Call(const wchar_t * functionName, T1 arg1, T2 arg2, T3 arg3)
    {
        return lua_tinker::call<void>(_luaState, functionName, arg1, arg2, arg3);
    }

    template<typename T1, typename T2, typename T3, typename T4>
    void Call(const wchar_t * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
    {
        return lua_tinker::call<void>(_luaState, functionName, arg1, arg2, arg3, arg4);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    void Call(const wchar_t * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
    {
        return lua_tinker::call<void>(_luaState, functionName, arg1, arg2, arg3, arg4, arg5);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    void Call(const wchar_t * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6)
    {
        return lua_tinker::call<void>(_luaState, functionName, arg1, arg2, arg3, arg4, arg5, arg6);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    void Call(const wchar_t * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7)
    {
        return lua_tinker::call<void>(_luaState, functionName, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    void Call(const wchar_t * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8)
    {
        return lua_tinker::call<void>(_luaState, functionName, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    }

    template<typename RVal>
    RVal Call(const wchar_t * functionName)
    {
        return lua_tinker::call<RVal>(_luaState, functionName);
    }

    template<typename RVal, typename T1>
    RVal Call(const wchar_t * functionName, T1 arg1)
    {
        return lua_tinker::call<RVal>(_luaState, functionName, arg1);
    }

    template<typename RVal, typename T1, typename T2>
    RVal Call(const wchar_t * functionName, T1 arg1, T2 arg2)
    {
        return lua_tinker::call<RVal>(_luaState, functionName, arg1, arg2);
    }

    template<typename RVal, typename T1, typename T2, typename T3>
    RVal Call(const wchar_t * functionName, T1 arg1, T2 arg2, T3 arg3)
    {
        return lua_tinker::call<RVal>(_luaState, functionName, arg1, arg2, arg3);
    }

    template<typename RVal, typename T1, typename T2, typename T3, typename T4>
    RVal Call(const wchar_t * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
    {
        return lua_tinker::call<RVal>(_luaState, functionName, arg1, arg2, arg3, arg4);
    }

    template<typename RVal, typename T1, typename T2, typename T3, typename T4, typename T5>
    RVal Call(const wchar_t * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
    {
        return lua_tinker::call<RVal>(_luaState, functionName, arg1, arg2, arg3, arg4, arg5);
    }

    template<typename RVal, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    RVal Call(const wchar_t * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6)
    {
        return lua_tinker::call<RVal>(_luaState, functionName, arg1, arg2, arg3, arg4, arg5, arg6);
    }

    template<typename RVal, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    RVal Call(const wchar_t * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7)
    {
        return lua_tinker::call<RVal>(_luaState, functionName, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }

    template<typename RVal, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    RVal Call(const wchar_t * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8)
    {
        return lua_tinker::call<RVal>(_luaState, functionName, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    }

protected:
    lua_State * _luaState = nullptr;

    static std::once_flag s_onceFlag;
    static std::unique_ptr<CLuaTinker> s_luaTinker;
};
}