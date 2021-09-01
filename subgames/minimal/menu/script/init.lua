zepha.set_gui(zepha.gui(function()
    return Gui.Box {
        background = "#334",

        Gui.Text {
            pos = { "4dp", "4dp" },
            text_size = "2px",
            content = "Minimalminimalmmnal"
        },

        Gui.Box {
            pos = { "64dp", "64dp" },
            size = { "128dp * (16/9)", "128dp" },

            background = "zeus_background"
        },

        Gui.Box {
            pos = { "64dp + 128dp", "128dp" },
            size = { "128dp * (16/9)", "128dp" },

            background = "zeus_background",

            Gui.Text {
                pos = "4dp",
                text_size = "4px",
                content = "What's the fuck it'd going on?"
            }
        }
    }
end))