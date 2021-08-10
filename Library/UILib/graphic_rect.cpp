#include "graphic_rect.h"

namespace three_kingdoms {
void CGraphicRect::RegisterFunctions(lua_State *L) {
  LUA_BEGIN(CGraphicRect, "_GraphicRect");

  LUA_METHOD(GetColor);
  LUA_METHOD(GetOpacity);
  LUA_METHOD(GetWidth);
  LUA_METHOD(GetHeight);

  LUA_METHOD(SetColor);
  LUA_METHOD(SetOpacity);
  LUA_METHOD(SetWidth);
  LUA_METHOD(SetHeight);
}

CGraphicRect::CGraphicRect() {}

CGraphicRect::~CGraphicRect() {}

COLORREF CGraphicRect::GetColor() { return _color; }

int CGraphicRect::GetWidth() { return _size.cx; }

int CGraphicRect::GetHeight() { return _size.cy; }

double CGraphicRect::GetOpacity() { return _opacity; }

void CGraphicRect::SetColor(COLORREF color) { _color = color; }

void CGraphicRect::SetWidth(int width) { _size.cx = width; }

void CGraphicRect::SetHeight(int height) { _size.cy = height; }

void CGraphicRect::SetOpacity(double opacity) { _opacity = opacity; }
}  // namespace three_kingdoms