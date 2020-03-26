local env = {}
setmetatable(env, {__index = _G})

env.Gui = {}

local function create_gui_table(elem_type, data)
    local element = {}
    element.type = elem_type
    element.children = {}
    element.callbacks = {}
    element.traits = {}
    element.key = ""

    for k, v in pairs(data) do
        if type(k) == "number" then
            table.insert(element.children, v)
        elseif k == "key" then
            element.key = v
        elseif k == "callbacks" then
            element.callbacks = v
        else
            element.traits[k] = v
        end
    end

    return element
end

env.Gui.Body          = function(data) return create_gui_table("body", data) end
env.Gui.Rect          = function(data) return create_gui_table("rect", data) end
env.Gui.Text          = function(data) return create_gui_table("text", data) end
env.Gui.Model         = function(data) return create_gui_table("model", data) end
env.Gui.Button        = function(data) return create_gui_table("button", data) end
env.Gui.InventoryList = function(data) return create_gui_table("inventory_list", data) end

env.pc = function(num)
    return tostring(num) .. "%"
end

zepha.create_menu = function(fn)
    setfenv(fn, env)
    return fn()
end