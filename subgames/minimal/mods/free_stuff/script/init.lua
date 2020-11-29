if zepha.server then
  zepha.bind("new_player", function(player)
    local main = player:get_inventory():get_list("main")
    local hw = player:get_inventory():get_list("hot_wheel_2")
    
    main:add_stack({"minetest:chocolate:cocoa", 1})

    hw:add_stack({"minetest:chocolate:cocoa_plant", 5})
    hw:add_stack({"minetest:chocolate:cocoa_plant_top", 5})
    hw:add_stack({"minetest:chocolate:cocoa_plant_top_grown", 5})
    hw:add_stack({"minetest:chocolate:testblock", 5})

    -- hw:add_stack({"minetest:lantern:lantern", 5})
    -- hw:add_stack({"minetest:lantern:airlight", 1})

    for i = 1, 8 do
    	main:add_stack({"minetest:chocolate:chocolate_bar_" .. i, 1})
  	end
  	main:add_stack({"minetest:chocolate:chocolate_bar", 1})
  end)
end
