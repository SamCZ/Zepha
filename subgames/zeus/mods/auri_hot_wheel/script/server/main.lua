zepha.bind('new_player', function(player)
    print('added inventories :)')
    local inv = player:get_inventory()
    inv:add_list('hot_wheel_1', 5, 5)
    inv:add_list('hot_wheel_2', 5, 5)
    inv:add_list('hot_wheel_3', 5, 5)
    inv:add_list('hot_wheel_4', 5, 5)
    inv:add_list('hot_wheel_5', 5, 5)
    inv:add_list('hot_wheel_6', 5, 5)
end)
