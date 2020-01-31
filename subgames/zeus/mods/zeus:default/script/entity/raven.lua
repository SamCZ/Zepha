local friendly = false

zepha.register_entity("zeus:default:raven", {
    display = "model",
    display_object = "zeus:default:bird",
    display_texture = "zeus:default:raven",

    on_create = function(self)
        self.object:set_scale(1/16)

        self.object.anims:define({
            fly = {1, 100},
            preen = {101, 200},
            hover = {202, 260}
        })

        self.object.anims:set_anim("hover"):play()
    end,
    on_update = function(self, delta)
        local dist = vector.distance(zepha.player.pos, self.object.pos)

        if dist < 5 and not self.targeting then
            self.targeting = true
            self.object.anims:set_anim("fly"):play()
        elseif dist > 6 and self.targeting then
            self.targeting = false
            self.object.anims:set_anim("hover"):play()
        end

        if self.targeting then
            self.object.pos = v {
                self.object.pos.x + 0.08 * math.sin(math.rad(self.object.yaw)),
                self.object.pos.y,
                self.object.pos.z + 0.08 * math.cos(math.rad(self.object.yaw))
            }

            self.object.yaw = math.deg(math.atan2(zepha.player.pos.x - self.object.pos.x, zepha.player.pos.z - self.object.pos.z)) + 180
        else
            self.object.yaw = math.deg(math.atan2(zepha.player.pos.x - self.object.pos.x, zepha.player.pos.z - self.object.pos.z))
        end
    end
})

zepha.register_keybind("zeus:default:spawn_raven", {
    description = "Spawn Raven",
    default = zepha.keys.z,
    on_press = function() zepha.add_entity("zeus:default:raven", vector.add(zepha.player.pos, v{0, 1.7, 0})) end
})