local identifier = "zeus:world:highlands"

local grass = zepha.create_structure({
    origin = V{1, 2, 3},
    schematic = {
        {{"zeus:default:tall_grass_4"}}
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

zepha.register_biome(identifier, {
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
    tags = { natural = 1, default = 1 },
    structures = { grass },
    biome_tint = "#c2fa61",
    noise = noise
})

return identifier