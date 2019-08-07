zeus.register_block("default:open_formspec", {
    name = "Open Formspec Block",
    model = "default:block",
    textures = {"zeus:default:brick"},
    on_place_client = function()
        zeus.show_menu({
            key = "form_root",
            type = "rect",

            size = "1920px, 1080px",
            background = "#0004",

            children = {
                {
                    key = "inv_root",
                    type = "rect",

                    position = "202px, 138px",
                    size = "236px, 118px",
                    padding = "20px, 10px, 8px, 10px",
                    background = "inventory"
                }, {
                    key = "hotwheel_root",
                    type = "rect",

                    position = "296px, 268px",
                    size = "52px, 52px",
                    background = "inventory_wheel"
                }
            }
        });
    end
})