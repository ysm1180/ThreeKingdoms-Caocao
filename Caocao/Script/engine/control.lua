--- 해당 파일에 있는 함수와 변수는 실제로 사용하지 않음
--- 타입을 정의하기 위해서 사용
---@class EngineBaseControl 엔진 내부의 컨트롤을 표현, 실제로 해당 변수가 사용되지는 않고 타입을 정의하기 위해서 만듬
_EngineBaseControl = {}

---@alias DefaultEvent fun(self: self):void
---@alias MouseEvent fun(self: self, key: number, x: number, y: number):void

---컨트롤이 활성화되어 있는지의 여부를 반환한다.
---@return boolean
function _EngineBaseControl:IsEnabled()
end

---컨트롤 표시 여부를 반환한다.
---@return boolean
function _EngineBaseControl:IsVisible()
end

---컨트롤의 너비를 반환한다.
---@return integer
function _EngineBaseControl:GetWidth()
end

---컨트롤의 높이를 반환한다.
---@return integer
function _EngineBaseControl:GetHeight()
end

---컨트롤의 X 위치를 반환한다.
---@return integer
function _EngineBaseControl:GetX()
end

---컨트롤의 Y 위치를 반환한다.
---@return integer 
function _EngineBaseControl:GetY()
end

---컨트롤의 임의 데이터를 반환한다.
---@return string
function _EngineBaseControl:GetUserData()
end

---컨트롤의 활성화 여부를 설정한다.
---@param isEnabled boolean
function _EngineBaseControl:SetEnabled(isEnabled)
end

---컨트롤의 너비를 설정한다.
---@param width integer
function _EngineBaseControl:SetWidth(width)
end

---컨트롤의 높이를 설정한다.
---@param height integer 
function _EngineBaseControl:SetHeight(height)
end

---컨트롤의 X 위치를 설정한다.
---@param x integer
function _EngineBaseControl:SetX(x)
end

---컨트롤의 Y 위치를 설정한다.
---@param y integer 
function _EngineBaseControl:SetY(y)
end

---컨트롤의 생성 이벤트를 설정한다.
---@param event DefaultEvent
function _EngineBaseControl:SetCreateEvent(event)
end

---컨트롤의 파괴 이벤트를 설정한다.
---@param event DefaultEvent
function _EngineBaseControl:SetDestroyEvent(event)
end

---컨트롤에서 왼쪽 마우스 버튼을 누르고 뗐을 때의 이벤트를 설정한다.
---@param event MouseEvent 
function _EngineBaseControl:SetMouseLButtonUpEvent(event)
end

---컨트롤에서 왼쪽 마우스 버튼을 눌렀을 때의 이벤트를 설정한다.
---@param event MouseEvent
function _EngineBaseControl:SetMouseLButtonDownEvent(event)
end

---컨트롤에서 마우스를 움직일 때 이벤트를 설정한다.
---@param event MouseEvent 
function _EngineBaseControl:SetMouseMoveEvent(event)
end

---컨트롤의 생성 이벤트를 설정한다.
---@param event DefaultEvent
function _EngineBaseControl:SetMouseEnterEvent(event)
end

---컨트롤의 생성 이벤트를 설정한다.
---@param event DefaultEvent 
function _EngineBaseControl:SetMouseLeaveEvent(event)
end

---컨트롤의 유저 데이터를 설정한다.
---@param data string 
function _EngineBaseControl:SetUserData(data)
end

---컨트롤을 새로고침합니다.
function _EngineBaseControl:Refresh()
end

---컨트롤의 일부 영역을 새로고침합니다.
---@param left number @좌상단 X 위치
---@param top number @좌상단 Y 위치
---@param right number @우하단 X 위치
---@param bottom number @우하단 Y 위치
function _EngineBaseControl:RefreshRegion(left, top, right, bottom)
end

---컨트롤을 보여줍니다.
function _EngineBaseControl:Show()
end

---컨트롤을 숨깁니다.
function _EngineBaseControl:Hide()
end
