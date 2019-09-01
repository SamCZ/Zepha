print("Hello from default")

if not zeus.is_server() then
    local entity = LuaEntity.new()

    zeus.delay(function()
        zeus.delay(function()
            entity:set_pos({x = entity:get_pos().x + (1/20), y = 40, z = 0})
--            entity:set_scale(entity:get_scale() * 1.02);
            entity:set_yaw(entity:get_yaw() + 15);
            return true
        end, 1/20)
--        entity:set_pos({x = entity:get_pos().x + 30, y = 40, z = 0})
    end, 2)
end

dofile("zeus:default/models")
dofile("zeus:default/blocks")
dofile("zeus:default/items")