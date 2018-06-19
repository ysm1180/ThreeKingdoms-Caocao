require "../base/object.lua"

local currentPath = fileManager:GetWorkingPath()

-- @class ControlManager
-- @description 공통 Control 을 관리하는 Manager class, 직접적으로 사용할 일은 없는 클래스
-- @inherit Object
ControlManager = Object:Instance 
{
    

    ParseFromXML = function(self, file)
        local SLAXML = require "../base/slaxml.lua"
        local xml = io.open(currentPath .. file):read("*all")


        parser =
            SLAXML:parser {
            startElement = function(name, nsURI, nsPrefix)
                if name == "dialog" then

                elseif name == "label" then

                end
            end, -- When "<foo" or <x:foo is seen
            attribute = function(name, value, nsURI, nsPrefix)
                OUTPUT(name .. ":" .. value)
            end, -- attribute found on current element
            closeElement = function(name, nsURI)
                OUTPUT("close " .. name)
            end, -- When "</foo>" or </x:foo> or "/>" is seen
            text = function(text)
            end, -- text and CDATA nodes
            comment = function(content)
            end, -- comments
            pi = function(target, content)
            end -- processing instructions e.g. "<?yes mon?>"
        }

        parser:parse(xml)
    end
}
