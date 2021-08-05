zepha.register_entity("zeus:default:test", {
    display = "model",
    display_object = "zeus:default:player",
    display_texture = "zeus:default:player",

    on_create = function(self)
        self.object.anims:define({
            walk = {0, 300}
        })
        self.object.anims:set_anim("walk")
        self.object.anims:play()

        self.object.gravity = 40
        self.object.collides = true
        self.object.collision_box = { V { -5/16, 0/16, -5/16 }, V { 5/16, 29/16, 5/16 } }

        math.randomseed(zepha.time.ns())

--        zepha.after(function()
--            self.object.dim:add_entity(self.object.pos, "zeus:default:test")
--            return true
--        end, 10)
    end,
    on_update = function(self, delta)
--        self.object.yaw = self.object.yaw + (180 * delta)
        self.object.vel = V {
            2 * math.sin(math.rad(self.object.yaw)),
            self.object.vel.y,
            2 * math.cos(math.rad(self.object.yaw))
        }

        if math.random() > 0.99 then
            self.object.yaw = math.random(0, 360)
        end

        if math.random() > 0.99 then
            self.object:set_vel(V { self.object.vel.x, 10, self.object.vel.z })
        end
    end
})

--if zepha.client then
--    zepha.after(function()
--        zepha.player.dim:add_entity(V(0, -34, 0), "zeus:default:test")
--    end, 0)
--end

-- if zepha.server then
--     zepha.bind("player_join", function(player)
--         for i = 0, 4 do
--             player.dim:add_entity(V(-145, -32, -30), "zeus:default:test")
--         end
--     end)
-- end