local path = "/home/aurailus/C++/GlProject/res/lua/default/";

-- Temporarily generate Air in the lua file
-- TODO: Move this to C++
-- Air [ID 0]
zeus.register_block("_:air", {
    visible = false,
    culls = false,
    solid = false,
    name = "Air",
    model = "default:block",
    textures = {"_missing"},
})


--Grass, dirt, stone
dofile(path .. "blocks/ground.lua");
--Leaves, wood
dofile(path .. "blocks/tree.lua");
--Tallgrass, Flowers
dofile(path .. "blocks/foliage.lua");
--Sand, sandstone
dofile(path .. "blocks/desert.lua");