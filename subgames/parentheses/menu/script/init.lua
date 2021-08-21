zepha.set_gui(zepha.gui(function()
    return Gui.Box {
        background = "#124778",
        h_align = "center",
        v_align = "center",

        Gui.Text {
            size = { 64, 4 },
            content = "Parentheses"
        }
    }
end))