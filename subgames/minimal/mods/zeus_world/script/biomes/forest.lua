local identifier = "zeus:world:forest"

local noise = {
   heightmap = {
       module = "add",
       sources = {{
           -- Elevation
           module = "scale_bias",
           source = {
               module = "perlin",
               frequency = 0.002,
               octaves = 8
           },
           scale = 250,
           bias = -32
       }, {
           -- Features
           module = "scale_bias",
           source = {
               module = "perlin",
               frequency = 0.2,
               octaves = 3,
           },
           scale = 6,
           bias = 6
       }}
   }
}

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

local tree = zepha.create_structure({
   origin = V(2, 2, 2),
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
})

zepha.register_biome(identifier, {
    environment = {
        temperature = 15/100,
        humidity = 80/100,
        roughness = 20/100,
    },
    blocks = {
        top = "zeus:default:grass",
        soil = "zeus:default:dirt",
        rock = "zeus:default:stone"
    },
    tags = { natural = 1, default = 1 },
    structures = { tree },
    biome_tint = "#7beb26",
    noise = noise
})

return identifier