-- Register base models
runfile(_PATH .. "models/_index")

-- Load Libraries
runfile(_PATH .. "dump")
runfile(_PATH .. "math")
runfile(_PATH .. "vector")

-- Set flying toggle
zepha.register_keybind("base:toggle_flying", {
    description = "Toggle Flying",
    default = zepha.keys.f,
    on_press = function() zepha.player.flying = not zepha.player.flying end
})

-- Signal completion
print("Base definitions loaded.")