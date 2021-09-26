-- Register hand item
zepha.register_item(':hand', {
    name = 'Hand (you broke the game)',

    textures = { 'zepha:base:hand' },

    tool_props = {
        interval = 0.25,
        damage_groups = { grab = 1, _other = 0 }
    }
})

-- Register main and hand inventories
if zepha.server then
    zepha.bind('new_player', function(p)
        local inv = p:get_inventory()

        local main = inv:add_list('main', 30, 10)
        inv:set_default_list(main)

        local hand = inv:add_list('hand', 1, 1)
        hand:add_stack({ 'zepha:base:hand', 1 })
        p:set_hand_list(hand)
    end)
end
