local noise = {
    module = "add",
    sources = {
        { -- Elevation
            module = "scale_bias",
            source = {
                module = "perlin",
                seed = zepha.seed,
                frequency = 0.002,
                octaves = 8
            },
            scale = 250,
            bias = 32
        },
        { -- Features
            module = "scale_bias",
            source = {
                module = "perlin",
                seed = zepha.seed,
                frequency = 0.2,
                octaves = 3,
            },
            scale = 6,
            bias = 6
        }
    }
}

zepha.register_biome("zeus:default:plains", {
    environment = {
        temperature = 15/100,
        humidity = 80/100,
        roughness = 20/100,
    },
    blocks = {
        top = "zeus:default:tallgrass_1",
        soil = "zeus:default:grass",
        rock = "zeus:default:stone"
    },
    biome_tint = "#aaed45",
    noise = noise
})