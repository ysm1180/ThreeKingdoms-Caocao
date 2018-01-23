#include "stdafx.h"
#include "MemoryPool.h"


namespace jojogame {
CMemoryPoolBase::CMemoryPoolBase()
{
}
CMemoryPoolBase::~CMemoryPoolBase()
{
}


template<typename T> std::once_flag CMemoryPool<T>::s_onceFlag;
template<typename T> std::unique_ptr<CMemoryPool<T>> CMemoryPool<T>::s_sharedMemoryPool;

template <typename T>
CMemoryPool<T>::CMemoryPool()
{
}

template <typename T>
CMemoryPool<T>::~CMemoryPool()
{
}

template<typename T>
CMemoryPool<T>& CMemoryPool<T>::GetInstance()
{
    std::call_once(s_onceFlag, [] {
        s_sharedMemoryPool.reset(new CMemoryPool<T>);
        CMemoryPoolManager::GetInstance().RegisterMemoryPool(s_sharedMemoryPool.get());
    });

    return *s_sharedMemoryPool.get();
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

void CMemoryPoolManager::DestroyMemoryPools()
{
    for (auto v : _poolManager) {
        v->Destory();
    }
}

CMemoryPoolManager & CMemoryPoolManager::GetInstance()
{
    std::call_once(s_onceFlag, [] {
        s_sharedMemoryPoolManager.reset(new CMemoryPoolManager);
    });

    return *s_sharedMemoryPoolManager.get();
}

}