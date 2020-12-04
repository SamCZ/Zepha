zepha.register_item(':key_iron', {
    name = 'Iron Key',
    textures = { 'zeus:world:key_iron' }
})

zepha.register_item(':key_silver', {
    name = 'Silver Key',
    textures = { 'zeus:world:key_silver' }
})

zepha.register_item(':key_gold', {
    name = 'Gold Key',
    textures = { 'zeus:world:key_gold' }
})

if zepha.server then
    zepha.bind("new_player", function(player)
        local inv = player:get_inventory():get_list('main')
        inv:add_stack({'zeus:world:key_iron', 8})
        inv:add_stack({'zeus:world:key_silver', 4})
        inv:add_stack({'zeus:world:key_gold', 2})
    end)
end

