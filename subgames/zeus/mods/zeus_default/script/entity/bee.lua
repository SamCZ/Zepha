
zepha.register_entity("zeus:default:bee", {
    display = "model",
    display_object = "zeus:default:bee",
    display_texture = "zeus:default:bee",

    on_create = function(self)
        self.object.scale = 1/12
        self.object.gravity = 3
        self.object.collides = true
        self.object.collision_box = { { -3/16, 0/16, -3/16 }, { 3/16, 4/16, 3/16 } }

        self.object.anims:define({ fly = {1, 45} })
        self.object.anims:set_anim("fly")
        self.object.anims:play()
        self.object.yaw = math.random(0, 360)

        self.resting = false
        self.rest_timer = 0
        self.next_rest = 45 + math.random() * 30
        self.orient_timer = 0
        self.next_orient = 5 + math.random() * 3

        if math.random() < (1/1024) then
            self.object:set_display_type('model', 'zeus:default:bee', 'zeus:default:bee_shiny')
        end
    end,
    on_update = function(self, delta)
        self.rest_timer = self.rest_timer + delta
        self.orient_timer = self.orient_timer + delta

        if self.rest_timer > self.next_rest then
            self.resting = not self.resting

            if self.resting then
                self.object.vel = V {}
                self.object.anims:pause()
            else
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
            self.object.vel = V {
                1 * math.sin(math.rad(self.object.yaw + 90)),
                self.object.vel.y,
                1 * math.cos(math.rad(self.object.yaw + 90))
            }

            local fly = false
            local def = zepha.registered_blocks[self.object.dim:get_block(self.object.pos:floor() - V{ 0, 2, 0 })]
            if def and (def.solid == nil or def.solid ~= false) then fly = true end

            if fly and self.object.vel.y <= 0 then
                self.object.vel = self.object.vel + V { 0, 1, 0 }
            end
        end
    end
})

if zepha.server then
    zepha.bind("new_player", function(player)
        for i = 0, 10 do
            player.dim:add_entity(player.pos + V { math.random(-100, 100), 30, math.random(-100, 100) }, "zeus:default:bee")
        end
    end)
end