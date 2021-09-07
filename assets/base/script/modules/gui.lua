zepha.__builtin.gui_env = { Gui = {}}
zepha.Gui = zepha.__builtin.gui_env.Gui
local env = zepha.__builtin.gui_env
setmetatable(env, {__index = _G})

-- create_element
-- Build a GUI Element with the provided constructor data, apply the metatable.
local function create_element(elem_type, data)
    return GuiElement.new(elem_type, data)
end

-- register_element
-- Add an element to the Gui namespace.
local function register_element(key)
    if type(key) == 'table' then
        for _, v in pairs(key) do register_element(v) end
        return
    end
    env.Gui[key] = function(data) return create_element(key, data) end
end

register_element({ 'Box', 'Text' })

-- zepha.build_gui
-- Allows you to Build UI Elements with the GUI namespace outside of a callback.
zepha.gui = function(fn)
    setfenv(fn, env)
    return fn()
end