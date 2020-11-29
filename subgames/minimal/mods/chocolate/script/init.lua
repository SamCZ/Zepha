print("Hello, world! I am from chocolate init.lua!");

dofile(minetest.get_modpath("chocolate") .. "/cocoa.lua")
dofile(minetest.get_modpath("chocolate") .. "/chocolate.lua")

minetest.register_node("chocolate:testblock", {
	description = "Testy",
	tiles = {
		"chocolate_cocoa_head_grown.png",
		"chocolate_cocoa_head.png",
		"chocolate_cocoa_stalk.png",
		"chocolate_cocoa_stalk.png",
		"chocolate_cocoa_stalk.png",
		"chocolate_cocoa_stalk.png"
	},
	groups = {cracky = 1}
})
