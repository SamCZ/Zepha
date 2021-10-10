local api = hot_wheel
local r = 6.28319 / 6

local currentRotation = 0
local desiredRotation = 0

local rotating = false
local mod = false

local slot = 1
local item = 1

local function position()
    rotating = true
    zepha.after(function()
        currentRotation = currentRotation + (desiredRotation - currentRotation) / 2

        for i, list in ipairs(api.lists) do
            list.pos = {
                32.5 + 22 * math.sin(currentRotation - (r * (i + 3))) .. 'dp',
                32.5 + 22 * math.cos(currentRotation - (r * (i + 3))) .. 'dp'
            }
        end
        for i, label in ipairs(api.labels) do
            label.pos = {
                37 + 35 * math.sin(currentRotation - (r * (i + 3))) .. 'dp',
                36 + 35 * math.cos(currentRotation - (r * (i + 3))) .. 'dp'
            }
        end

        if currentRotation == desiredRotation then
            rotating = false
            return false
        elseif math.abs(currentRotation - desiredRotation) < 0.033 then
            currentRotation = desiredRotation
            return true
        else return true end
    end, 1/60)
end

local function select_slot(n)
    slot = n
    local offset = n - 1
    desiredRotation = offset * r

--     api.list_expanded.list = 'hot_wheel_' .. n

--     for i, list in ipairs(api.lists) do list.visible = i ~= n end
--     for i, label in ipairs(api.labels) do label.visible = i ~= n end

    zepha.player:set_wield_list('hot_wheel_' .. slot);
    if not rotating then position() end
end

local function select_item(n)
    item = ((item + n) - 1) % 5 + 1
    local offset = slot - 1
    api.list_expanded_indicator.pos = { 2 + 18 * (item - 1) .. 'dp', 0 }

    zepha.player:set_wield_index(item)
end

select_slot(1)
select_item(0)

local function key(n)
    select_slot(n)
end

local function scroll(n)
    if mod then select_slot(((slot + n - 1) % 6) + 1)
    else select_item(n) end
end

for i = 1, 6 do
    zepha.register_keybind("@auri:hot_wheel:select_slot_" .. i, {
        description = "Select Slot " .. i,
        default = zepha.keys[tostring(i)],
        on_press = function() key(i) end
    })
end

zepha.register_keybind("@auri:hot_wheel:item_down", {
    description = "Cycle Item Down",
    default = zepha.keys.scrolldown,
    on_press = function() scroll(1) end
})

zepha.register_keybind("@auri:hot_wheel:item_up", {
    description = "Cycle Item Up",
    default = zepha.keys.scrollup,
    on_press = function() scroll(-1) end
})

zepha.register_keybind("@auri:hot_wheel:mod", {
    description = "Modifier Key",
    default = zepha.keys.leftshift,
    on_press = function() mod = true end,
    on_release = function() mod = false end
})