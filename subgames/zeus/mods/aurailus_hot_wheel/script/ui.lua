local hud = zepha.player:get_hud()

hud:append(function() return Gui.Rect {
    key = "hot_wheel_root",

    size = { 140, 80 },
    position = { "0%", "100%" },
    position_anchor = { "-10%", "110%" },

    Gui.Rect {
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
        }
    },
    Gui.Rect {
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
    }
} end)
