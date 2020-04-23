if zepha.server then return end

local hud = zepha.player:get_hud()
hud:append(function() return Gui.Rect {
    key = "hot_wheel_root",

    size = { 140, 80 },
    position = { "0%", "100%" },
    position_anchor = { "-10%", "110%" },
--     background = "#966"
} end)
local root = hud:get("hot_wheel_root")

root(function(e)
    e:append(Gui.Rect {
        size = { 80, 80 },
        background = "@aurailus:hot_wheel:hot_wheel_circle",

        Gui.Rect {
            key = "numbers",
            size = { 80, 80 },
            background = "@aurailus:hot_wheel:circle_numbers_1"
        },

        Gui.InventoryList {
            key = "list_1",
            position = { 12, 22 },
            slot_spacing = { 2, 2 },
            length = 1,
            source = "current_player",
            list = "hot_wheel_1",
        },
        Gui.InventoryList {
            key = "list_2",
            position = { 32, 11 },
            slot_spacing = { 2, 2 },
            length = 1,
            source = "current_player",
            list = "hot_wheel_2",
        },
--         Gui.InventoryList {
--             position = { 6, 16 },
--             slot_spacing = { 2, 2 },
--             length = 1,
--             source = "current_player",
--             list = "hot_wheel_3",
--         },
        Gui.InventoryList {
            key = "list_4",
            position = { 52, 43 },
            slot_spacing = { 2, 2 },
            length = 1,
            source = "current_player",
            list = "hot_wheel_4",
        },
        Gui.InventoryList {
            key = "list_5",
            position = { 32, 54 },
            slot_spacing = { 2, 2 },
            length = 1,
            source = "current_player",
            list = "hot_wheel_5",
        },
        Gui.InventoryList {
            key = "list_6",
            position = { 12, 43 },
            slot_spacing = { 2, 2 },
            length = 1,
            source = "current_player",
            list = "hot_wheel_6",
        },
    })

    e:append(Gui.Rect {
        size = { 100, 18 },
        position = { 52, 19 },
        background = "@aurailus:hot_wheel:hot_wheel_line",

        Gui.InventoryList {
            key = "list_3",
            position = { 3, 1 },
            slot_spacing = { 2, 2 },
            length = 6,
            source = "current_player",
            list = "hot_wheel_3",
        }
    })
end)

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