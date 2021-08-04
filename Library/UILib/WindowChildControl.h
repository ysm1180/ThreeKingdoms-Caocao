#pragma once
#include "BaseControl.h"

namespace three_kingdoms {
class CWindowControl;

class CWindowChildControl : public CBaseControl {
 public:
  static void RegisterFunctions(lua_State *L);

  int GetLuaY() const;
  CWindowControl *GetParentWindow() const;

  void SetLuaY(int y);
};
}  // namespace three_kingdoms
