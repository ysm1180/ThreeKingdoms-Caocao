﻿#include "File.h"

namespace three_kingdoms
{
CFile::CFile()
{
}

CFile::~CFile()
{
    Close();
}

bool CFile::Open(std::wstring filePath, std::wstring fileMode)
{
    auto result = _wfopen_s(&_file, filePath.c_str(), fileMode.c_str());

    if (result == ENOENT)
    {
        result = _wfopen_s(&_file, filePath.c_str(), L"wb+");
    }

    if (result != 0)
    {
        return false;
    }

    fseek(_file, 0, SEEK_END);
    _size = ftell(_file);
    fseek(_file, 0, SEEK_SET);
    ;

    _data = static_cast<char *>(malloc(_size));
    fread(_data, 1, _size, _file);

    return true;
}

void CFile::Close()
{
    if (_file != nullptr)
    {
        fclose(_file);
        _file = nullptr;
    }

    if (_data != nullptr)
    {
        free(_data);
        _data = nullptr;
    }
}

const char *CFile::GetData()
{
    return _data;
}

int CFile::GetSize()
{
    return _size;
}

void CFile::Write(char *in, int position, int length)
{
    fseek(_file, position, SEEK_SET);
    fwrite(in, 1, length, _file);
    if (length + position > _size)
    {
        _data = static_cast<char *>(realloc(_data, length + position));
    }
    memcpy(_data + position, in, length);
}
} // namespace three_kingdoms
