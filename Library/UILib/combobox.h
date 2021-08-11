#pragma once
#include <vector>

#include "text_font.h"
#include "child_control.h"

namespace three_kingdoms {
class CComboBoxControl : public CWindowChildControl {
 public:
  static void RegisterFunctions(lua_State *L);

  CComboBoxControl();
  virtual ~CComboBoxControl();

  void AddItem(std::wstring item);

  bool Create() override;

  static WNDPROC GetOriginalProc();
  static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam,
                                        LPARAM lParam);

 private:
  CTextFont _font;
  std::vector<std::wstring> _items;

  static WNDPROC s_originalProc;
};
}  // namespace three_kingdoms
