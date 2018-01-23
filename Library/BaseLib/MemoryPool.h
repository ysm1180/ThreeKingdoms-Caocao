#pragma once

#include <set>
#include <memory>
#include <mutex>


namespace jojogame {

class CMemoryPoolBase {
public:
    CMemoryPoolBase();
    virtual ~CMemoryPoolBase();

    virtual void Destory();
};

template <typename T>
class CMemoryPool : public CMemoryPoolBase
{
public:
    CMemoryPool();
    virtual ~CMemoryPool();

    static CMemoryPool<T>& GetInstance();

    T* New() {
        T *instance = _unusingPool.front();
        T *pointer = new T();
        _usingPool.insert(pointer);
        return pointer;
    }

    template <typename T1>
    T* New(T1 t1) {
        T *pointer = new T(t1);
        _usingPool.insert(pointer);
        return pointer;
    }

    template <typename T1, typename T2>
    T* New(T1 t1, T2 t2) {

    }

    template <typename T1, typename T2, typename T3>
    T* New(T1 t1, T2 t2, T3 t3) {

    }

    template <typename T1, typename T2, typename T3, typename T4>
    T* New(T1 t1, T2 t2, T3 t3, T4 t4) {

    }


protected:
    std::set<T *> _usingPool;
    std::set<T *> _unusingPool;

    static std::once_flag s_onceFlag;
    static std::unique_ptr<CMemoryPool<T>> s_sharedMemoryPool;
};

class CMemoryPoolManager {
public:
    CMemoryPoolManager();
    virtual ~CMemoryPoolManager();

    void RegisterMemoryPool(CMemoryPoolBase *pool);
    
    void DestroyMemoryPools();

    static CMemoryPoolManager& GetInstance();
private:
    std::set<CMemoryPoolBase *> _poolManager;

    static std::once_flag s_onceFlag;
    static std::unique_ptr<CMemoryPoolManager> s_sharedMemoryPoolManager;
};


}