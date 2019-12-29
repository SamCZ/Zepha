zepha.register_entity("zeus:default:test", {
    display = "model",
    display_object = "zeus:default:player",
    display_texture = "zeus:default:player",

    on_create = function(self)
--        self.object:set_scale(1/4)
    end,
    on_update = function(self, delta)
       local pos = self.object.pos
       pos.z = pos.z + 1 * delta
       if pos.z > 13 then
           pos.z = 0
       end
       self.object:int_pos(pos)
       self.object.yaw = self.object.yaw + 1;

--         self.object:int_pos({
--             x = self.object.pos.x + 0.08 * math.sin(math.rad(self.object.yaw)),
--             y = self.object.pos.y,
--             z = self.object.pos.z + 0.08 * math.cos(math.rad(self.object.yaw))})
--         self.object:int_yaw(self.object.yaw + 2)
--
--         self.object:int_yaw(math.deg(math.atan2(zepha.player.pos.x - self.object.pos.x, zepha.player.pos.z - self.object.pos.z)) + 180)
    end
})

if zepha.server then
    zepha.add_entity("zeus:default:test", {x = 10, y = 35, z = 0})
end