#pragma once

#define MAX_BUFFER_SIZE 8192

#include <string>

namespace jojogame
{
class CFile
{
public:
    CFile();
    ~CFile();

    bool Open(std::wstring filePath, std::wstring fileMode = L"rb+");
    void Close();

    const char *GetData();
    int GetSize();
    void Write(char *in, int position, int length);

private:
    FILE *_file = nullptr;
    int _size = 0;

    char *_data = nullptr;
};
} // namespace jojogame
