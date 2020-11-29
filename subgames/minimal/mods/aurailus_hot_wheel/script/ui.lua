local api = aurailus_hot_wheel
local hud = zepha.player:get_hud()

hud:append(function()
    local root = Gui.Rect {
        key = "hot_wheel_root",

        size = { 140, 80 },
        position = { "0%", "100%" },
        position_anchor = { "-10%", "110%" },

        Gui.Rect {
            size = { 94, 18 },
            position = { 52, 19 },
            background = "@aurailus:hot_wheel:hot_wheel_line",

            Gui.Rect {
                key = "expanded_indicator",
                size = { 18, 18 },
                position = { 2, 0 },
                background = "@aurailus:hot_wheel:hot_wheel_selection"
            },

            Gui.InventoryList {
                key = "list_expanded",
                position = { 3, 1 },
                slot_spacing = { 2, 2 },
                length = 6,
                source = "current_player",
                list = "hot_wheel_1",
            }
        },

        Gui.Rect {
            size = { 80, 80 },
            background = "@aurailus:hot_wheel:hot_wheel_circle",

            Gui.InventoryList {
                key = "list_1",
                slot_spacing = { 2, 2 },
                length = 1,
                source = "current_player",
                list = "hot_wheel_1",
            },
            Gui.InventoryList {
                key = "list_2",
                slot_spacing = { 2, 2 },
                length = 1,
                source = "current_player",
                list = "hot_wheel_2",
            },
            Gui.InventoryList {
                key = "list_3",
                slot_spacing = { 2, 2 },
                length = 1,
                source = "current_player",
                list = "hot_wheel_3",
            },
            Gui.InventoryList {
                key = "list_4",
                slot_spacing = { 2, 2 },
                length = 1,
                source = "current_player",
                list = "hot_wheel_4",
            },
            Gui.InventoryList {
                key = "list_5",
                slot_spacing = { 2, 2 },
                length = 1,
                source = "current_player",
                list = "hot_wheel_5",
            },
            Gui.InventoryList {
                key = "list_6",
                slot_spacing = { 2, 2 },
                length = 1,
                source = "current_player",
                list = "hot_wheel_6",
            },

            Gui.Text {
                key = "label_1",
                content = "1"
            },
            Gui.Text {
                key = "label_2",
                content = "2"
            },
            Gui.Text {
                key = "label_3",
                content = "3"
            },
            Gui.Text {
                key = "label_4",
                content = "4"
            },
            Gui.Text {
                key = "label_5",
                content = "5"
            },
            Gui.Text {
                key = "label_6",
                content = "6"
            }
        }
    }

    api.labels = {
        root:get("label_1"),
        root:get("label_2"),
        root:get("label_3"),
        root:get("label_4"),
        root:get("label_5"),
        root:get("label_6")
    }

    api.lists = {
        root:get("list_1"),
        root:get("list_2"),
        root:get("list_3"),
        root:get("list_4"),
        root:get("list_5"),
        root:get("list_6")
    }

    api.list_expanded = root:get("list_expanded")
    api.list_expanded_indicator = root:get("expanded_indicator")

    return root
end)