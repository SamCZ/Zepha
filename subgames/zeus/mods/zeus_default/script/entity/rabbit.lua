local friendly = false

zepha.register_entity("zeus:default:rabbit", {
    display = "model",
    display_object = "zeus:default:rabbit",
    display_texture = "zeus:default:rabbit",

    on_create = function(self)
        self.object:set_scale(1/16)

        self.object.anims:define({
            idle = {1, 181},
            run = {182, 210},
            stand_up = {213, 231},
            stand_idle = {232, 401}
        })

        self.object.anims:set_anim("run"):play()
    end,
    on_update = function(self, delta)
        local dist = vector.distance(zepha.player.pos, self.object.pos)

        if friendly then
            if dist > 3 and dist < 7 and not self.targeting then
                self.targeting = true
                self.object.anims:set_anim("run"):play()
            elseif dist < 2 or dist > 7 and self.targeting then
                self.targeting = false
                self.object.anims:set_anim("idle"):play()
            end

            if self.targeting then
                self.object.pos = self.object.pos +
                    V(0.08 * math.sin(math.rad(self.object.yaw)), 0, 0.08 * math.cos(math.rad(self.object.yaw)))
                self.object.yaw = math.deg(math.atan2(zepha.player.pos.x - self.object.pos.x, zepha.player.pos.z - self.object.pos.z))
            end
        else
            if dist < 5 and not self.targeting then
                self.targeting = true
                self.object.anims:set_anim("run"):play()
            elseif dist > 6 and self.targeting then
                self.targeting = false
                self.object.anims:set_anim("idle"):play()
            end

            if self.targeting then
                self.object.pos = self.object.pos +
                    V(0.08 * math.sin(math.rad(self.object.yaw)), 0, 0.08 * math.cos(math.rad(self.object.yaw)))
                self.object.yaw = math.deg(math.atan2(zepha.player.pos.x - self.object.pos.x, zepha.player.pos.z - self.object.pos.z)) + 180
            else
                self.object.yaw = math.deg(math.atan2(zepha.player.pos.x - self.object.pos.x, zepha.player.pos.z - self.object.pos.z))
            end
        end
    end
})

zepha.register_keybind("zeus:default:spawn_rabbit", {
    description = "Spawn Rabbit",
    default = zepha.keys.y,
    on_press = function()
        zepha.add_entity(zepha.player.pos, "zeus:default:rabbit")
    end
})

zepha.register_keybind("zeus:default:change_rabbit_behavior", {
    description = "Change rabbit behavior",
    default = zepha.keys.u,
    on_press = function()
        friendly = not friendly
    end
})