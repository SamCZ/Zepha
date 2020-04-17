zepha.player:set_hud(zepha.build_gui(function()
    return Gui.Body {
        background = "base:viginette",

        Gui.Rect {
            key = "crosshair",
            position = { pc(50), pc(50) },
            position_anchor = { pc(50), pc(50) },
            size = { 22 / 3, 22 / 3 },

            background = "base:crosshair"
        }
    }
end))