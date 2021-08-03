#include "ConsoleOutput.h"

namespace three_kingdoms
{
std::vector<CConsoleOutput *> CConsoleOutput::_consoles;

CConsoleOutput::CConsoleOutput()
{
}

CConsoleOutput::~CConsoleOutput()
{
}

void CConsoleOutput::RegisterConsole(CConsoleOutput *console)
{
    _consoles.push_back(console);
}

void CConsoleOutput::OutputConsoles(std::wstring msg)
{
    for (auto console : _consoles)
    {
        console->Output(msg);
    }
}
} // namespace three_kingdoms