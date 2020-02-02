zepha.register_entity("zeus:default:test", {
    display = "model",
    display_object = "zeus:default:player",
    display_texture = "zeus:default:player",

    on_create = function(self)
--         self.object.set_animations({
--             {"wal0, 100}
--         })
--        self.object:snap_scale(1/4)
    end,
    on_update = function(self, delta)
        self.object:set_pos({
            x = self.object.pos.x + 0.08 * math.sin(math.rad(self.object.yaw)),
            y = self.object.pos.y,
            z = self.object.pos.z + 0.08 * math.cos(math.rad(self.object.yaw))})
        self.object:set_yaw(self.object.yaw + 2)
    end
})

if zepha.server then
    local entity = zepha.add_entity("zeus:default:test", {x = 10, y = 0, z = 0})

    zepha.delay(function()
        zepha.remove_entity(entity)
    end, 10)
end