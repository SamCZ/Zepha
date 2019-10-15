print("Hello from default")

runfile(_PATH .. "models/_index")
runfile(_PATH .. "blocks/_index")
runfile(_PATH .. "entity/_index")

zepha.register_keybind("open_inventory", {
    description = "Open Inventory",
    default = zepha.keys.e,
    on_press = function()
        print("Inventory Toggle")
    end
})

zepha.register_keybind("open_chat", {
    description = "Open Chat",
    default = zepha.keys.t,
    on_press = function()
        print("Chat Toggle")
    end
})