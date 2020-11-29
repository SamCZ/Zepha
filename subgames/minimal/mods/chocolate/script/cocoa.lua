minetest.register_node("chocolate:cocoa_plant", {
  description = "Cocoa Seed",
  drawtype = "plantlike",
  tiles = {"chocolate_cocoa_stalk.png"},
  inventory_image = "chocolate_cocoa_seeds.png",
  sunlight_propogates = true,
  walkable = false,
  -- waving = 1,
  paramtype = "light",
  drop = "chocolate:cocoa_plant",
  groups = {snappy = 3, flora = 1, flammable = 2, plant = 1},
  on_place = function(itemstack, placer, pointed_thing)
    pos = minetest.get_pointed_thing_position(pointed_thing, true)
    node = minetest.get_node({x = pos.x, y = pos.y-1, z = pos.z})
    if node.name == "default:dirt_with_grass" 
    or node.name == "farming:soil_wet" then
      minetest.item_place(itemstack, placer, pointed_thing)
    end
    return itemstack
  end,
  on_destruct = function(pos)
    print("destruction")
    if minetest.get_node({x = pos.x, y = pos.y+1, z = pos.z}).name == "chocolate:cocoa_plant" 
    or minetest.get_node({x = pos.x, y = pos.y+1, z = pos.z}).name == "chocolate:cocoa_plant_top"
    or minetest.get_node({x = pos.x, y = pos.y+1, z = pos.z}).name == "chocolate:cocoa_plant_top_grown" then
      minetest.dig_node({x = pos.x, y = pos.y+1, z = pos.z})
    end
    if minetest.get_node({x = pos.x, y = pos.y+2, z = pos.z}).name == "chocolate:cocoa_plant" 
    or minetest.get_node({x = pos.x, y = pos.y+2, z = pos.z}).name == "chocolate:cocoa_plant_top"
    or minetest.get_node({x = pos.x, y = pos.y+2, z = pos.z}).name == "chocolate:cocoa_plant_top_grown"  then
      minetest.dig_node({x = pos.x, y = pos.y+2, z = pos.z})
    end
    if minetest.get_node({x = pos.x, y = pos.y+3, z = pos.z}).name == "chocolate:cocoa_plant" 
    or minetest.get_node({x = pos.x, y = pos.y+3, z = pos.z}).name == "chocolate:cocoa_plant_top"
    or minetest.get_node({x = pos.x, y = pos.y+3, z = pos.z}).name == "chocolate:cocoa_plant_top_grown" then
      minetest.dig_node({x = pos.x, y = pos.y+3, z = pos.z})
    end
  end
})

minetest.register_node("chocolate:cocoa_plant_top", {
  description = "Cocoa Head",
  drawtype = "plantlike",
  tiles = {"chocolate_cocoa_head.png"},
  sunlight_propogates = true,
  walkable = false,
  waving = 1,
  paramtype = "light",
  drop = "chocolate:cocoa_plant",
  groups = {snappy = 3, flora = 1, flammable = 2, plant = 1, not_in_creative_inventory = 1}
})

minetest.register_node("chocolate:cocoa_plant_top_grown", {
  description = "Cocoa Head",
  drawtype = "plantlike",
  tiles = {"chocolate_cocoa_head_grown.png"},
  sunlight_propogates = true,
  walkable = false,
  waving = 1,
  paramtype = "light",
  drop = "chocolate:cocoa_plant",
  on_punch = function(pos, node, player, pointed_thing)
    n = math.floor(1 + (math.random()*3.4))
    player:get_inventory():add_item("main", ("chocolate:cocoa " .. n))
    minetest.set_node({x = pos.x, y = pos.y, z = pos.z}, {name = "chocolate:cocoa_plant_top"})
  end,
  groups = {snappy = 3, flora = 1, flammable = 2, plant = 1, not_in_creative_inventory = 1}
})

minetest.register_abm({
  nodenames = {"chocolate:cocoa_plant"},
  interval = 60,
  chance = 8,
  action = function(pos, node, active_object_count, active_object_count_wider)
    if (minetest.get_node({ x = pos.x, y = pos.y-1, z = pos.z}).name ~= "chocolate:cocoa_plant"
    or minetest.get_node({ x = pos.x, y = pos.y-2, z = pos.z}).name ~= "chocolate:cocoa_plant")
    and minetest.get_node({ x = pos.x, y = pos.y+1, z = pos.z}).name == "air" then

      minetest.set_node({x = pos.x, y = pos.y + 1, z = pos.z}, {name = "chocolate:cocoa_plant"})

    elseif minetest.get_node({ x = pos.x, y = pos.y-3, z = pos.z}).name ~= "chocolate.cocoa_plant"
    and minetest.get_node({x = pos.x, y = pos.y+1, z = pos.z}).name == "air" then 

      minetest.set_node({x = pos.x, y = pos.y + 1, z = pos.z}, {name = "chocolate:cocoa_plant_top"})

    end
  end
})

minetest.register_abm({
  nodenames = {"chocolate:cocoa_plant_top"},
  interval = 60,
  chance = 8,
  action = function(pos, node, active_object_count, active_object_count_wider)
    minetest.set_node(pos, {name = "chocolate:cocoa_plant_top_grown"})
  end
})

minetest.register_craftitem("chocolate:cocoa", {
  description = "Cocoa",
  inventory_image = "chocolate_cocoa.png"
})