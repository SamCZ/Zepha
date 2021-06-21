local identifier = "zeus:world:plains"

local wood = "zeus:default:wood"
local leaf = "zeus:default:leaves"
local none = "invalid"

local structures = {}

table.insert(structures, zepha.create_structure({
    noise = {
        module = "perlin",
        frequency = 0.002,
        octaves = 8
    },
    region_size = 4,
    origin = V{1, 1, 1},
    layout = {{
        { none, none, none },
        { none, wood, none },
        { none, none, none }
    }, {
        { none, leaf, none },
        { leaf, wood, leaf },
        { none, leaf, none }
    }, {
        { none, none, none },
        { none, leaf, none },
        { none, none, none }
    }}
}))

--for i = 1, 5 do
--    table.insert(structures, zepha.create_structure({
--        origin = V(),
--        probability = 0.1,
--        layout = {{{ "zeus:default:tall_grass_" .. tostring(i) }}}
--    }))
--end
--
--table.insert(structures, zepha.create_structure({
--    origin = V(),
--    probability = 0.025,
--    layout = {{{ "zeus:flowers:flower_geranium" }}}
--}))
--
--table.insert(structures, zepha.create_structure({
--    origin = V(),
--    probability = 0.025,
--    layout = {{{ "zeus:flowers:flower_white_dandelion" }}}
--}))

local noise = {
    heightmap = runfile(_PATH .. 'world_noise')
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