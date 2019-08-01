zeus.register_block("default:open_formspec", {
    name = "Open Formspec Block",
    model = "default:block",
    textures = {"default_brick"},
    on_place_client = function()
        print("Open Menu")
--        zeus.show_menu({
--            {
--                type = "rect",
--                size = {x = 32, y = 24},
--                padding = {h = 1, v = 1},
--                background_color = "#171717",
--                children = {
--                    {
--                        type = "text",
--                        contents = "Hello world",
--                        color = "#0ff",
--                        position = {x = 2, y = 0}
--                    }
--                }
--            }
--        })
    end
})