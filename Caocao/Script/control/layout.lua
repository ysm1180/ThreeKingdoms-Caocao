require "control.lua"

---@class Layout : ControlObject @게임 내의 레이아웃을 Wrapping 하는 클래스
Layout = Class(ControlObject)

---레이아웃을 새로 생성합니다.
---@return Layout 
function Layout:New()
    local newControl = {}

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateLayout()
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---레이아웃을 움직입니다.
---@param x number
---@param y number 
---@param isRedraw boolean @움직일 때 다시 그리기 여부
function Layout:Move(x, y, isRedraw)
    if isRedraw == nil then
        isRedraw = true
    end

    if x ~= nil then
        self.control:SetX(x, isRedraw)
    end
    if y ~= nil then
        self.control:SetY(y)
    end

end

---레이아웃 너비를 설정합니다.
---@param width number
---@param isRedraw boolean @다시 그리기 여부
function Layout:SetWidth(width, isRedraw)
    if width ~= nil then
        if isRedraw == nil then
            isRedraw = true
        end

        self.control:SetWidth(width)
    end
end

---레이아웃 높이를 설정합니다.
---@param height number 
---@param isRedraw boolean @다시 그리기 여부
function Layout:SetHeight(height, isRedraw)
    if height ~= nil then
        if isRedraw == nil then
            isRedraw = true
        end

        self.control:SetHeight(height)
    end
end

---레이아웃 가로 비율을 설정합니다.
---@param ratio number 
---@param isRedraw boolean @다시 그리기 여부
function Layout:SetRatioX(ratio, isRedraw)
    if ratio ~= nil then
        if isRedraw == nil then
            isRedraw = true
        end

        self.control:SetRatioX(ratio, isRedraw)
    end
end

---레이아웃 세로 비율을 설정합니다.
---@param ratio number
---@param isRedraw boolean @다시 그리기 여부
function Layout:SetRatioY(ratio, isRedraw)
    if ratio ~= nil then
        if isRedraw == nil then
            isRedraw = true
        end

        self.control:SetRatioY(ratio, isRedraw)
    end
end

---레이아웃의 X 좌표를 가져온다.
---@return integer
function Layout:X()
    return self.control:GetX()
end

---레이아웃의 Y 좌표를 가져온다.
---@return integer
function Layout:Y()
    return self.control:GetY()
end

---레이아웃의 좌표를 가져온다.
---@return integer, integer @X, Y
function Layout:Position()
    return self.control:GetX(), self.control:GetY()
end

---레이아웃의 너비를 가져온다.
---@return integer
function Layout:Width()
    return self.control:GetWidth()
end

---레이아웃의 높이를 가져온다.
---@return integer
function Layout:Height()
    return self.control:GetHeight()
end

---레이아웃의 크기를 가져온다.
---@return integer, integer @width, height
function Layout:Size()
    return self.control:GetWidth(), self.control:GetHeight()
end

---레이아웃에 이미지를 추가합니다.
---@param image Image @추가할 이미지
---@param x integer @추가할 이미지의 X 좌표
---@param y integer @추가할 이미지의 Y 좌표
---@param isShow boolean @추가한 이미지가 바로 보일지 여부
---@return integer @추가된 이미지의 해당 레이아웃에 대한 고유 인덱스, -1 이면 ERROR
function Layout:AddImage(image, x, y, isShow)
    if image ~= nil and x ~= nil and y ~= nil then
        if isShow == nil then
            isShow = true
        end
        return self.control:AddImage(image.control, x, y, isShow)
    end

    return -1
end

---레이아웃에 이미지를 삭제합니다.
---@param index integer @삭제할 이미지의 고유 인덱스
---@param isUpdate boolean @삭제한 이미지 즉시 업데이트 여부
function Layout:DeleteImage(index, isUpdate)
    if index ~= nil then
        if isUpdate == nil then
            isUpdate = true
        end
        self.control:DeleteImage(index, isUpdate)

    end
end

---레이아웃에 있는 이미지의 좌표를 이동합니다.
---@param index integer @이동할 이미지의 고유 인덱스
---@param x integer @이동할 이미지의 X 좌표
---@param y integer @이동할 이미지의 Y 좌표
---@param isUpdate boolean @이미지 즉시 업데이트 여부
function Layout:MoveImage(index, x, y, isUpdate)
    if index ~= nil and x ~= nil and y ~= nil then
        if isUpdate == nil then
            isUpdate = true
        end
        return self.control:MoveImage(index, x, y, isUpdate)
    end

    return -1
end

---이미지를 숨깁니다.
---@param index integer @숨길 이미지의 고유 인덱스
---@param isUpdate boolean @이미지 즉시 업데이트 여부
function Layout:HideImage(index, isUpdate)
    if index ~= nil then
        if isUpdate == nil then
            isUpdate = true
        end
        self.control:HideImage(index, isUpdate)
    end
end

---이미지를 보여줍니다.
---@param index integer @보여줄 이미지의 고유 인덱스
---@param isUpdate boolean @이미지 즉시 업데이트 여부
function Layout:ShowImage(index, isUpdate)
    if index ~= nil then
        if isUpdate == nil then
            isUpdate = true
        end
        self.control:ShowImage(index, isUpdate)
    end
end

function Layout:Refresh()
    self.control:Refresh()
end
