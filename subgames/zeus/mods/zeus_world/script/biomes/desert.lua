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
--     heightmap = runfile(_PATH .. 'world_noise')
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
--     volume = {
--         module = "scale",
--         y_scale = 2,
--         source = {
--             module = "add",
--             scalar = -1800,
--             source = {
--                 module = "multiply",
--                 scalar = 3000,
--                 source = {
--                     module = "simplex",
--                     frequency = 0.0025,
--                     octaves = 6,
--                     lacunarity = 2
--                 }
--             }
--         }
--     }
    volume = require './world_noise'
}

zepha.register_biome(identifier, {
    environment = {
        temperature = 50/100,
        humidity = 0/100,
        roughness = 0/100
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
