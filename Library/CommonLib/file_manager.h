#pragma once
#include <memory>
#include <mutex>

#include "BaseLib/binary_file.h"
#include "LuaLib/lua_tinker_helper.h"

namespace three_kingdoms {
class CFileManager {
 public:
  static void RegisterFunctions(lua_State *L);

  BinaryFile *LoadFile(std::wstring path);
  char ReadByte(BinaryFile *file, int position);
  void WriteByte(BinaryFile *file, int position, char byte);
  void CloseFile(BinaryFile *file);
  void SetWorkingPath(std::wstring dir);

  std::wstring GetWorkingPath();
  std::wstring GetFilePath(std::wstring filePath);

  static CFileManager &GetInstance();

 private:
  std::wstring _currentWorkingPath = L"script/";

  static std::once_flag s_onceFlag;
  static std::unique_ptr<CFileManager> s_sharedFileManager;
};
}  // namespace three_kingdoms
