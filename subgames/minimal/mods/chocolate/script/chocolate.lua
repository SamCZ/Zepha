--Items

minetest.register_craftitem("chocolate:chocolate_bar_1", {
  description = "Chocolate Bar",
  inventory_image = "chocolate_chocolate_bar_1.png",
  on_use = minetest.item_eat(2)
})

minetest.register_craftitem("chocolate:chocolate_bar_2", {
  description = "Chocolate Bar",
  inventory_image = "chocolate_chocolate_bar_2.png",
  on_use = minetest.item_eat(2, "chocolate:chocolate_bar_1")
})

minetest.register_craftitem("chocolate:chocolate_bar_3", {
  description = "Chocolate Bar",
  inventory_image = "chocolate_chocolate_bar_3.png",
  on_use = minetest.item_eat(2, "chocolate:chocolate_bar_2")
})

minetest.register_craftitem("chocolate:chocolate_bar_4", {
  description = "Chocolate Bar",
  inventory_image = "chocolate_chocolate_bar_4.png",
  on_use = minetest.item_eat(2, "chocolate:chocolate_bar_3")
})

minetest.register_craftitem("chocolate:chocolate_bar_5", {
  description = "Chocolate Bar",
  inventory_image = "chocolate_chocolate_bar_5.png",
  on_use = minetest.item_eat(2, "chocolate:chocolate_bar_4")
})

minetest.register_craftitem("chocolate:chocolate_bar_6", {
  description = "Chocolate Bar",
  inventory_image = "chocolate_chocolate_bar_6.png",
  on_use = minetest.item_eat(2, "chocolate:chocolate_bar_5")
})

minetest.register_craftitem("chocolate:chocolate_bar_7", {
  description = "Chocolate Bar",
  inventory_image = "chocolate_chocolate_bar_7.png",
  on_use = minetest.item_eat(2, "chocolate:chocolate_bar_6")
})

minetest.register_craftitem("chocolate:chocolate_bar_8", {
  description = "Chocolate Bar",
  inventory_image = "chocolate_chocolate_bar_8.png",
  on_use = minetest.item_eat(2, "chocolate:chocolate_bar_7")
})

minetest.register_craftitem("chocolate:chocolate_bar", {
  description = "Chocolate Bar",
  inventory_image = "chocolate_chocolate_bar_new.png",
  on_use = minetest.item_eat(0, "chocolate:chocolate_bar_8")
})

--Recipes

minetest.register_craft({
    output = "chocolate:chocolate_bar",
    recipe = {
      {"chocolate:cocoa", "chocolate:cocoa"},
      {"chocolate:cocoa", "chocolate:cocoa"}
    }
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_2",
    recipe = {"chocolate:chocolate_bar_1", "chocolate:chocolate_bar_1"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_3",
    recipe = {"chocolate:chocolate_bar_1", "chocolate:chocolate_bar_2"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_4",
    recipe = {"chocolate:chocolate_bar_1", "chocolate:chocolate_bar_3"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_5",
    recipe = {"chocolate:chocolate_bar_1", "chocolate:chocolate_bar_4"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_6",
    recipe = {"chocolate:chocolate_bar_1", "chocolate:chocolate_bar_5"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_7",
    recipe = {"chocolate:chocolate_bar_1", "chocolate:chocolate_bar_6"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_8",
    recipe = {"chocolate:chocolate_bar_1", "chocolate:chocolate_bar_7"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_4",
    recipe = {"chocolate:chocolate_bar_2", "chocolate:chocolate_bar_2"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_5",
    recipe = {"chocolate:chocolate_bar_2", "chocolate:chocolate_bar_3"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_6",
    recipe = {"chocolate:chocolate_bar_2", "chocolate:chocolate_bar_4"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_7",
    recipe = {"chocolate:chocolate_bar_2", "chocolate:chocolate_bar_5"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_8",
    recipe = {"chocolate:chocolate_bar_2", "chocolate:chocolate_bar_6"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_6",
    recipe = {"chocolate:chocolate_bar_3", "chocolate:chocolate_bar_3"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_7",
    recipe = {"chocolate:chocolate_bar_3", "chocolate:chocolate_bar_4"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_8",
    recipe = {"chocolate:chocolate_bar_3", "chocolate:chocolate_bar_5"}
})

minetest.register_craft({
    type = "shapeless",
    output = "chocolate:chocolate_bar_8",
    recipe = {"chocolate:chocolate_bar_4", "chocolate:chocolate_bar_4"}
})