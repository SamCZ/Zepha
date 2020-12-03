zepha.bind("new_player", function(p)
    local inv = p:get_inventory()

    -- Resize the main inventory
    inv:get_list("main"):resize(15, 5);

    -- Create inventory mod slot
    inv:add_list("mod", 1, 1);

    -- Create and bind crafting inventories
    local craft_input = inv:add_list("craft", 4, 2)
    local craft_output = inv:add_list("craft_result", 1, 1)
    crafting.bind(craft_input, craft_output)
end)