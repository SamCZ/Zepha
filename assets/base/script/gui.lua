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

    __call = function(tbl, fn)
        setfenv(fn, env)
        return fn(tbl)
    end,

    find = function(tbl, key)
        for _,v in pairs(rawget(tbl, "children")) do
            if v.key == key then return v end
            local c = v:find(key)
            if c ~= nil then return c end
        end
    end,

    append = function(tbl, elem)
        if type(elem) == "function" then elem = zepha.build_gui(elem) end
        rawset(elem, "parent", tbl)
        table.insert(rawget(tbl, "children"), elem)
    end,

    prepend = function(tbl, elem)
        if type(elem) == "function" then elem = zepha.build_gui(elem) end
        rawset(elem, "parent", tbl)
        table.insert(rawget(tbl, "children"), 1, elem)
    end,

    remove = function(tbl)
        if tbl.parent == nil then return end
        local ind = 0
        for k,v in pairs(tbl.parent.children) do
            if v == tbl then ind = k end
        end
        if ind == 0 then return end
        table.remove(tbl.parent.children, ind)
        return tbl
    end
}

-- create_element
-- Build a GUI Element with the provided constructor data, apply the metatable.
local function create_element(elem_type, data)
    local element = {
        type = elem_type,
        key = "",

        parent = nil,
        children = {},

        callbacks = {},
        traits = {}
    }

    setmetatable(element, elem_mt)

    for k, v in pairs(data) do
        if type(k) == "number" then
            rawset(v, "parent", element)
            table.insert(element.children, v)
        else
            element[k] = v
        end
    end

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