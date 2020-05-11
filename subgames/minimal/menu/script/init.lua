local gui = zepha.build_gui(function()
    return Gui.Body {
--         background = "#214a21",
        background = "#334",

        Gui.Text {
            position = { 4, 4 },
            content = "Minimalminimalmmnal"
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
                content = "What's the fuck it'd going on?"
            }
        }
    }
end)

zepha.set_gui(gui)