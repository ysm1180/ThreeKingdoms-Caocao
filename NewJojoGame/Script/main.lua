require "window_manager.lua"

local title = "三國志曺操傳"

main = WindowManager.Create({
    Width = 640,
    Height = 440,
    X = 100,
    Y = 100,
    TitleName = title,
    BackColor = {R = 0, G = 0, B = 0},
    CloseEvent = "main_close"
})
main:Show()


function main_close() 
    gameManager:Quit()
end
