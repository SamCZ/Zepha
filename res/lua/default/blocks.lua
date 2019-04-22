local path = zeus.get_path("default");

--Grass, dirt, stone
dofile(path .. "blocks/ground.lua");
--Leaves, wood
dofile(path .. "blocks/tree.lua");
--Tallgrass, Flowers
dofile(path .. "blocks/foliage.lua");
--Sand, sandstone
dofile(path .. "blocks/desert.lua");