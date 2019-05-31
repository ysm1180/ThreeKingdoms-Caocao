#pragma once

#include <set>
#include <list>
#include <memory>
#include <mutex>

namespace jojogame
{
class CMemoryPoolBase
{
public:
    CMemoryPoolBase();
    virtual ~CMemoryPoolBase();

    virtual void Destroy() = 0;
};

class CMemoryPoolManager
{
public:
    CMemoryPoolManager();
    virtual ~CMemoryPoolManager();

    void RegisterMemoryPool(CMemoryPoolBase *pool);

    void DestroyAllMemoryPool();

    static CMemoryPoolManager &GetInstance();

private:
    std::set<CMemoryPoolBase *> _poolManager;

    static std::once_flag s_onceFlag;
    static std::unique_ptr<CMemoryPoolManager> s_sharedMemoryPoolManager;
};

template <typename T>
class CMemoryPool : public CMemoryPoolBase
{
public:
    struct ArrayMemory
    {
        T *pointer;
        int size;
    };

    CMemoryPool()
    {
    }

    virtual ~CMemoryPool()
    {
    }

    T *GetUnusingArrayPointer(int size)
    {
        T *instance = nullptr;

        if (_unusingArrayPool.empty())
        {
            instance = (T *)malloc(sizeof(T) * size);
        }
        else
        {
            int count = 0;
            int length = _unusingArrayPool.size();

            while (count < length)
            {
                ArrayMemory info = _unusingArrayPool.front();
                if (info.size >= size)
                {
                    delete[] info.pointer;
                    instance = info.pointer;
                    break;
                }
                _unusingArrayPool.pop_front();
                _unusingArrayPool.push_back(info);
                count++;
            }

            if (instance != nullptr)
            {
                instance = (T *)malloc(sizeof(T) * size);
            }
        }

        return instance;
    }

    T *GetUnusingPointer()
    {
        T *instance = nullptr;
        if (_unusingPool.empty())
        {
            instance = (T *)malloc(sizeof(T));
        }
        else
        {
            instance = _unusingPool.front();
            _unusingPool.pop_front();
        }

        return instance;
    }

    void Destroy() override
    {
        for (auto info : _usingArrayPool)
        {
            delete[] info.pointer;
        }
        _usingArrayPool.clear();

        for (auto v : _usingPool)
        {
            delete v;
        }
        _usingPool.clear();

        for (auto v : _unusingPool)
        {
            delete v;
        }
        _unusingPool.clear();
    }

    static CMemoryPool<T> &GetInstance()
    {
        static CMemoryPool<T> *s_sharedMemoryPool = nullptr;

        if (s_sharedMemoryPool == nullptr)
        {
            s_sharedMemoryPool = new CMemoryPool<T>();
            CMemoryPoolManager::GetInstance().RegisterMemoryPool(s_sharedMemoryPool);
        }
        return *s_sharedMemoryPool;
    }

public:
    T *Array(int size)
    {
        T *pointer = GetUnusingArrayPointer(size);

        new (pointer) T[size];
        _usingArrayPool.push_back(ArrayMemory{pointer, size});

        return pointer;
    }

    T *New()
    {
        T *pointer = GetUnusingPointer();

        _usingPool.push_back(new (pointer) T());

        return pointer;
    }

    template <typename T1>
    T *New(T1 t1)
    {
        T *pointer = GetUnusingPointer();

        _usingPool.push_back(new (pointer) T(t1));

        return pointer;
    }

    template <typename T1, typename T2>
    T *New(T1 t1, T2 t2)
    {
        T *pointer = GetUnusingPointer();

        _usingPool.push_back(new (pointer) T(t1, t2));

        return pointer;
    }

    template <typename T1, typename T2, typename T3>
    T *New(T1 t1, T2 t2, T3 t3)
    {
        T *pointer = GetUnusingPointer();

        _usingPool.push_back(new (pointer) T(t1, t2, t3));

        return pointer;
    }

    template <typename T1, typename T2, typename T3, typename T4>
    T *New(T1 t1, T2 t2, T3 t3, T4 t4)
    {
        T *pointer = GetUnusingPointer();

        _usingPool.push_back(new (pointer) T(t1, t2, t3, t4));

        return pointer;
    }

    void Delete(T *instance, bool isCallDestructor = true)
    {
        if (isCallDestructor)
        {
            instance->~T();
        }

        if (_limit < _unusingPool.size())
        {
            free(instance);
        }
        else
        {
            _usingPool.remove(instance);
            _unusingPool.push_back(instance);
        }
    }

private:
    std::list<ArrayMemory> _usingArrayPool;
    std::list<ArrayMemory> _unusingArrayPool;

    std::list<T *> _usingPool;
    std::list<T *> _unusingPool;
    size_t _limit = 65535;
};
} // namespace jojogame