local function collides(entity)
    return zepha.get_block({
        x = math.floor(entity.pos.x),
        y = math.floor(entity.pos.y - 0.5),
        z = math.floor(entity.pos.z)}) ~= "air"
end

if not zepha.is_server() then
    zepha.register_entity("default:dropped_item", {
        display = "gameobject",
        display_object = "default:leaves",

        on_load = function(self, static)
            if static == nil then static = {} end

            if (static.object) then self.entity:set_display_type("gameobject", static.object) end

            self.speed = static.speed or 60
            self.velocityY = static.velocityY or -3
            self.time = static.time or 0

            if self.time == 0 then self.entity:set_scale(1) end
            if not zepha.registered_blocks[static.object] then
                self.entity:int_scale(1/2)
            else
                self.entity:int_scale(1/3)
            end

            zepha.delay(function()
                self:on_update(1/20)
                return true
            end, 1/20)
        end,
        on_update = function(self, delta)
            self.entity:int_yaw(self.entity.yaw + self.speed)
            if self.speed > 10 then self.speed = self.speed * 0.8 end

            if (not collides(self.entity)) then
                self.velocityY = math.min(self.velocityY + 1, 16)
            end

            local v = 1
            while (not collides(self.entity) and v <= math.abs(self.velocityY)) do
                local interval = 0
                if self.velocityY < 0 then interval = 1/16 else interval = -1/16 end
                self.entity:int_pos({x = self.entity.pos.x, y = self.entity.pos.y + interval, z = self.entity.pos.z})
                v = v + 0.25
            end
            self.entity:int_visual_offset({x = 0, y = math.sin(self.time * 4) / 8, z = 0})
            if collides(self.entity) then
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