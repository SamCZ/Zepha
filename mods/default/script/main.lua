print("Hello from default")

runfile(_PATH .. "models/_index")
runfile(_PATH .. "blocks/_index")
runfile(_PATH .. "entity/_index")

zepha.register_keybind("open_inventory", {
    description = "Open Inventory",
    default = zepha.keys.e,
    on_press = function()
        zepha.show_menu({
            key = "form_root",
            type = "rect",

            size = "1920px, 1080px",
            background = "#0004",

            children = {{
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
            } }
        });

        zepha.show_menu([[
            rect[form_root]
                size=1920px 1080px
                background=#0004

                rect[inv_root]
                    position=202px 138px
                    size=236px 138px
                    padding=20px 10px 8px 10px
                    background=img("inventory")

                rect[hotwheel_root]
                    position=296px 268px
                    size=52px 52px
                    background=img("inventory_wheel")
        ]]);
    end
})

zepha.register_keybind("open_chat", {
    description = "Open Chat",
    default = zepha.keys.t,
    on_press = function()
        print("Chat Toggle")
    end
})
