local identifier = "zeus:world:forest"

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
       probability = 0.025,
       layout = {{{ "zeus:default:tall_grass_" .. tostring(i) }}}
   }))
end
--
table.insert(structures, zepha.create_structure({
   origin = V(),
   probability = 0.05,
   layout = {{{ "zeus:flowers:flower_red_mushroom" }}}
}))

table.insert(structures, zepha.create_structure({
   origin = V(),
   probability = 0.05,
   layout = {{{ "zeus:flowers:flower_brown_mushroom" }}}
}))

table.insert(structures, zepha.create_structure({
    origin = V(1),
    probability = 0.1,
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
  probability = 0.01,
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

    heightmap = {
        module = "add",
        sources = {
            require('./world_noise'),
            {
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
}

zepha.register_biome(identifier, {
    environment = {
        temperature = 25/100,
        humidity = 70/100,
        roughness = 20/100,
    },
    blocks = {
        top = "zeus:default:grass",
        soil = "zeus:default:dirt",
        rock = "zeus:default:stone"
    },
    tags = { natural = 1, default = 1 },
    structures = structures,
    biome_tint = "#43bce0",
    noise = noise
})

return identifier