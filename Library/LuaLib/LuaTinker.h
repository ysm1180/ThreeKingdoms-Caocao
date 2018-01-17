#pragma once

#include "lua_tinker.h"

namespace jojogame {
	class CLuaTinker {
	public:
		CLuaTinker();
		virtual ~CLuaTinker();
		
		template<typename T>
		void registerVariable(const char * name, T value)
		{
			lua_tinker::set(_luaState, name, value);
		}

		template<typename F>
		void registerFunction(const char * name, F func)
		{
			lua_tinker::def<F>(_luaState, name, func);
		}

		void call(const char * functionName)
		{
			lua_tinker::call<void>(_luaState, functionName);
		}

		template<typename RVal>
		RVal call(const char * functionName)
		{
			return lua_tinker::call<RVal>(_luaState, functionName);
		}

		template<typename RVal, typename T1>
		RVal call(const char * functionName, T1 arg1)
		{
			return lua_tinker::call<RVal>(_luaState, functionName, arg1);
		}

		template<typename RVal, typename T1, typename T2>
		RVal call(const char * functionName, T1 arg1, T2 arg2)
		{
			return lua_tinker::call<RVal>(_luaState, functionName, arg1, arg2);
		}

		template<typename RVal, typename T1, typename T2, typename T3>
		RVal call(const char * functionName, T1 arg1, T2 arg2, T3 arg3)
		{
			return lua_tinker::call<RVal>(_luaState, functionName, arg1, arg2, arg3);
		}

		template<typename RVal, typename T1, typename T2, typename T3, typename T4>
		RVal call(const char * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
		{
			return lua_tinker::call<RVal>(_luaState, functionName, arg1, arg2, arg3, arg4);
		}

		template<typename RVal, typename T1, typename T2, typename T3, typename T4, typename T5>
		RVal call(const char * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
		{
			return lua_tinker::call<RVal>(_luaState, functionName, arg1, arg2, arg3, arg4, arg5);
		}

		template<typename RVal, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		RVal call(const char * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6)
		{
			return lua_tinker::call<RVal>(_luaState, functionName, arg1, arg2, arg3, arg4, arg5, arg6);
		}

		template<typename RVal, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		RVal call(const char * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7)
		{
			return lua_tinker::call<RVal>(_luaState, functionName, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
		}

		template<typename RVal, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		RVal call(const char * functionName, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8)
		{
			return lua_tinker::call<RVal>(_luaState, functionName, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
		}

		static CLuaTinker* getLuaTinker();
	protected:
		lua_State * _luaState;
	};
}