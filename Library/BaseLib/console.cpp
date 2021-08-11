#include "console.h"

namespace three_kingdoms {
std::vector<Console *> Console::_consoles;

Console::Console() {}

Console::~Console() {}

void Console::RegisterConsole(Console *console) {
  _consoles.push_back(console);
}

void Console::OutputConsoles(std::wstring msg) {
  for (auto console : _consoles) {
    console->Output(msg);
  }
}
}  // namespace three_kingdoms