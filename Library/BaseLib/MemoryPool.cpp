#include "MemoryPool.h"

namespace three_kingdoms
{
CMemoryPoolBase::CMemoryPoolBase()
{
}

CMemoryPoolBase::~CMemoryPoolBase()
{
}

std::once_flag CMemoryPoolManager::s_onceFlag;
std::unique_ptr<CMemoryPoolManager> CMemoryPoolManager::s_sharedMemoryPoolManager;

CMemoryPoolManager::CMemoryPoolManager()
{
}

CMemoryPoolManager::~CMemoryPoolManager()
{
}

void CMemoryPoolManager::RegisterMemoryPool(CMemoryPoolBase *pool)
{
    _poolManager.insert(pool);
}

void CMemoryPoolManager::DestroyAllMemoryPool()
{
    for (auto v : _poolManager)
    {
        v->Destroy();
        delete v;
    }

    _poolManager.clear();
}

CMemoryPoolManager &CMemoryPoolManager::GetInstance()
{
    std::call_once(s_onceFlag, [] {
        s_sharedMemoryPoolManager = std::make_unique<three_kingdoms::CMemoryPoolManager>();
    });

    return *s_sharedMemoryPoolManager;
}
} // namespace jojogame