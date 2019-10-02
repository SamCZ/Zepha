zepha.register_block("default:grass", {
    name = "Grass",
    model = "default:block_foliage",
    textures = {
        "zeus:default:grass_top",
        "zeus:default:dirt",
        "zeus:default:grass_side",
        "zeus:default:grass_side",
        "zeus:default:grass_side",
        "zeus:default:grass_side",
        "zeus:default:grass_floating",
    },
    lowdef_textures = {
        "zeus:default:grass_top",
        "zeus:default:dirt",
        "zeus:default:grass_side_ld"
    },
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    },
    on_break_client = function(pos)
        local entity = LuaEntity.new(pos)
        table.insert(entities, entity)

        local speed = 60
        local velocityY = -3
        local time = 0

        zepha.delay(function()
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
            if collides(entity) then
                velocityY = 0
                time = time + 1/20
            end

            entity.visual_offset = {x = 0, y = math.sin(time * 4) / 8, z = 0}

            return true
        end, 1/20)
    end
})