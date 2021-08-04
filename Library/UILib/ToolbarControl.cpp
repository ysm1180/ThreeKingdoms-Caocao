#include "ToolbarControl.h"

#include "ControlManager.h"
#include "ImageControl.h"
#include "ToolbarManager.h"
#include "WindowControl.h"

namespace three_kingdoms {
void CToolbarControl::RegisterFunctions(lua_State *L) {
  LUA_BEGIN(CToolbarControl, "_ToolbarControl");

  LUA_METHOD(GetHeight);

  LUA_METHOD(Create);
  LUA_METHOD(AddButton);
  LUA_METHOD(DeleteButton);

  LUA_METHOD(AutoSize);

  LUA_METHOD(Show);
  LUA_METHOD(Hide);
}

CToolbarControl::CToolbarControl() {}

CToolbarControl::~CToolbarControl() {
  if (_hWnd != nullptr) {
    DestroyWindow(_hWnd);
    _hWnd = nullptr;
  }
  if (_hImageList) {
    ImageList_Destroy(_hImageList);
    _hImageList = nullptr;
  }
}

HWND CToolbarControl::GetHWnd() const { return _hWnd; }

int CToolbarControl::GetHeight() const {
  if (_isVisible) {
    RECT rect;
    GetWindowRect(_hWnd, &rect);

    return rect.bottom - rect.top;
  }

  return 0;
}

bool CToolbarControl::Create(CWindowControl *parentWindow, int imageWidth,
                             int imageHeight) {
  if (parentWindow) {
    _parentWindow = parentWindow;
    _hWnd = CreateWindowEx(
        0, TOOLBARCLASSNAME, nullptr,
        WS_CHILD | TBSTYLE_WRAPABLE | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS, 0, 0,
        0, 0, _parentWindow->GetHWnd(), (HMENU)this,
        CControlManager::GetInstance().GetHInstance(), nullptr);
    _hImageList = ImageList_Create(imageWidth, imageHeight,
                                   ILC_MASK | ILC_COLOR24, 16, 4);
    SendMessage(_hWnd, TB_SETIMAGELIST, (WPARAM)0, (LPARAM)_hImageList);

    _parentWindow->SetToolbar(this);
    AutoSize();

    _buttons.clear();
    _imageList.clear();

    return _hWnd != nullptr;
  }

  return false;
}

void CToolbarControl::AddButton(CToolbarButton *button) {
  auto index = CToolbarManager::GetInstance().AddToolbarButton(button);

  if (index != static_cast<int>(TOOLBAR_INDEX::FULL_INDEX)) {
    button->SetIndex(index);
    if (button->GetImage()) {
      int imageIndex;

      auto iter =
          std::find(_imageList.begin(), _imageList.end(), button->GetImage());
      if (iter == _imageList.end()) {
        _imageList.push_back(button->GetImage());
        imageIndex = _imageList.size() - 1;
        ImageList_Add(_hImageList, button->GetImage()->GetImageHandle(),
                      button->GetImage()->GetMaskImageHandle());
      } else {
        imageIndex = std::distance(_imageList.begin(), iter);
      }
      button->SetImageIndex(imageIndex);
    }
    _buttons.push_back(button);

    SendMessage(_hWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(_hWnd, TB_ADDBUTTONS, (WPARAM)1,
                (LPARAM)&button->GetButtonStruct());

    AutoSize();

    button->SetParentToolbar(this);
  }
}

void CToolbarControl::DeleteButton(CToolbarButton *button) {
  auto iter = std::begin(_buttons);

  while (iter != std::end(_buttons)) {
    if (*iter == button) {
      iter = _buttons.erase(iter);
      break;
    } else {
      ++iter;
    }
  }
  CToolbarManager::GetInstance().DeleteToolbarButton(button);

  SendMessage(_hWnd, TB_DELETEBUTTON, (WPARAM)button->GetIndex(), (LPARAM)0);
}

void CToolbarControl::AutoSize() {
  int height = GetHeight();
  SendMessage(_hWnd, TB_AUTOSIZE, 0, 0);
  int delta = height - _prevHeight;
  _prevHeight = height;
  _parentWindow->SetHeight(_parentWindow->GetHeight() + delta);
}

void CToolbarControl::Show() {
  _isVisible = true;
  ::ShowWindow(_hWnd, TRUE);
  this->AutoSize();
}

void CToolbarControl::Hide() {
  _isVisible = false;
  ::ShowWindow(_hWnd, FALSE);
  this->AutoSize();
}

void CToolbarButton::RegisterFunctions(lua_State *L) {
  LUA_BEGIN(CToolbarButton, "_ToolbarButton");

  LUA_METHOD(IsEnabled);
  LUA_METHOD(GetText);

  LUA_METHOD(SetEnabled);
  LUA_METHOD(SetText);
  LUA_METHOD(SetImage);
  LUA_METHOD(SetClickEvent);
  LUA_METHOD(SetTooltipText);

  LUA_METHOD(Create);
}

CToolbarButton::CToolbarButton() { _state = TBSTATE_ENABLED; }

CToolbarButton::~CToolbarButton() {
  if (_clickEvent != LUA_NOREF) {
    luaL_unref(CLuaTinker::GetLuaTinker().GetLuaState(), LUA_REGISTRYINDEX,
               _clickEvent);
  }
}

bool CToolbarButton::IsEnabled() { return _isEnabled; }

int CToolbarButton::GetIndex() { return _button.idCommand; }

TBBUTTON &CToolbarButton::GetButtonStruct() { return _button; }

CImageControl *CToolbarButton::GetImage() { return _image; }

std::wstring CToolbarButton::GetText() { return _text; }

int CToolbarButton::GetClickEvent() { return _clickEvent; }

std::wstring CToolbarButton::GetTooltipText() { return _tooltipText; }

CToolbarControl *CToolbarButton::GetParentToolbar() { return _parent; }

void CToolbarButton::SetEnabled(bool isEnabled) {
  _isEnabled = isEnabled;

  if (_isEnabled) {
    _state = _state | TBSTATE_ENABLED;
  } else {
    _state = _state & ~TBSTATE_ENABLED;
  }

  if (_parent) {
    SendMessage(_parent->GetHWnd(), TB_SETSTATE, _button.idCommand, _state);
  }
}

void CToolbarButton::SetIndex(int index) { _button.idCommand = index; }

void CToolbarButton::SetImageIndex(int imageIndex) {
  _button.iBitmap = MAKELONG(imageIndex, 0);
}

void CToolbarButton::SetText(std::wstring text) {
  _text = text;

  if (_text.length() == 0) {
    _button.iString = (INT_PTR) nullptr;
  } else {
    _button.iString = (INT_PTR)_text.c_str();
  }
}

void CToolbarButton::SetImage(CImageControl *image) { _image = image; }

bool CToolbarButton::Create() {
  _button = {0, 0, _state, BTNS_AUTOSIZE, {0}, 0, 0};

  if (_text.length() != 0) {
    _button.iString = (INT_PTR)_text.c_str();
  }

  return true;
}

void CToolbarButton::Show() {
  _state = _state & ~TBSTATE_HIDDEN;
  if (_parent) {
    SendMessage(_parent->GetHWnd(), TB_SETSTATE, _button.idCommand, _state);
  }
}

void CToolbarButton::Hide() {
  _state = _state | TBSTATE_HIDDEN;
  if (_parent) {
    SendMessage(_parent->GetHWnd(), TB_SETSTATE, _button.idCommand, _state);
  }
}

void CToolbarButton::SetClickEvent() {
  auto l = CLuaTinker::GetLuaTinker().GetLuaState();
  if (lua_isfunction(l, -1)) {
    lua_pushvalue(l, -1);
    _clickEvent = luaL_ref(l, LUA_REGISTRYINDEX);
  }

  lua_pop(l, 1);
}

void CToolbarButton::SetTooltipText(std::wstring tooltipText) {
  _tooltipText = tooltipText;
}

void CToolbarButton::SetParentToolbar(CToolbarControl *parent) {
  _parent = parent;
}
}  // namespace three_kingdoms