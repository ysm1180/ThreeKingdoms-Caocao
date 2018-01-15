#pragma once

namespace jojogame {
class __declspec(dllexport) Ref
{
public:
    Ref();
    virtual ~Ref();

    void release();

protected:
    unsigned int _referenceCount;
};
}
