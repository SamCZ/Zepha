local env = { Gui = {}}
local Gui = env.Gui
setmetatable(env, {__index = _G})

elem_mt = {
    __index = function(elem, key)
        -- Don't directly access the traits table.
        if key == "traits" then return nil end

        -- Attempt to get a value from the table, e.g. type, key, children, etc.
        local val = rawget(elem, key)
        -- Attempt to get a value from the metatable, e.g. a function.
        if val == nil then val = rawget(getmetatable(elem), key) end
        -- Attempt to get a value from the traits array.
        if val == nil then val = rawget(rawget(elem, "traits"), key) end

        return val
    end,

    __newindex = function(elem, key, val)
        if key == "type" or key == "key" or key == "callbacks" then rawset(elem, key, val) end
        rawset(rawget(elem, "traits"), key, val)
    end,

    get = function(tbl, key)
        if type(key) == number then return rawget(tbl, "children")[key] end

        for _,v in pairs(rawget(tbl, "children")) do
            if v == key then return v end
        end
    end,

    append = function(tbl, elem)
        if type(elem) == "function" then elem = zepha.build_ui(elem) end
        table.insert(rawget(tbl, "children"), elem)
    end,

    prepend = function(tbl, elem)
        if type(elem) == "function" then elem = zepha.build_ui(elem) end
        table.insert(rawget(tbl, "children"), 0, elem)
    end,
}

-- create_element
-- Build a GUI Element with the provided constructor data, apply the metatable.
local function create_element(elem_type, data)
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

    setmetatable(element, elem_mt)
    return element
end

-- register_element
-- Add an element to the Gui namespace.
local function register_element(key)
    if type(key) == "table" then
        for _,v in pairs(key) do register_element(v) end
        return
    end
    Gui[key] = function(data) return create_element(key, data) end
end

register_element({"Body", "Rect", "Text", "Model", "Button", "InventoryList"})

-- pc
-- Formats a number to be a percent string.
env.pc = function(num)
    return tostring(num) .. "%"
end

-- zepha.build_ui
-- Allows you to Build UI Elements with the GUI namespace outside of a callback.
zepha.build_gui = function(fn)
    setfenv(fn, env)
    return fn()
end