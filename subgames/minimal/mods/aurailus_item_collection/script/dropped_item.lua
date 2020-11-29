local function collides(entity)
    return entity.dim:get_block((entity.pos - V{0, 0.5, 0}):floor()) ~= "air"
end

zepha.register_entity("@aurailus:item_collection:dropped_item", {
    display = "gameobject",
    display_object = "invalid",

    on_create = function(self, static)
        static = static or {}
        self.item = static.item or self.display_object

        self.object:set_display_type("gameobject", self.item)

        self.tick = 0
        self.time = static.time or 0
        self.speed = static.speed or 15

        self.delete = false
        self.scooping = false

        local angle = math.random() * (math.pi*2)
        local amp = (math.random() + 0.5) * 5
        local x = math.sin(angle) * amp
        local z = math.cos(angle) * amp

        self.vel = static.vel or V(x, -85, z)

        if not zepha.registered_blocks[self.item] then self.object.scale = 1/2
        else self.object.scale = 1/3 end
    end,
    on_update = function(self, delta)
        self.object.yaw = self.object.yaw + self.speed
        if self.speed > 1 then self.speed = self.speed * 0.92 end

        self.tick = self.tick + delta

        if self.time > 5 * 60 then
            zepha.remove_entity(self)
            return
        end

        if self.scooping then return end

        if not collides(self.object) then
            self.vel.y = math.min(self.vel.y + 300 * delta, 480)
        end

        local iter = 1
        while not collides(self.object) and iter <= math.abs(self.vel.y * delta) do
            local interval = 1/16
            if self.vel.y > 0 then interval = -1/16 end

            self.object.pos = self.object.pos + V{0, interval, 0}
            iter = iter + 0.25
        end

        self.object.pos = self.object.pos + V(self.vel.x, 0, self.vel.z) * delta
        self.vel.x = self.vel.x * 0.6
        self.vel.z = self.vel.z * 0.6

        self.object.visual_offset = V{0, math.sin(self.time * 2) / 8, 0}

        if collides(self.object) then
            self.vel.y = 0
            self.time = self.time + delta
        end

        if self.tick > 0.15 then
            self.tick = 0
            self:check_collect()
        end
    end,
    check_collect = function(self)
        for _,p in pairs(zepha.players) do
            if p.pos:distance(self.object.pos) < 2 then
                self.object.pos = p.pos + V{0, 0.90, 0}
                self.scooping = true

                zepha.after(function()
                    p:get_inventory():get_list("main"):add_stack({self.item, 1})
                    self.object.dim:remove_entity(self)
                end, 2/20)
            end
        end
    end,
    on_serialize = function(self)
        return {
            vel = self.vel,
            time = self.time,
            speed = self.speed
        }
    end
})