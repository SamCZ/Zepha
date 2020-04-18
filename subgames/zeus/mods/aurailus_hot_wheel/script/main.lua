if zepha.server then return end

local hud = zepha.player:get_hud()
hud:append(function() return Gui.Rect {
    key = "hot_wheel_root",

    size = { 139, 70 },
    position = { "0%", "100%" },
    position_anchor = { "-10%", "110%" },
--     background = "#966"
} end)
local root = hud:get("hot_wheel_root")

root(function(e)

    e:append(Gui.Rect {
        size = { 68, 68 },
        background = "@aurailus:hot_wheel:hot_wheel_circle",

        Gui.InventoryList {
            position = { 7, 1 },
            slot_spacing = { 2, 2 },
            length = 1,
            source = "current_player",
            list = "main",
        }
    })

    e:append(Gui.Rect {
        size = { 84, 18 },
        position = { 47, 13 },
        background = "@aurailus:hot_wheel:hot_wheel_line",
    })

end)