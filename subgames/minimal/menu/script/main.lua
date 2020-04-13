local menu = zepha.build_gui(function()
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
        }
    }
end)

menu(function(e)
    e:append(Gui.Text {
        position = { 300, 16 },
        color = "#0fc",
        content = "Magic phones"
    })
end)

menu:append(function()
    return Gui.Text {
        position = { 300, 32 },
        color = "#f39",
        content = "AAAAAA"
    }
end)

zepha.set_gui(menu)