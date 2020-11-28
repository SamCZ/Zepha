local identifier = "zeus:world:plains"

local woo = "zeus:default:wood"
local lea = "zeus:default:leaves"
local inv = "invalid"

local shrub_layer_0 = {
    { inv, inv, inv },
    { inv, woo, inv },
    { inv, inv, inv }
}

local shrub_layer_1 = {
    { inv, lea, inv },
    { lea, woo, lea },
    { inv, lea, inv }
}

local shrub_layer_2 = {
    { inv, inv, inv },
    { inv, lea, inv },
    { inv, inv, inv }
}

local shrub = zepha.create_structure({
    origin = V{1, 1, 1},
    probability = 0.01,
    schematic = {
        shrub_layer_0,
        shrub_layer_1,
        shrub_layer_2,
    }
})

local structures = { shrub }

for i = 1, 5 do
    table.insert(structures, zepha.create_structure({
        origin = V(),
        probability = 0.1,
        schematic = {{{ "zeus:default:tall_grass_" .. tostring(i) }}}
    }))
end

local noise = {
    heightmap = {
        module = "add",
        sources = {{
            module = "const",
            value = -12
        }, {
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
        }}
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