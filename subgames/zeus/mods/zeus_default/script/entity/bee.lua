zepha.register_entity("zeus:default:bee", {
    display = "model",
    display_object = "zeus:default:bee",
    display_texture = "zeus:default:bee_shiny",

    on_create = function(self)
        self.object.scale = 1/12
        self.object.anims:define({
            fly = {1, 45}
        })

        self.object.anims:set_anim("fly"):play()
    end,
    on_update = function(self, delta)
        self.object.pos = self.object.pos +
            V(0.03 * math.sin(math.rad(self.object.yaw + 90)), 0, 0.03 * math.cos(math.rad(self.object.yaw + 90)))
        self.object.yaw = self.object.yaw + 2
    end
})

zepha.register_keybind("zeus:default:spawn_bee", {
    description = "Spawn Bee",
    default = zepha.keys.b,
    on_press = function()
        zepha.send_message("zeus:default:spawn", "bee");
    end
})

zepha.bind("message", function(channel, message, player)
    if channel ~= "zeus:default:spawn" then return end
    player.dim:add_entity(player.pos + V(0, 1.7, 0), "zeus:default:bee")
end)