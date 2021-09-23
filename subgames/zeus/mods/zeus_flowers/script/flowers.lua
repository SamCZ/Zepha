local flowers = {
    "rose",
    "tulip",
    "viola",
    "geranium",
    "red_mushroom",
    "brown_mushroom",
    "white_dandelion",
    "yellow_dandelion"
}

local titlecase = function(first, rest) return first:upper()..rest:lower() end

for _,flower in pairs(flowers) do
    local name = flower:gsub("_", " "):gsub("(%a)([%w_']*)", titlecase)

    zepha.register_block("zeus:flowers:flower_" .. flower, {
        culls = false,
        solid = false,
        name = name,
        model = "zepha:base:cross_plant",
        textures = {
            "zeus:flowers:" .. flower
        },
        light_propagates = true,
        lowdef_render = false,
        selection_box = {
            { 4/16, 0, 4/16, 12/16, 14/16, 12/16 }
        },
        toughness = {
            hand = 0
        }
    })
end

require './models/hash'

zepha.register_block("zeus:flowers:clover", {
    name = "Clover",
    culls = false,
    solid = false,
    model = "zeus:flowers:hash",
    textures = {
        "tint(0, crop(0, 0, 16, 16, zeus:flowers:clover))",
--         "tint(0, crop(16, 0, 16, 16, zeus:flowers:clover))",
--         "tint(0, crop(32, 0, 16, 16, zeus:flowers:clover))"
    },
    light_propagates = true,
    lowdef_render = false,
    selection_box = {
        {1/16, 0, 1/16, 15/16, 4/16, 15/16}
    },
    toughness = {
        hand = 0
    }
})