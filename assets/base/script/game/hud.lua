zepha.player:set_hud(zepha.gui(function()
    return Gui.Box {
        background = "base:viginette",

        Gui.Box {
--             id = "crosshair",

            size = { "22dp", "22dp" },
            pos = { "50cw - 50sw", "50ch - 50sh" },

            background = "base:crosshair"
        }
    }
end))