#include "Color.h"

namespace jojogame {
void RgbToHsv(const unsigned char r, const unsigned char g, const unsigned char b, double& h, double& s, double& v)
{
    auto computedR = r / 255.0f;
    auto computedG = g / 255.0f;
    auto computedB = b / 255.0f;

    auto max = max_f(computedR, computedG, computedB);
    auto min = min_f(computedR, computedG, computedB);

    auto computedH = 0.0f;
    auto computedS = 0.0f;
    auto computedV = max;

    if (max == 0.0f)
    {
        computedS = 0;
        computedH = 0;
    }
    else if (max - min == 0.0f)
    {
        computedS = 0;
        computedH = 0;
    }
    else
    {
        computedS = (max - min) / max;

        if (max == computedR)
        {
            computedH = 60 * ((computedG - computedB) / (max - min)) + 0;
        }
        else if (max == computedG)
        {
            computedH = 60 * ((computedB - computedR) / (max - min)) + 120;
        }
        else
        {
            computedH = 60 * ((computedR - computedG) / (max - min)) + 240;
        }
    }

    if (computedH < 0) computedH += 360.0f;

    h = computedH; // dst_h : 0-360
    s = computedS; // dst_s : 0-1
    v = computedV; // dst_v : 0-1
}

void HsvToRgb(const double h, const double s, const double v, unsigned char& r, unsigned char& g, unsigned char& b)
{
    float computedH = h; // 0-360
    float computedS = s; // 0.0-1.0
    float computedV = v; // 0.0-1.0

    float resultR = 0;
    float resultG = 0;
    float resultB = 0;

    auto hi = static_cast<int>(computedH / 60.0f) % 6;
    auto f = (computedH / 60.0f) - hi;
    auto p = computedV * (1.0f - computedS);
    auto q = computedV * (1.0f - computedS * f);
    auto t = computedV * (1.0f - computedS * (1.0f - f));

    switch (hi)
    {
    case 0: resultR = computedV, resultG = t, resultB = p;
        break;
    case 1: resultR = q, resultG = computedV, resultB = p;
        break;
    case 2: resultR = p, resultG = computedV, resultB = t;
        break;
    case 3: resultR = p, resultG = q, resultB = computedV;
        break;
    case 4: resultR = t, resultG = p, resultB = computedV;
        break;
    case 5: resultR = computedV, resultG = p, resultB = q;
        break;
    }

    r = static_cast<unsigned char>(resultR * 255); // dst_r : 0-255
    g = static_cast<unsigned char>(resultG * 255); // dst_r : 0-255
    b = static_cast<unsigned char>(resultB * 255); // dst_r : 0-255
}
}
