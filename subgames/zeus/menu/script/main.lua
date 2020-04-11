zepha.set_gui(zepha.build_gui(function()
    return Gui.Body {
        background = "zeus_background",

        Gui.Rect {
            key = "sidebar",
            position = { pc(20), 0 },
            position_anchor = { pc(50), 0 },
            size = { 102, pc(100) },
            background = "#0135",

            Gui.Rect {
                key = "logo",
                position = { 8, 8 },
                size = { 86, 30 },
                background = "zeus_logo"
            },

            Gui.Button {
                key = "buttonPlay",

                callbacks = {
                    primary = function()
                        zepha.start_game_local()
                    end
                },

                position = { 6, 50 },
                size = { 90, 20 },
                background = "crop(0, 0, 90, 20, zeus_button)",
                background_hover = "crop(0, 20, 90, 20, zeus_button)",
                content = "Local Play"
            },

            Gui.Button {
                key = "buttonServers",

                callbacks = {
                    primary = function()
                        zepha.start_game()
                    end
                },

                position = { 6, 74 },
                size = { 90, 20 },
                background = "crop(0, 0, 90, 20, zeus_button)",
                background_hover = "crop(0, 20, 90, 20, zeus_button)",
                content = "Browse Servers"
            }
        }
    }
end))