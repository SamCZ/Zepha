zepha.create_dimension("zeus:default")
zepha.set_default_dimension("zeus:default")

zepha.create_dimension("zeus:second")

local v = false
zepha.register_keybind("zeus:world:go_to_hell", {
    default = zepha.keys.enter,
    on_press = function()
        print("Here we go again")

        if not v then
            zepha.player:set_dimension("zeus:second")
            v = true
        else
            zepha.player:set_dimension("zeus:default")
            v = false
        end
    end
})