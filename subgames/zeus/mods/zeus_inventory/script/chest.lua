local chest = zepha.gui(function()
    return Gui.Box {
        background = "#0003",

--         Gui.Rect {
--             key = "inventory",
--             position = { pc(50), pc(50) },
--             position_anchor = { pc(50), pc(32) },
--             size = { 218, 160 },
--
--             Gui.Rect {
--                 key = "inventory_background",
--
--                 position = { 0, 50 },
--                 size = { 218, 100 },
--                 padding = { 20, 10, 8, 10 },
--                 background = "zeus:inventory:inventory",
--
--                 Gui.InventoryList {
--                     position = { 1, 1 },
--                     slot_spacing = { 2, 2 },
--                     source = "current_player",
--                     list = "main",
--                 }
--             },
--
--             Gui.Rect {
--                 key = "chest_background",
--
--                 position = { 0, -48 },
--                 size = { 218, 100 },
--                 padding = { 20, 10, 8, 10 },
--                 background = "zeus:inventory:chest",
--
--                 Gui.InventoryList {
--                     position = { 1, 1 },
--                     slot_spacing = { 2, 2 },
--                     source = "current_player",
--                     list = "main",
--                 }
--             }
--         }
    }
end)

inventory.open_chest = function()
    zepha.player:show_menu(chest)
end