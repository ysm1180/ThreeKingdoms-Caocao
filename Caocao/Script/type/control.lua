MOUSE_CODE_CONTROL = 0x8
MOUSE_CODE_LBUTTON = 0x1
MOUSE_CODE_MBUTTON = 0x10
MOUSE_CODE_RBUTTON = 0x2
MOUSE_CODE_SHIFT = 0x4

---@class ControlEvent @control의 이벤트 테이블
---@field Create fun(self: self):void @생성 이벤트
---@field Destroy fun(self: self):void @파괴 이벤트
---@field MouseLButtonUp fun(self: self, code: number, x: number, y: number):void @마우스 왼쪽 버튼 눌렀다 땠을 때 이벤트
---@field MouseLButtonDown fun(self: self, code: number, x: number, y: number):void @마우스 왼쪽 버튼 눌렀을 때 이벤트
---@field MouseMove fun(self: self, code: number, x: number, y: number):void @마우스 움직일 때 이벤트
---@field MouseEnter fun(self: self) @마우스 진입 이벤트
---@field MouseLeave fun(self: self) @마우스 벗어났을 때 이벤트
ControlEvent = {}
