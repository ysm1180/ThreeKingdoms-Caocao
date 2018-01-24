#pragma once

#include <set>
#include <list>
#include <memory>
#include <mutex>


namespace jojogame {

class CMemoryPoolBase {
public:
    CMemoryPoolBase();
    virtual ~CMemoryPoolBase();

    virtual void Destroy() = 0;
};

template <typename T>
class CMemoryPool : public CMemoryPoolBase
{
public:
    CMemoryPool();
    virtual ~CMemoryPool();

    T* GetUnsingPointer();
    void Destroy() override;

    static CMemoryPool<T>& GetInstance();

public:
    T* New() {
        T *pointer = CMemoryPool<T>::GetInstance().GetUnsingPointer();

        _usingPool.push_back(new (pointer) T());

        return pointer;
    }

    template <typename T1>
    T* New(T1 t1) {
        T *pointer = CMemoryPool<T>::GetInstance().GetUnsingPointer();

        _usingPool.insert(new (pointer) T(t1));

        return pointer;
    }

    template <typename T1, typename T2>
    T* New(T1 t1, T2 t2) {
        T *pointer = CMemoryPool<T>::GetInstance().GetUnsingPointer();

        _usingPool.insert(new (pointer) T(t1, t2));

        return pointer;
    }

    template <typename T1, typename T2, typename T3>
    T* New(T1 t1, T2 t2, T3 t3) {
        T *pointer = CMemoryPool<T>::GetInstance().GetUnsingPointer();

        _usingPool.insert(new (pointer) T(t1, t2, t3));

        return pointer;
    }

    template <typename T1, typename T2, typename T3, typename T4>
    T* New(T1 t1, T2 t2, T3 t3, T4 t4) {
        T *pointer = CMemoryPool<T>::GetInstance().GetUnsingPointer();

        _usingPool.insert(new (pointer) T(t1, t2, t3, t4));

        return pointer;
    }

    void Delete(T *instance, bool isCallDestructor = true);


protected:
    std::list<T *> _usingPool;
    std::list<T *> _unusingPool;
    int _limit = 65535;

    static std::once_flag s_onceFlag;
    static std::unique_ptr<CMemoryPool<T>> s_sharedMemoryPool;
};



class CMemoryPoolManager {
public:
    CMemoryPoolManager();
    virtual ~CMemoryPoolManager();

    void RegisterMemoryPool(CMemoryPoolBase *pool);
    
    void DestroyAllMemoryPool();

    static CMemoryPoolManager& GetInstance();
private:
    std::set<CMemoryPoolBase *> _poolManager;

    static std::once_flag s_onceFlag;
    static std::unique_ptr<CMemoryPoolManager> s_sharedMemoryPoolManager;
};


}