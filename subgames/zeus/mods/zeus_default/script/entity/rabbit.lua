local friendly = false

zepha.register_entity("zeus:default:rabbit", {
    display = "model",
    display_object = "zeus:default:rabbit",
    display_texture = "zeus:default:rabbit",

    on_create = function(self)
        self.object.scale = 1/16
        self.object.gravity = 40
        self.object.collides = true
        self.object.yaw = math.random(0, 360)
        self.object.collision_box = { { -4/16, 0/16, -4/16 }, { 4/16, 7/16, 4/16 } }

        self.object.anims:define({
            idle = {1, 181},
            run = {182, 210},
            stand_up = {213, 231},
            stand_idle = {232, 401}
        })

        self.object.anims:set_anim("run")
        self.object.anims:play()

        self.resting = false
        self.rest_timer = 0
        self.next_rest = 45 + math.random() * 30
        self.orient_timer = 0
        self.next_orient = 5 + math.random() * 3
    end,
    on_update = function(self, delta)
        self.rest_timer = self.rest_timer + delta
        self.orient_timer = self.orient_timer + delta

        if self.rest_timer > self.next_rest then
            self.resting = not self.resting

            if self.resting then
                self.object.vel = V {}
                self.object.anims:set_anim("idle")
                self.object.anims:play()
            else
                self.object.anims:set_anim("run")
                self.object.anims:play()
            end

            self.rest_timer = 0
            self.next_rest = self.resting and (3 + math.random() * 2) or (45 + math.random() * 30)
        end

        if self.orient_timer > self.next_orient then
            self.orient_timer = 0
            self.next_orient = 5 + math.random() * 3
            self.object.yaw = math.random(0, 360)
        end

        if not self.resting then
            if self.object.vel:length() < 2.8 and self.object.vel.y == 0 then
                self.object:set_vel(V { self.object.vel.x, 9, self.object.vel.z })
            end

            self.object.vel = V {
                3 * math.sin(math.rad(self.object.yaw + 90)),
                self.object.vel.y,
                3 * math.cos(math.rad(self.object.yaw + 90))
            }
        end
    end
})

if zepha.server then
    zepha.bind("new_player", function(player)
        for i = 0, 5 do
            player.dim:add_entity(player.pos + V { math.random(-100, 100), 30, math.random(-100, 100) }, "zeus:default:rabbit")
        end
    end)
end