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

        zepha.after(function()
--            local in_desert = self.object.dimension.identifier == 'zeus:world:endless_desert'
--            player:set_dimension(in_desert and "zeus:world:default" or "zeus:world:endless_desert")
        end, 2)
    end,
    on_update = function(self, delta)
       self.object.pos = self.object.pos +
           V(0.6 * math.sin(math.rad(self.object.yaw)), 0, 0.6 * math.cos(math.rad(self.object.yaw))) * delta
       self.object.yaw = self.object.yaw + 50 * delta
    end
})

if zepha.client then
    zepha.after(function()
        zepha.player.dim:add_entity(V(0, -34, 0), "zeus:default:test")
    end, 0)
end

if zepha.server then
    zepha.bind("player_join", function(player)
        player.dim:add_entity(V(0, -32, 0), "zeus:default:test")
    end)
end