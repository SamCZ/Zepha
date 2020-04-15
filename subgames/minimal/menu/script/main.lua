local gui = zepha.build_gui(function()
    return Gui.Body {
--         background = "#214a21",
        background = "#334",

        Gui.Text {
            position = { 4, 4 },
            content = "Minimal Subgame"
        },

        Gui.Rect {
            position = { 64, 64 },
            size = { 128 * (16/9), 128 },

            background = "zeus_background"
        },

        Gui.Rect {
            position = { 64 + 128, 64 + 64 },
            size = { 128 * (16/9), 128 },

            background = "zeus_background",

            Gui.Text {
                content = "What's the fuck is going on?"
            }
        },

        Gui.Rect {
            position = { 0, 0 },
            size = { 32, 32 },
            background = "#f00",
            key = "wee"
        }
    }
end)

zepha.set_gui(gui)

local menu = gui:find("wee")

zepha.delay(function()
    print("updating position")
    menu.position = {menu.position[1] + 16, menu.position[2] + 16}
    return true
end, 1)