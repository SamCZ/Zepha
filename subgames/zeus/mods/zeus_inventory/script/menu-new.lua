local formspec = zepha.create_menu(function()
    return Gui.Body {
        background = "#0003",

        callbacks = {
            primary = zepha.player:close_menu
        },

        Gui.Rect {
            key = "inventory",
            position = { pc(50), pc(50) },
            position_anchor = { pc(50), pc(55) },
            size = { 218, 160 },

            Gui.Rect {
                key = "inventory_background",

                position = { 0, 45 },
                size = { 218, 100 },
                padding = { 20, 10, 8, 10 },
                background = "zeus:inventory:inventory",

                Gui.InventoryList {
                    position = { 1, 1 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "main",
                }
            },
            Gui.Rect {
                key = "crafting_background",

                position = { 0, -5 },
                size = { 218, 72 },
                padding = { 20, 10, 8, 10 },
                background = "zeus:inventory:crafting",

                Gui.InventoryList {
                    position = { 111, 1 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "craft",
                },
                Gui.InventoryList {
                    position = { 163, 10 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "craft_result",
                },
                Gui.Rect {
                    key = "player_clamp",

                    position = { 41, -8 },
                    size = { 32, 52 },
                    overflow = "hidden",

                    Gui.Model {
                        position = { 15, 52 },
                        scale = { 86, 86 },

                        type = "model",
                        source = "zeus:default:player",
                        texture = "zeus:default:player",
                        anim_range = { 0, 300 }
                    }
                }
            },
            Gui.Rect {
                position = { 2, 160 },
                size = { 214, 67 },
                background = "zeus:inventory:inventory_wheel",

                children = {
                Gui.InventoryList {
                    position = { 9, 1 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "hot_wheel_1",
                },
                Gui.InventoryList {
                    position = { 117, 1 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "hot_wheel_2",
                },
                Gui.InventoryList {
                    position = { 125, 25 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "hot_wheel_3",
                },
                Gui.InventoryList {
                    position = { 117, 50 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "hot_wheel_4",
                },
                Gui.InventoryList {
                    position = { 9, 50 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "hot_wheel_5",
                },
                Gui.InventoryList {
                    position = { 1, 25 },
                    slot_spacing = { 2, 2 },
                    source = "current_player",
                    list = "hot_wheel_6",
                }
            }
        }
    }
end)

-- Register the inventory menu and keybind
zepha.register_keybind("zeus:inventory:open_inventory", {
    description = "Open Inventory",
    default = zepha.keys.e,
    on_press = function()
        if (zepha.player.menu_state == "") then
            zepha.player:open_menu(formspec)
        else
            zepha.player:close_menu()
        end
    }
})