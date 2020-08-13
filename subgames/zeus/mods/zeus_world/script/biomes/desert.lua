local identifier = "zeus:world:desert"

local noise = {
    heightmap = {
        module = "add",
        sources = {{
                module = "const",
                value = -80
            }, {
            -- Elevation
            module = "scale_bias",
            source = {
                module = "perlin",
                frequency = 0.02,
                octaves = 8
            },
            scale = 20,
            bias = 32
        }, {
            -- Features
            module = "scale_bias",
            source = {
                module = "perlin",
                frequency = 0.8,
                octaves = 3,
            },
            scale = 5,
            bias = 6
        }}
    }
}

zepha.register_biome(identifier, {
   environment = {
       temperature = 40/100,
       humidity = 20/100,
       roughness = 10/100
   },
   blocks = {
       top = "zeus:default:sand",
       soil = "zeus:default:sand",
       rock = "zeus:default:sandstone"
   },
   biome_tint = "#e6fa61",
   noise = noise
})

return identifier;