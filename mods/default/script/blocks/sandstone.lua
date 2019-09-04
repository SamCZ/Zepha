local entities = {}

local function collides(entity)
    return zeus.get_block({
        x = math.floor(entity.pos.x),
        y = math.floor(entity.pos.y - 0.5),
        z = math.floor(entity.pos.z)}) ~= "air"
end

zeus.register_block("default:sandstone", {
    name = "Sandstone",
    model = "default:block",
    textures = {"zeus:default:sandstone"},
    toughness = {
        hand = 14,
        pick = 3
    },
    on_break_client = function(pos)
        local entity = LuaEntity.new(pos)
        table.insert(entities, entity)

        local speed = 60
        local velocityY = -3
        local time = 0

        zeus.delay(function()
            entity.yaw = entity.yaw + speed
            if speed > 10 then speed = speed * 0.8 end

            if (not collides(entity)) then
                velocityY = math.min(velocityY + 1, 16)
            end

            local v = 1
            while (not collides(entity) and v <= math.abs(velocityY)) do
                local interval
                if velocityY < 0 then interval = 1/16 else interval = -1/16 end
                entity.pos = {x = entity.pos.x, y = entity.pos.y + interval, z = entity.pos.z}
                v = v + 0.25
            end

            entity.visual_offset = {x = 0, y = math.sin(time * 4) / 8, z = 0}
            time = time + 1/20

            return true
        end, 1/20)
    end
})