zepha.register_entity("zeus:default:test", {
    display = "model",
    display_object = "zeus:default:player",
    display_texture = "zeus:default:player",

    on_create = function(self)
        self.object.anims:define({
            walk = {0, 300}
        })
        self.object.anims:set_anim("walk"):play()
        self.object.scale = 1/4
    end,
    on_update = function(self, delta)
--        self.object.pos = self.object.pos +
--            V(0.6 * math.sin(math.rad(self.object.yaw)), 0, 0.6 * math.cos(math.rad(self.object.yaw))) * delta
--        self.object.yaw += 50 * delta
    end
})

if zepha.client then zepha.after(function() zepha.get_default_dimension():add_entity(V(), "zeus:default:test") end, 0) end