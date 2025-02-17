local identifier = "zeus:world:plains"

local wood = "zeus:default:bush_stem"
local leaf = "zeus:default:leaves"
local none = "invalid"

local structures = {}
--
-- table.insert(structures, zepha.create_structure({
-- --     noise = {
-- --         module = "perlin",
-- --         frequency = 0.002,
-- --         octaves = 8
-- --     },
-- --     region_size = 4,
--     probability = 0.1,
-- --     origin = V{1, 1, 1},
--     origin = V(),
--    layout = {{{ "zeus:vegetation:geranium" }}}
-- }))

table.insert(structures, zepha.create_structure({
    origin = V(1),
    probability = 0.00125,
    layout = {{
        { none, none, none },
        { none, wood, none },
        { none, none, none }
    }, {
        { none, leaf, none },
        { leaf, wood, leaf },
        { none, leaf, none }
    }, {
        { none, none, none },
        { none, leaf, none },
        { none, none, none }
    }}
}))

local woo = "zeus:default:wood"
local lea = "zeus:default:leaves"
local inv = "invalid"

local trunk_layer_0 = {
  { inv, inv, inv, inv, inv },
  { inv, woo, woo, woo, inv },
  { inv, woo, woo, woo, inv },
  { inv, woo, woo, woo, inv },
  { inv, inv, inv, inv, inv }
}

local trunk_layer_1 = {
  { inv, inv, inv, inv, inv },
  { inv, inv, woo, inv, inv },
  { inv, woo, woo, woo, inv },
  { inv, inv, woo, inv, inv },
  { inv, inv, inv, inv, inv }
}

local trunk_layer_2 = {
  { inv, inv, inv, inv, inv },
  { inv, inv, inv, inv, inv },
  { inv, inv, woo, inv, inv },
  { inv, inv, inv, inv, inv },
  { inv, inv, inv, inv, inv }
}

local leaf_layer_1 = {
  { inv, lea, lea, lea, inv },
  { lea, lea, lea, lea, lea },
  { lea, lea, woo, lea, lea },
  { lea, lea, lea, lea, lea },
  { inv, lea, lea, lea, inv }
}

local leaf_layer_2 = {
  { inv, inv, inv, inv, inv },
  { inv, lea, lea, lea, inv },
  { inv, lea, woo, lea, inv },
  { inv, lea, lea, lea, inv },
  { inv, inv, inv, inv, inv }
}

local leaf_layer_3 = {
  { inv, inv, inv, inv, inv },
  { inv, lea, lea, inv, inv },
  { inv, lea, lea, lea, inv },
  { inv, inv, lea, lea, inv },
  { inv, inv, inv, inv, inv }
}

table.insert(structures, zepha.create_structure({
  origin = V(2, 2, 2),
  probability = 0.00025,
  layout = {
--       trunk_layer_0,
--       trunk_layer_0,
--       trunk_layer_0,
--       trunk_layer_0,
--       trunk_layer_1,
--       trunk_layer_1,
--       trunk_layer_1,
      trunk_layer_2,
      trunk_layer_2,
      trunk_layer_2,
      trunk_layer_2,
      trunk_layer_2,
      trunk_layer_2,
      trunk_layer_2,
      trunk_layer_2,
      trunk_layer_2,
      trunk_layer_2,
      trunk_layer_2,
      leaf_layer_2,
      leaf_layer_1,
      leaf_layer_1,
      leaf_layer_1,
      leaf_layer_1,
      leaf_layer_2,
      leaf_layer_3
  }
}))

table.insert(structures, zepha.create_structure({
   origin = V(),
   probability = 0.1,
   layout = {{{ "zeus:vegetation:purple" }}}
}))

for i = 1, 5 do
   table.insert(structures, zepha.create_structure({
       origin = V(),
       probability = 0.05,
       layout = {{{ "zeus:vegetation:tall_grass_" .. tostring(i) }}}
   }))
end

table.insert(structures, zepha.create_structure({
   origin = V(),
   probability = 0.55,
   layout = {{{ "zeus:vegetation:clover" }}}
}))

table.insert(structures, zepha.create_structure({
   origin = V(),
   probability = 0.05,
   layout = {{{ "zeus:vegetation:dandelion_white" }}}
}))

local noise = {
    heightmap = {
        module = "add",
        sources = {
            {
                module = "multiply",
                scalar = 100,
                source = {
                    module = "simplex",
                    frequency = 0.0002,
                    octaves = 5,
                    lacunarity = 3,
                    persistence = 0.45
                }
            }, {
                module = "max",
                smoothness = 50,
                scalar = 0,
                source = {
                    module = "add",
                    scalar = -50,
                    source = {
                        module = "multiply",
                        scalar = 400,
                        source = {
                            module = "simplex",
                            frequency = 0.00025,
                            lacunarity = 2.5,
                            octaves = 8,
                            persistence = 0.55
                        }
                    }
                }
            }
        }
    }
--     volume = require './world_noise'
--     volume = {
--         module = "scale",
--         y_scale = 2,
--         source = {
--             module = "add",
--             scalar = -1800,
--             source = {
--                 module = "multiply",
--                 scalar = 3000,
--                 source = {
--                     module = "simplex",
--                     frequency = 0.0025,
--                     octaves = 6,
--                     lacunarity = 2
--                 }
--             }
--         }
--     }
}

zepha.register_biome(identifier, {
    environment = {
        temperature = 20/100,
        humidity = 60/100,
        roughness = 40/100,
    },
    blocks = {
        top = "zeus:default:grass",
        soil = "zeus:default:dirt",
        rock = "zeus:default:stone"
    },
    tags = { natural = 1, default = 1 },
    structures = structures,
    biome_tint = "#46cfc0",
--     biome_tint = "#aaed45",
--     biome_tint = "#80d12e",
--     biome_tint = "#54beff",
--     biome_tint = "#1f83db",
    noise = noise
})

return identifier