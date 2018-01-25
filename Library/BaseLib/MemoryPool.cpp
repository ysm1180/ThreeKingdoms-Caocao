#include "stdafx.h"
#include "MemoryPool.h"

namespace jojogame {


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

void CMemoryPoolManager::RegisterMemoryPool(CMemoryPoolBase * pool)
{
    _poolManager.insert(pool);
}

void CMemoryPoolManager::DestroyAllMemoryPool()
{
    for (auto v : _poolManager) {
        v->Destroy();
        delete v;
    }

    _poolManager.clear();
}

CMemoryPoolManager & CMemoryPoolManager::GetInstance()
{
    std::call_once(s_onceFlag, [] {
        s_sharedMemoryPoolManager.reset(new CMemoryPoolManager);
    });

    return *s_sharedMemoryPoolManager.get();
}

}