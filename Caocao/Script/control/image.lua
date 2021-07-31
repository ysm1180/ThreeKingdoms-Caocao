require "control.lua"

---@class ImageLoadOptions
---@field FilePath string : 불러올 me5 파일 경로
---@field MaskColor integer | RGB @투명화할 색
---@field Brightness number @밝기
---@field Image Me5Item @불러올 이미지
---@field Mirror boolean

---@class Image : ControlObject @게임 내의 이미지를 Wrapping 한 클래스
Image = Class(ControlObject)

Image.DEFAULT_MASK_COLOR = {
    R = 0xF7,
    G = 0x00,
    B = 0xFF
}
Image.ClipingSize = {
    Width = 0,
    Height = 0
}

---이미지를 새로 생성합니다.
---@return Image
function Image:New()
    local newControl = {}

    -- controlManager 는 Lua의 ControlManager 랑 다르다!!
    -- 기본 게임 엔진에 내장되어있는 Control 생성 담당 인스턴스!
    newControl.control = controlManager:CreateImage()
    newControl.ClipingSize = {
        Width = 0,
        Height = 0
    }
    setmetatable(newControl, self)
    self.__index = self

    return newControl
end

---이미지를 Me5 파일에서 불러옵니다.
---@param options ImageLoadOptions 
function Image:LoadFromMe5(options)
    if options.FilePath ~= nil and options.Image ~= nil then
        if type(options.MaskColor) == "number" then
            options.MaskColor = {
                R = options.MaskColor % 0x100,
                G = math.floor(options.MaskColor / 0x100) % 0x100,
                B = math.floor(options.MaskColor / 0x10000) % 0x100
            }
        end
        if options.MaskColor == nil then
            options.MaskColor = {
                R = nil,
                G = nil,
                B = nil
            }
        end

        if options.Brightness == nil then
            options.Brightness = 1.0
        end

        if options.Mirror == nil then
            options.Mirror = false
        end

        options.MaskColor.R = options.MaskColor.R or self.DEFAULT_MASK_COLOR.R
        options.MaskColor.G = options.MaskColor.G or self.DEFAULT_MASK_COLOR.G
        options.MaskColor.B = options.MaskColor.B or self.DEFAULT_MASK_COLOR.B

        local file = gameManager:OpenFile(options.FilePath)
        if type(options.Image.Group) == "string" then
            local groupIndex = file:FindGroupIndexByName(options.Image.Group)
            if groupIndex == -1 then
                return
            end
            options.Image.Group = groupIndex
        end

        if type(options.Image.Item) == "string" then
            local itemIndex = file:FindItemIndexByName(options.Image.Group, options.Image.Item)
            if itemIndex == -1 then
                return
            end
            options.Image.Item = itemIndex - file:GetGroupStartItemIndex(options.Image.Group)
        end
        gameManager:CloseFile(file)

        self.control:LoadImageFromMe5FileByIndex(options.FilePath, options.Image.Group, options.Image.Item,
            gameManager:Color(options.MaskColor.R, options.MaskColor.G, options.MaskColor.B), options.Brightness,
            options.Mirror)
    end
end

function Image:Width()
    return self.control:GetWidth()
end

function Image:Height()
    return self.control:GetHeight()
end

---실제 그려질 영역을 지정한다.
---그려질 영역을 지정할 뿐, 실제로 이미지가 자동으로 Refresh 되지 않기 때문에 반영하기 위해선 반드시 Refresh 를 명시적으로 해주어야한다.
---@param rect Rect 그려질 영역 (사각형)
function Image:SetClipingRect(rect)
    if rect ~= nil then
        self.control:SetClipingRect(rect.Left, rect.Top, rect.Right, rect.Bottom)
    end
end

function Image:SetClipingSize(width, height)
    if width ~= nil and height ~= nil then
        self.ClipingSize.Width = width
        self.ClipingSize.Height = height
    end
end

function Image:SetClipingCoordinate(x, y)
    if x ~= nil and y ~= nil then
        self:SetClipingRect({
            Left = x * self.ClipingSize.Width,
            Top = y * self.ClipingSize.Height,
            Right = (x + 1) * self.ClipingSize.Width,
            Bottom = (y + 1) * self.ClipingSize.Height
        })
    end
end

---그려질 영역을 전체 영역으로 초기화한다.
function Image:ResetClipingRect()
    self.control:ResetClipingRect()
end

function Image:Mirror(value)
    if value ~= nil then
        self.control:SetDisplayMirror(value)
    end
end
