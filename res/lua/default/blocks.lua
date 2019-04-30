local path = zeus.get_path("default");

--Grass, dirt, stone
dofile(path .. "blocks/ground.lua");
--Leaves, wood
dofile(path .. "blocks/tree.lua");
--Tallgrass, Flowers
dofile(path .. "blocks/foliage.lua");
--Sand, sandstone
dofile(path .. "blocks/desert.lua");

-- Grass Slab
zeus.register_block("default:grass_slab", {
    name = "Grass",
    model = "default:halfslab_foliage",
    culls = false,
    textures = {
        "default_grass_top",
        "default_dirt",
        "default_grass_side",
        "default_grass_side",
        "default_grass_side",
        "default_grass_side",
        "default_grass_float",
    },
    lowdef_textures = {
        "default_grass_top",
        "default_dirt",
        "default_grass_side"
    },
    selection_box = {
        {0, 0, 0, 1, 0.5, 1}
    },
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    },
})