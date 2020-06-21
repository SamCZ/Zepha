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
    schematic = {
        shrub_layer_0,
        shrub_layer_1,
        shrub_layer_2,
    }
})

local noise = {
    volume = {
        module = "add",
        sources = {{
            module = "add",
            sources = {{
                -- Voronoi
                module = "scale_bias",
                source = {
                    module = "voronoi",
                    frequency = 0.2,
                    displacement = 5
                },
                scale = 8,
                bias = -32
            }, {
                -- Features
                module = "scale_bias",
                source = {
                    module = "perlin",
                    frequency = 0.6,
                    octaves = 3,
                },
                scale = 3
            }}
        }, {
            -- Variation
            module = "scale_bias",
            source = {
                module = "perlin",
                frequency = 0.05,
                octaves = 6
            },
            scale = 15
        }}
    }
}

zepha.register_biome("zeus:mapgen:highlands", {
    environment = {
        temperature = 0/100,
        humidity = 0/100,
        roughness = 50/100
    },
    blocks = {
        top = "zeus:default:grass",
        soil = "zeus:default:dirt",
        rock = "zeus:default:stone"
    },
    biome_tint = "#c2fa61",
    noise = noise,
    structures = {
        shrub
    }
})