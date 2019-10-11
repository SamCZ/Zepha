local function collides(entity)
    return zepha.get_block({
        x = math.floor(entity.pos.x),
        y = math.floor(entity.pos.y - 0.5),
        z = math.floor(entity.pos.z)}) ~= "air"
end

if not zepha.is_server() then
    zepha.register_entity("default:dropped_item", {
        display = "gameobject",
        display_object = "default:stone",

        on_load = function(self, static)
            if static == nil then static = {} end

            if (static.object) then self.object:set_display_type("gameobject", static.object) end
--            self.object:set_display_type("model", "zeus:default:rabbit", "zeus:default:rabbit")

            self.speed = static.speed or 60
            self.velocityY = static.velocityY or -3
            self.time = static.time or 0

            if self.time == 0 then self.object:set_scale(1) end
            if not zepha.registered_blocks[static.object] then
                self.object:int_scale(1/2)
            else
                self.object:int_scale(1/3)
            end

            zepha.delay(function()
                self:on_update(1/20)
                return true
            end, 1/20)

            zepha.delay(function()
                zepha.remove_entity(self)
            end, 60)
        end,
        on_update = function(self, delta)
            self.object:int_yaw(self.object.yaw + self.speed)
            if self.speed > 10 then self.speed = self.speed * 0.8 end

            if (not collides(self.object)) then
                self.velocityY = math.min(self.velocityY + 1, 16)
            end

            local v = 1
            while (not collides(self.object) and v <= math.abs(self.velocityY)) do
                local interval = 0
                if self.velocityY < 0 then interval = 1/16 else interval = -1/16 end
                self.object:int_pos({x = self.object.pos.x, y = self.object.pos.y + interval, z = self.object.pos.z})
                v = v + 0.25
            end
            self.object:int_visual_offset({x = 0, y = math.sin(self.time * 4) / 8, z = 0})
            if collides(self.object) then
                self.velocityY = 0
                self.time = self.time + delta
            end
        end,
        on_unload = function(self)
            return {
                velocityY = self.velocityY,
                time = self.time,
                speed = self.speed
            }
        end
    })
end