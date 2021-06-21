local identifier = "zeus:world:forest"

local noise = {
   heightmap = {
       module = "add",
       sources = {
           runfile(_PATH .. 'world_noise'), {
           -- Features
           module = "scale_bias",
           source = {
               module = "perlin",
               frequency = .5,
               octaves = 3,
           },
           scale = 3,
           bias = 0
       } }
   }
}

--local woo = "zeus:default:wood"
--local lea = "zeus:default:leaves"
--local inv = "invalid"
--
--local trunk_layer_0 = {
--   { inv, inv, inv, inv, inv },
--   { inv, woo, woo, woo, inv },
--   { inv, woo, woo, woo, inv },
--   { inv, woo, woo, woo, inv },
--   { inv, inv, inv, inv, inv }
--}
--
--local trunk_layer_1 = {
--   { inv, inv, inv, inv, inv },
--   { inv, inv, woo, inv, inv },
--   { inv, woo, woo, woo, inv },
--   { inv, inv, woo, inv, inv },
--   { inv, inv, inv, inv, inv }
--}
--
--local trunk_layer_2 = {
--   { inv, inv, inv, inv, inv },
--   { inv, inv, inv, inv, inv },
--   { inv, inv, woo, inv, inv },
--   { inv, inv, inv, inv, inv },
--   { inv, inv, inv, inv, inv }
--}
--
--local leaf_layer_1 = {
--   { inv, lea, lea, lea, inv },
--   { lea, lea, lea, lea, lea },
--   { lea, lea, woo, lea, lea },
--   { lea, lea, lea, lea, lea },
--   { inv, lea, lea, lea, inv }
--}
--
--local leaf_layer_2 = {
--   { inv, inv, inv, inv, inv },
--   { inv, lea, lea, lea, inv },
--   { inv, lea, woo, lea, inv },
--   { inv, lea, lea, lea, inv },
--   { inv, inv, inv, inv, inv }
--}
--
--local leaf_layer_3 = {
--   { inv, inv, inv, inv, inv },
--   { inv, lea, lea, inv, inv },
--   { inv, lea, lea, lea, inv },
--   { inv, inv, lea, lea, inv },
--   { inv, inv, inv, inv, inv }
--}
--
--local tree = zepha.create_structure({
--   origin = V(2, 2, 2),
--   probability = 0.01,
--   schematic = {
--       trunk_layer_0,
--       trunk_layer_0,
--       trunk_layer_0,
--       trunk_layer_0,
--       trunk_layer_1,
--       trunk_layer_1,
--       trunk_layer_1,
--       trunk_layer_2,
--       trunk_layer_2,
--       trunk_layer_2,
--       trunk_layer_2,
--       trunk_layer_2,
--       trunk_layer_2,
--       trunk_layer_2,
--       trunk_layer_2,
--       trunk_layer_2,
--       trunk_layer_2,
--       trunk_layer_2,
--       leaf_layer_2,
--       leaf_layer_1,
--       leaf_layer_1,
--       leaf_layer_1,
--       leaf_layer_1,
--       leaf_layer_2,
--       leaf_layer_3
--   }
--})
--
--local woo = "zeus:default:wood"
--local lea = "zeus:default:leaves"
--local inv = "invalid"
--
--local shrub_layer_0 = {
--    { inv, inv, inv },
--    { inv, woo, inv },
--    { inv, inv, inv }
--}
--
--local shrub_layer_1 = {
--    { inv, lea, inv },
--    { lea, woo, lea },
--    { inv, lea, inv }
--}
--
--local shrub_layer_2 = {
--    { inv, inv, inv },
--    { inv, lea, inv },
--    { inv, inv, inv }
--}
--
--local shrub = zepha.create_structure({
--    origin = V{1, 1, 1},
--    probability = 0.005,
--    schematic = {
--        shrub_layer_0,
--        shrub_layer_1,
--        shrub_layer_2,
--    }
--})
--
--local structures = { tree, shrub }
--
--for i = 1, 5 do
--    table.insert(structures, zepha.create_structure({
--        origin = V(),
--        probability = 0.01,
--        schematic = {{{ "zeus:default:tall_grass_" .. tostring(i) }}}
--    }))
--end
--
--table.insert(structures, zepha.create_structure({
--    origin = V(),
--    probability = 0.0025,
--    schematic = {{{ "zeus:flowers:flower_red_mushroom" }}}
--}))
--
--table.insert(structures, zepha.create_structure({
--    origin = V(),
--    probability = 0.0025,
--    schematic = {{{ "zeus:flowers:flower_brown_mushroom" }}}
--}))

local structures = {}

zepha.register_biome(identifier, {
    environment = {
        temperature = 15/100,
        humidity = 80/100,
        roughness = 20/100,
    },
    blocks = {
        top = "zeus:default:podzol",
        soil = "zeus:default:dirt",
        rock = "zeus:default:stone"
    },
    tags = { natural = 1, default = 1 },
    structures = structures,
    biome_tint = "#7beb26",
    noise = noise
})

return identifier