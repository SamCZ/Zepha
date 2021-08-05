zepha.register_item(':key_iron', {
    name = 'Iron Key',
    textures = { 'zeus:world:key_iron' },

    on_use = function(stack, target, player)
        player.dim:add_entity(player.pos, "zeus:default:test")
        stack.count = stack.count - 1
        return stack
    end
})

zepha.register_item(':key_silver', {
    name = 'Silver Key',
    textures = { 'zeus:world:key_silver' },

    on_use = function(stack, target, player)
        player.dim:add_entity(player.pos, "zeus:default:rabbit")
        stack.count = stack.count - 1
        return stack
    end
})

zepha.register_item(':key_gold', {
    name = 'Gold Key',
    textures = { 'zeus:world:key_gold' },
    stack = 1024,
    on_use = function(stack, target, player)
        player.dim:add_entity(player.pos, "zeus:default:bee")
        stack.count = stack.count - 1
        return stack
    end
})

if zepha.server then
    zepha.bind("new_player", function(player)
        local inv = player:get_inventory():get_list('hot_wheel_2')
        inv:add_stack({'zeus:world:key_iron', 64})
        inv:add_stack({'zeus:world:key_silver', 64})
        inv:add_stack({'zeus:world:key_gold', 1024})
    end)
end

