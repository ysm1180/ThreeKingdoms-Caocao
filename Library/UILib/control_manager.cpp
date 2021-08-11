#include "control_manager.h"

#include "audio_player.h"
#include "base_control.h"
#include "BaseLib\memory_pool.h"
#include "button.h"
#include "checkbox.h"
#include "combobox.h"
#include "graphic_rect.h"
#include "graphic_text.h"
#include "groupbox.h"
#include "image.h"
#include "layout.h"
#include "listview.h"
#include "menu.h"
#include "movie_player.h"
#include "radio.h"
#include "static.h"
#include "text_font.h"
#include "toolbar.h"
#include "child_control.h"
#include "window.h"

namespace three_kingdoms {
std::once_flag CControlManager::s_onceFlag;
std::unique_ptr<CControlManager> CControlManager::s_controlManager;

void CControlManager::RegisterFunctions(lua_State *L) {
  LUA_BEGIN(CControlManager, "_ControlManager");

  LUA_METHOD(CreateWindowForm);
  LUA_METHOD(CreateMoviePlayer);
  LUA_METHOD(CreateButton);
  LUA_METHOD(CreateMenu);
  LUA_METHOD(CreateMenuItem);
  LUA_METHOD(CreateImage);
  LUA_METHOD(CreateToolbar);
  LUA_METHOD(CreateToolbarButton);
  LUA_METHOD(CreateLayout);
  LUA_METHOD(CreateListView);
  LUA_METHOD(CreateListViewColumn);
  LUA_METHOD(CreateListViewRow);
  LUA_METHOD(CreateListViewItem);
  LUA_METHOD(CreateStatic);
  LUA_METHOD(CreateGroupBox);
  LUA_METHOD(CreateCheckbox);
  LUA_METHOD(CreateRadioButton);
  LUA_METHOD(CreateComboBox);
  LUA_METHOD(CreateAudioPlayer);
  LUA_METHOD(CreateGraphicText);
  LUA_METHOD(CreateGraphicRect);
}

CControlManager::CControlManager() {}

CControlManager::~CControlManager() {}

void CControlManager::Init(HINSTANCE hInstance) {
  _hInstance = hInstance;

  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CControlManager>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CBaseControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CWindowChildControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CTextFont>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CWindowControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CMoviePlayerControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CButtonControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CMenu>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CMenuItem>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CImageControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CToolbarControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CToolbarButton>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CLayoutControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CListViewControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CListViewColumn>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CListViewRow>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CListViewItem>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CStaticControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CGroupBoxControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CCheckboxControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CRadioButtonControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CComboBoxControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CAudioPlayerControl>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CGraphicText>();
  LuaTinkerHelper::GetLuaTinker().RegisterClassToLua<CGraphicRect>();
}

CWindowControl *CControlManager::CreateWindowForm(CWindowControl *parent) {
  auto control = CMemoryPool<CWindowControl>::GetInstance().New();
  control->SetParentWindow(parent);
  return control;
}

CAudioPlayerControl *CControlManager::CreateAudioPlayer() {
  auto control = CMemoryPool<CAudioPlayerControl>::GetInstance().New();
  return control;
}

CGraphicText *CControlManager::CreateGraphicText() {
  auto control = CMemoryPool<CGraphicText>::GetInstance().New();
  return control;
}

CGraphicRect *CControlManager::CreateGraphicRect() {
  auto control = CMemoryPool<CGraphicRect>::GetInstance().New();
  return control;
}

CMoviePlayerControl *CControlManager::CreateMoviePlayer(CWindowControl *parent,
                                                        std::wstring fileName) {
  auto control =
      CMemoryPool<CMoviePlayerControl>::GetInstance().New(parent, fileName);
  return control;
}

CButtonControl *CControlManager::CreateButton(CWindowControl *parent) {
  auto control = CMemoryPool<CButtonControl>::GetInstance().New();
  control->SetParentWindow(parent);
  return control;
}

CMenu *CControlManager::CreateMenu() {
  return CMemoryPool<CMenu>::GetInstance().New();
}

CMenuItem *CControlManager::CreateMenuItem() {
  return CMemoryPool<CMenuItem>::GetInstance().New();
  ;
}

CImageControl *CControlManager::CreateImage() {
  return CMemoryPool<CImageControl>::GetInstance().New();
}

CToolbarControl *CControlManager::CreateToolbar() {
  return CMemoryPool<CToolbarControl>::GetInstance().New();
}

CToolbarButton *CControlManager::CreateToolbarButton() {
  return CMemoryPool<CToolbarButton>::GetInstance().New();
}

CLayoutControl *CControlManager::CreateLayout() {
  auto control = CMemoryPool<CLayoutControl>::GetInstance().New();
  _layouts.push_back(control);
  return control;
}

CListViewControl *CControlManager::CreateListView(CWindowControl *parent) {
  auto control = CMemoryPool<CListViewControl>::GetInstance().New();
  control->SetParentWindow(parent);
  return control;
}

CListViewColumn *CControlManager::CreateListViewColumn() {
  auto control = CMemoryPool<CListViewColumn>::GetInstance().New();
  return control;
}

CListViewRow *CControlManager::CreateListViewRow() {
  auto control = CMemoryPool<CListViewRow>::GetInstance().New();
  return control;
}

CListViewItem *CControlManager::CreateListViewItem() {
  auto control = CMemoryPool<CListViewItem>::GetInstance().New();
  return control;
}

CStaticControl *CControlManager::CreateStatic(CWindowControl *parent) {
  auto control = CMemoryPool<CStaticControl>::GetInstance().New();
  control->SetParentControl(parent);
  return control;
}

CGroupBoxControl *CControlManager::CreateGroupBox(CWindowControl *parent) {
  auto control = CMemoryPool<CGroupBoxControl>::GetInstance().New();
  control->SetParentControl(parent);
  return control;
}

CCheckboxControl *CControlManager::CreateCheckbox(CWindowControl *parent) {
  auto control = CMemoryPool<CCheckboxControl>::GetInstance().New();
  control->SetParentControl(parent);
  return control;
}

CRadioButtonControl *CControlManager::CreateRadioButton(CWindowControl *parent,
                                                        bool isGroupStart) {
  auto control =
      CMemoryPool<CRadioButtonControl>::GetInstance().New(isGroupStart);
  control->SetParentControl(parent);
  return control;
}

CComboBoxControl *CControlManager::CreateComboBox(CWindowControl *parent) {
  auto control = CMemoryPool<CComboBoxControl>::GetInstance().New();
  control->SetParentControl(parent);
  return control;
}

std::vector<CLayoutControl *> CControlManager::GetLayouts() { return _layouts; }

HINSTANCE CControlManager::GetHInstance() { return _hInstance; }

CControlManager &CControlManager::GetInstance() {
  std::call_once(s_onceFlag, [] {
    s_controlManager = std::make_unique<three_kingdoms::CControlManager>();
  });

  return *s_controlManager;
}
}  // namespace three_kingdoms