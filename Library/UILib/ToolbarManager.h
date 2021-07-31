#pragma once

#include <memory>
#include <mutex>
#include <map>
#include <queue>

namespace three_kingdoms
{
enum class TOOLBAR_INDEX
{
    FULL_INDEX = -1,
    START_INDEX = 0x100,
    END_INDEX = 0x1FF
};

class CToolbarButton;

class CToolbarManager
{
public:
    CToolbarManager();
    ~CToolbarManager();

    CToolbarButton *GetToolbarButton(int id);

    int AddToolbarButton(CToolbarButton *button);

    void DeleteToolbarButton(CToolbarButton *button);

    static CToolbarManager &GetInstance();

private:
    int _GetNewIndex();

    std::map<int, CToolbarButton *> _toolbarButtonStorage;
    std::queue<int> _reusingIndexStorage;

    static std::once_flag s_onceFlag;
    static std::unique_ptr<CToolbarManager> s_sharedToolbarManager;
};
} // namespace jojogame
