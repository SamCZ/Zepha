--
-- Register basic blocks
--

-- Grass
zeus.register_block("default:grass", {
	name = "Grass",
	textures = {
		"default_grass_top",
		"default_dirt",
		"default_grass_side"
	}
})

-- Dirt
zeus.register_block('default:dirt', {
	name = "Dirt",
	textures = {"default_dirt"}
})

-- Stone
zeus.register_block('default:stone', {
	name = "Stone",
	textures = {"default_stone"}
})
