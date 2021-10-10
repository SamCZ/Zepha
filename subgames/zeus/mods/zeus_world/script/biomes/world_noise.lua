return {
    module = "multiply",
    scalar = 2500,
    source = {
        module = "add",
        sources = {{
            module = "multiply",
            sources = {{
                module = "simplex",
                octaves = 4,
                lacunarity = 2.65,
                frequency = 0.0025
            }, {
                module = "add",
                scalar = 0.75,
                source = {
                    module = "multiply",
                    scalar = 0.75,
                    source = {
                        module = "simplex",
                        frequency = 0.001
                    }
                }
            }}
        }, {
            module = "multiply",
            scalar = 0.0075,
            source = {
                module = "position_output",
                y_factor = -1
            }
        }}
    }
};