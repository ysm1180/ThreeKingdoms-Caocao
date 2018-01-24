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
T * CMemoryPool<T>::GetUnsingPointer()
{
    T *instance = nullptr;
    if (_unusingPool.empty()) {
        instance = (T *)malloc(sizeof(T));
    }
    else {
        instance = _unusingPool.front();
        _unusingPool.pop_front();
    }

    return instance;
}

template<typename T>
void CMemoryPool<T>::Destroy()
{
    for (auto v : _usingPool) {
        delete v;
    }
    _usingPool.clear();

    for (auto v : _unusingPool) {
        delete v;
    }
    _unusingPool.clear();
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

template<typename T>
void CMemoryPool<T>::Delete(T * instance, bool isCallDestructor) {
    if (isCallDestructor) {
        instance->~T();
    }

    if (_limit < _unusingPool.size()) {
        free(instance);
    }
    else {
        _unusingPool.push_back(instance);
    }
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