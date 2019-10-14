print("Hello from default")

runfile(_PATH .. "models/_index")
runfile(_PATH .. "blocks/_index")
runfile(_PATH .. "entity/_index")

zepha.register_keybind("open_inventory", {
    description = "Open Inventory",
    default = "E",
    on_press = function()
        print("Inventory Key Down")
    end,
    on_release = function()
        print("Inventory Key Up")
    end
})

zepha.register_keybind("open_chat", {
    description = "Open Chat",
    default = "T",
    on_press = function()
        print("Chat Key Down")
    end,
    on_release = function()
        print("Chat Key Up")
    end
})