require "../control/image_manager.lua"

---@class Map
Map = {
    MapFilePath = "../map.me5",
    MapGroupName = {
        BATTLE = "Battle"
    },
    TileSize = {
        Width = 48,
        Height = 48
    }
}

---map me5 파일에서 이미지를 불러와 맵을 생성한다.
---@field index integer
---@field layout Layout
---@return Map
function Map:New(index, layout)
    local newMap = {}

    newMap.BattleMapImage = ImageManager:CreateImage({
        FilePath = self.MapFilePath,
        Group = {
            Main = self.MapGroupName.BATTLE,
            Sub = index
        }
    })

    newMap.Layout = layout
    newMap.BattleMapImageIndex = layout:AddImage(newMap.BattleMapImage, 0, 0)

    setmetatable(newMap, self)
    self.__index = self

    return newMap
end

function Map:Width()
    return self.BattleMapImage:Width()
end

function Map:Height()
    return self.BattleMapImage:Height()
end

function Map:MapWidth()
    return math.floor(self.BattleMapImage:Width() / self.TileSize.Width)
end

function Map:MapHeight()
    return math.floor(self.BattleMapImage:Height() / self.TileSize.Height)
end
