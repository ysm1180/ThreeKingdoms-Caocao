#pragma once

#include "text_font.h"
#include "child_control.h"

#include <uxtheme.h>

namespace three_kingdoms {
class CRadioButtonControl : public CWindowChildControl {
 public:
  static void RegisterFunctions(lua_State *L);

  explicit CRadioButtonControl(bool isGroupStart);
  virtual ~CRadioButtonControl();

  bool IsChecked();
  std::wstring GetText();
  CTextFont *GetFont();
  HTHEME GetTheme();

  void SetText(std::wstring text);
  void SetChecked(bool checked);

  bool Create() override;

  static WNDPROC GetOriginalProc();
  static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam,
                                        LPARAM lParam);

 private:
  CTextFont _font;
  std::wstring _text;
  HTHEME _theme;
  bool _isChecked = false;

  static WNDPROC s_originalProc;
};
}  // namespace three_kingdoms
