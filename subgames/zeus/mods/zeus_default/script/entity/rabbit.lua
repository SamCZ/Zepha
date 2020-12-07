local ATTACK_INTERVAL = 1

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

        self.attack_timer = 0
    end,
    on_update = function(self, delta)
        if self.attack_timer > 0 then
            self.attack_timer = math.max(self.attack_timer - delta, 0)
        end

        local closest_player = nil
        for _, player in pairs(zepha.players) do
            if not closest_player or vector.dist(self.object.pos, player.pos)
                < vector.dist(self.object.pos, closest_player.pos) then
                closest_player = player
            end
        end

        if not closest_player then return end

        local offset = closest_player.pos - self.object.pos
        local dist = offset:length()
        if dist > 16 then
            self.attack_timer = ATTACK_INTERVAL
            return
        end

        if dist > 0.5 then
            local dir = offset:unit()

            local yaw = math.atan2(dir.x, dir.z)
            self.object.yaw = math.round(math.deg(yaw) + 270)

            if self.object.vel:length() < 2.8 and self.object.vel.y == 0 then
                self.object:set_vel(V { self.object.vel.x, 9, self.object.vel.z })
            end
        end

        if dist > 0.25 then
            self.object.vel = V {
                3 * math.sin(math.rad(self.object.yaw + 90)),
                self.object.vel.y,
                3 * math.cos(math.rad(self.object.yaw + 90))
            }
        else
            self.object.vel = V {}
            if self.attack_timer == 0 then
                health.damage_player(closest_player, 1)
                self.attack_timer = ATTACK_INTERVAL
            end
        end
    end
})