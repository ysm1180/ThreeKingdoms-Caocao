#include "memory_pool.h"

namespace three_kingdoms {
MemoryPoolBase::MemoryPoolBase() {}

MemoryPoolBase::~MemoryPoolBase() {}

std::once_flag MemoryPoolManager::s_onceFlag;
std::unique_ptr<MemoryPoolManager>
    MemoryPoolManager::s_sharedMemoryPoolManager;

MemoryPoolManager::MemoryPoolManager() {}

MemoryPoolManager::~MemoryPoolManager() {}

void MemoryPoolManager::RegisterMemoryPool(MemoryPoolBase *pool) {
  _poolManager.insert(pool);
}

void MemoryPoolManager::DestroyAllMemoryPool() {
  for (auto v : _poolManager) {
    v->Destroy();
    delete v;
  }

  _poolManager.clear();
}

MemoryPoolManager &MemoryPoolManager::GetInstance() {
  std::call_once(s_onceFlag, [] {
    s_sharedMemoryPoolManager =
        std::make_unique<three_kingdoms::MemoryPoolManager>();
  });

  return *s_sharedMemoryPoolManager;
}
}  // namespace three_kingdoms