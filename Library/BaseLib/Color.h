#pragma once

#include <math.h>

#define min_f(a, b, c)  (fminf(a, fminf(b, c)))
#define max_f(a, b, c)  (fmaxf(a, fmaxf(b, c)))

namespace jojogame {

void RgbToHsv(const unsigned char r, const unsigned char g, const unsigned char b, double &h, double &s, double &v);
void HsvToRgb(const double h, const double s, const double v, unsigned char &r, unsigned char &g, unsigned char &b);

}