local menu = zepha.build_gui(function()
    return Gui.Body {
        background = "#0003",

        callbacks = {
            primary = function() zepha.player:close_menu() end,
        },

        Gui.Rect {
            key = "inventory",
            position = { pc(50), pc(50) },
            position_anchor = { pc(50), pc(50) },
            size = { 342, 187 },

            Gui.Rect {
                key = "backpack",

                position = { 1, 0 },
                size = { 106, 187 },
                padding = { 19, 9, 8, 9 },
                background = "zeus:inventory:backpack",

                Gui.InventoryList {
                    position = { 0, 0 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "main",
                },
                Gui.InventoryList {
                    position = { 0, 18*3 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "hot_wheel_1",
                },
                Gui.InventoryList {
                    position = { 0, 18*4 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "hot_wheel_2",
                },
                Gui.InventoryList {
                    position = { 0, 18*5 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "hot_wheel_3",
                },
                Gui.InventoryList {
                    position = { 0, 18*6 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "hot_wheel_4",
                },
                Gui.InventoryList {
                    position = { 0, 18*7 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "hot_wheel_5",
                },
                Gui.InventoryList {
                    position = { 0, 18*8 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "hot_wheel_6",
                }
            },
            Gui.Rect {
                key = "player_frame",
                position = { 105, 0 },
                size = { 106, 187 },
                background = "zeus:inventory:player_frame",

                Gui.Model {
                    position = { 52, 150 },
                    scale = { 64, 64 },

                    type = "model",
                    source = "zeus:default:player",
                    texture = "zeus:default:player",
                    anim_range = { 0, 100 }
                }
            },
            Gui.Rect {
                key = "equipment",

                position = { 209, 1 },
                size = { 132, 80 },
                padding = { 18, 8, 8, 8 },
                background = "zeus:inventory:equipment",

                Gui.Rect {
                    key = "player_clamp",

                    position = { 41, 1 },
                    size = { 34, 52 },
                    overflow = "hidden"
                }
            },
            Gui.Rect {
                key = "dynamic",

                position = { 209, 80 },
                size = { 132, 107 },
                padding = { 8, 8, 8, 8 },
                background = "zeus:inventory:dynamic",

                Gui.InventoryList {
                    position = { 1, 1 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "mod",
                },

                Gui.InventoryList {
                    position = { 41, 32 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "craft",
                },
                Gui.InventoryList {
                    position = { 81, 41 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "craft_result",
                }
            }
        }
    }
end)

inventory.open_inventory = function()
    if not zepha.player.in_menu then
        zepha.player:show_menu(menu)
    else
        zepha.player:close_menu()
    end
end

zepha.register_keybind("zeus:inventory:open_inventory", {
    description = "Open Inventory",
    default = zepha.keys['.'],
    on_press = inventory.open_inventory
})