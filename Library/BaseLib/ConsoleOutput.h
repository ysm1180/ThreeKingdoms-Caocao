#pragma once

#define OUTPUT_MAX_LENGTH 4096

#include <string>
#include <vector>

namespace jojogame {
class CConsoleOutput
{
public:
    CConsoleOutput();
    virtual ~CConsoleOutput();

    virtual void Output(std::string msg) = 0;

    static void RegisterConsole(CConsoleOutput *console);

    static void OutputConsoles(std::string msg);

private:
    static std::vector<CConsoleOutput *> _consoles;
};
}