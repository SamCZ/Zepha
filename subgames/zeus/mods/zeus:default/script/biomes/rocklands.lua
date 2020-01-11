local noise = {
    module = "add",
    sources = {
        { -- Voronoi
            module = "scale_bias",
            source = {
                module = "voronoi",
                frequency = 0.2,
                displacement = 5
            },
            scale = 8,
            bias = -32
        },
        { -- Features
            module = "scale_bias",
            source = {
                module = "perlin",
                frequency = 0.8,
                octaves = 6,
            },
            scale = 6,
            bias = 0
        }
    }
}

zepha.register_biome("zeus:default:rocklands", {
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
    biome_tint = "#e6fa61",
    noise = noise
})