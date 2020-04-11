local menu = zepha.build_gui(function()
     return Gui.Body {
         background = "#214a21",

         Gui.Text {
             position = { 4, 4 },
             content = "Minimal Subgame"
         }
     }
end)

zepha.set_gui(menu)