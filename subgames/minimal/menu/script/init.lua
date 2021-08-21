zepha.set_gui(zepha.gui(function()
    return Gui.Box {
        background = "#334",

        Gui.Text {
            pos = { 4, 4 },
            text_size = "2px",
            content = "Minimalminimalmmnal"
        },

        Gui.Box {
            pos = { 64, 64 },
            size = { 128 * (16/9), 128 },

            background = "zeus_background"
        },

        Gui.Box {
            pos = { 64 + 128, 64 + 64 },
            size = { 128 * (16/9), 128 },

            background = "zeus_background",

            Gui.Text {
                pos = 4,
                text_size = "4px",
                content = "What's the fuck it'd going on?"
            }
        }
    }
end))