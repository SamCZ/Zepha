local identifier = "zeus:world:plains"

local wood = "zeus:default:wood"
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
--    layout = {{{ "zeus:flowers:flower_geranium" }}}
-- }))

for i = 1, 5 do
   table.insert(structures, zepha.create_structure({
       origin = V(),
       probability = 0.1,
       layout = {{{ "zeus:default:tall_grass_" .. tostring(i) }}}
   }))
end
--
table.insert(structures, zepha.create_structure({
   origin = V(),
   probability = 0.025,
   layout = {{{ "zeus:flowers:flower_geranium" }}}
}))

table.insert(structures, zepha.create_structure({
   origin = V(),
   probability = 0.025,
   layout = {{{ "zeus:flowers:flower_white_dandelion" }}}
}))

table.insert(structures, zepha.create_structure({
    origin = V(1),
    probability = 0.025,
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
  probability = 0.0005,
  layout = {
      trunk_layer_0,
      trunk_layer_0,
      trunk_layer_0,
      trunk_layer_0,
      trunk_layer_1,
      trunk_layer_1,
      trunk_layer_1,
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

local noise = {
--     heightmap = runfile(_PATH .. 'world_noise'),
    volume = {
        module = "scale",
        y_scale = 2,
        source = {
            module = "add",
            scalar = -2200,
            source = {
                module = "multiply",
                scalar = 3000,
                source = {
                    module = "simplex",
                    frequency = 0.0025,
                    octaves = 6,
                    lacunarity = 2
                }
            }
        }
    }
}

zepha.register_biome(identifier, {
    environment = {
        temperature = 15/100,
        humidity = 60/100,
        roughness = 20/100,
    },
    blocks = {
        top = "zeus:default:grass",
        soil = "zeus:default:dirt",
        rock = "zeus:default:stone"
    },
    tags = { natural = 1, default = 1 },
    structures = structures,
    biome_tint = "#aaed45",
    noise = noise
})

return identifier