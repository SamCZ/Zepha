return {
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
};