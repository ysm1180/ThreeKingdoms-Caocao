#pragma once

#include <string>
#include <vector>

namespace three_kingdoms {
class Console {
 public:
  Console();
  virtual ~Console();

  virtual void Output(std::wstring msg) = 0;

  static void RegisterConsole(Console *console);

  static void OutputConsoles(std::wstring msg);

 private:
  static std::vector<Console *> _consoles;
};
}  // namespace three_kingdoms