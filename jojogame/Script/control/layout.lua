require "control.lua"

-- @class Layout : 게임 내의 레이아웃을 Wrapping 하는 클래스
-- @description 이미지를 관리하는 레이아웃 컨트롤입니다.
-- @inherit ControlObject
Layout = ControlObject:Instance
{
    -- @description 레이아웃을 새로 생성합니다.
    -- @return Layout : 새로 생성된 Layout class
    New = function(self)
        local newControl = {}

        -- @description controlManager 는 Lua의 ControlManager 랑 다르다!!
        -- @description 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
        newControl.control = controlManager:CreateLayout()
        setmetatable(newControl, self)
        self.__index = self
        
        return newControl
    end,
    
    -- @description 레이아웃을 움직입니다.
    -- @param x int : x 위치
    -- @param y int : y 위치
    -- @param isRedraw boolean : 움직일 때 다시 그리기 여부
    Move = function(self, x, y, isRedraw)
        if isRedraw == nil then
            isRedraw = true
        end

        if x ~= nil then
            self.control:SetX(x, isRedraw)
        end
        if y ~= nil then
            self.control:SetY(y)
        end

    end,

    -- @description 레이아웃 너비를 설정합니다.
    -- @param width int : 너비
    -- @param isRedraw boolean : 다시 그리기 여부
    SetWidth = function(self, width, isRedraw)
        if width ~= nil then
            if isRedraw == nil then
                isRedraw = true
            end

            self.control:SetWidth(width)
        end
    end,

    -- @description 레이아웃 높이를 설정합니다.
    -- @param height int : 높이
    -- @param isRedraw boolean : 다시 그리기 여부
    SetHeight = function(self, height, isRedraw)
        if height ~= nil then
            if isRedraw == nil then
                isRedraw = true
            end

            self.control:SetHeight(height)
        end
    end,

    -- @description 레이아웃 가로 비율을 설정합니다.
    -- @param ratio double : 비율
    -- @param isRedraw boolean : 다시 그리기 여부
    SetRatioX = function(self, ratio, isRedraw)
        if ratio ~= nil then
            if isRedraw == nil then
                isRedraw = true
            end

            self.control:SetRatioX(ratio)
        end
    end,

    -- @description 레이아웃 세로 비율을 설정합니다.
    -- @param ratio double : 비율
    -- @param isRedraw boolean : 다시 그리기 여부
    SetRatioY = function(self, ratio, isRedraw)
        if ratio ~= nil then
            if isRedraw == nil then
                isRedraw = true
            end

            self.control:SetRatioY(ratio)
        end
    end,

    -- @description 레이아웃에 이미지를 추가합니다.
    -- @param image Image : 추가할 이미지
    -- @param x int : 추가할 이미지의 X 좌표
    -- @param y int : 추가할 이미지의 Y 좌표
    -- @param isUpdate boolean : 추가한 이미지 즉시 업데이트 여부
    AddImage = function(self, image, x, y, isUpdate)
        if image ~= nil and x ~= nil and y ~= nil then
            if isUpdate == nil then
                isUpdate = true
            end
            return self.control:AddImage(image.control, x, y, isUpdate)
        end
        
        return -1
    end,
    
    -- @description 레이아웃에 이미지를 삭제합니다.
    -- @param image Image : 삭제할 이미지
    -- @param isUpdate boolean : 삭제한 이미지 즉시 업데이트 여부
    DeleteImage = function(self, image, isUpdate)
        if image ~= nil then
            if isUpdate == nil then
                isUpdate = true
            end
            self.control:DeleteImage(image.control, isUpdate)

        end
    end,
}