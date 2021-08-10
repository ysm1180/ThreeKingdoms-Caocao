#pragma once

#include <Windows.h>

#include <map>
#include <memory>
#include <mutex>
#include <queue>

#include "LuaLib/lua_tinker_helper.h"

namespace three_kingdoms {
enum MENU_INDEX { FULL_INDEX = -1, START_INDEX = 1, END_INDEX = 0xFF };

class CMenuItem;

class CMenuManager {
 public:
  static void RegisterFunctions(lua_State *L);

  CMenuManager();
  ~CMenuManager();

  int AddMenuItemByHandle(CMenuItem *item, HMENU handle);
  int AddMenuItem(CMenuItem *item);

  CMenuItem *GetMenuItem(int id);

  void DeleteMenuItem(CMenuItem *item);

  static CMenuManager &GetInstance();

 private:
  int _GetNewIndex();

  std::map<int, CMenuItem *> _menuItemStorage;
  std::queue<int> _reusingIndexStorage;

  static std::once_flag s_onceFlag;
  static std::unique_ptr<CMenuManager> s_sharedMenuManager;
};
}  // namespace three_kingdoms
