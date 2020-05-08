local root = zepha.player:get_hud():get("hot_wheel_root")
local lists = {
    root:get("list_1"),
    root:get("list_2"),
    root:get("list_3"),
    root:get("list_4"),
    root:get("list_5"),
    root:get("list_6")
}

local offset = 0
local function select(n)
    offset = n - 1

    for i,list in ipairs(lists) do
        list.list = "hot_wheel_" .. tostring((i + offset - 1) % 6 + 1)
    end

    root:get("numbers").background = "@aurailus:hot_wheel:circle_numbers_" .. (offset + 1)
    zepha.player:set_selected_block(zepha.player:get_inventory():get_list("hot_wheel_" .. ((offset + 2) % 6 + 1)):get_stack(1).name)
end

for i = 1, 6 do
    zepha.register_keybind("@aurailus:hot_wheel:select_slot_" .. i, {
        description = "Select Slot " .. i,
        default = zepha.keys[tostring(i)],
        on_press = function() select(i) end
    })
end