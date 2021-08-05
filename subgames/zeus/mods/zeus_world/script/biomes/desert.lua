local identifier = "zeus:world:desert"

local structures = {}

table.insert(structures, zepha.create_structure({
    origin = V(),
    probability = 0.001,
    layout = {{{ "zeus:default:cactus" }}, {{ "zeus:default:cactus" }}}
}))

table.insert(structures, zepha.create_structure({
    origin = V(),
    probability = 0.001,
    layout = {{{ "zeus:default:cactus" }}, {{ "zeus:default:cactus" }}, {{ "zeus:default:cactus" }}}
}))

table.insert(structures, zepha.create_structure({
    origin = V(),
    probability = 0.001,
    layout = {{{ "zeus:default:cactus" }}, {{ "zeus:default:cactus" }}, {{ "zeus:default:cactus" }}, {{ "zeus:default:cactus" }}}
}))

local noise = {
--     heightmap = {
--         module = "add",
--         sources = {{
--             module = "min",
--             sources = {{
--                 module = "turbulence",
--                 power = 1,
--                 frequency = 0.3,
--                 source = {
--                     module = "add",
--                     sources = {{
--                         -- Elevation
--                         module = "scale_bias",
--                         source = {
--                             module = "spheres",
--                             frequency = 0.2
--                         },
--                         scale = 20
--                     }, {
--                         -- Features
--                         module = "scale_bias",
--                         source = {
--                             module = "perlin",
--                             frequency = 0.1,
--                             octaves = 6,
--                         },
--                         scale = 8
--                     }}
--                 }
--             }, {
--                 module = "scale_bias",
--                 scale = 1,
--                 bias = 1000,
--                 source = {
--                     module = "perlin",
--                     frequency = 0.2
--                 }
--             }}
--         }, {
--             module = "const",
--             value = -40
--         }}
--     }
    volume = {
        module = "scale_bias",
        scale = 3000,
        bias = -3500,
        source = {
            module = "scale_point",
            y_scale = 2,
            source = {
                module = "perlin",
                frequency = 0.1
            }
        }
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
    tags = { natural = 1, default = 1 },
    biome_tint = "#e6fa61",
    noise = noise,
    structures = structures
})

return identifier;
