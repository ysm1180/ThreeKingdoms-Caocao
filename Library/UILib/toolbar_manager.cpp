#include "toolbar_manager.h"

#include "toolbar.h"

namespace three_kingdoms {
std::once_flag CToolbarManager::s_onceFlag;
std::unique_ptr<CToolbarManager> CToolbarManager::s_sharedToolbarManager;

CToolbarManager::CToolbarManager() {}

CToolbarManager::~CToolbarManager() {}

CToolbarManager &CToolbarManager::GetInstance() {
  std::call_once(s_onceFlag, [] {
    s_sharedToolbarManager =
        std::make_unique<three_kingdoms::CToolbarManager>();
  });

  return *s_sharedToolbarManager;
}

int CToolbarManager::_GetNewIndex() {
  if (_reusingIndexStorage.empty()) {
    const auto index = static_cast<int>(TOOLBAR_INDEX::START_INDEX) +
                       _toolbarButtonStorage.size();
    if (index > static_cast<int>(TOOLBAR_INDEX::END_INDEX)) {
      return static_cast<int>(TOOLBAR_INDEX::FULL_INDEX);
    }
    if (index < static_cast<int>(TOOLBAR_INDEX::START_INDEX)) {
      return static_cast<int>(TOOLBAR_INDEX::START_INDEX);
    }
    return index;
  }

  auto result = _reusingIndexStorage.front();
  _reusingIndexStorage.pop();
  return result;
}

CToolbarButton *CToolbarManager::GetToolbarButton(int id) {
  auto iter = _toolbarButtonStorage.find(id);
  if (iter != _toolbarButtonStorage.end()) {
    return iter->second;
  } else {
    return nullptr;
  }
}

int CToolbarManager::AddToolbarButton(CToolbarButton *item) {
  int index = _GetNewIndex();

  if (index == static_cast<int>(TOOLBAR_INDEX::FULL_INDEX)) {
    return 0;
  }

  _toolbarButtonStorage.insert(
      std::map<int, CToolbarButton *>::value_type(index, item));
  return index;
}

void CToolbarManager::DeleteToolbarButton(CToolbarButton *button) {
  _reusingIndexStorage.push(button->GetIndex());
  _toolbarButtonStorage.erase(button->GetIndex());
}
}  // namespace three_kingdoms