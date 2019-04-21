-- TallGrass [ID 6..10]
for i=1,5,1 do
    zeus.register_block("default:tallgrass_" .. i, {
        culls = false,
        solid = false,
        name = "Tall Grass",
        model = "default:cross",
        textures = {
            "default_tallgrass_" .. i,
        },
        lowdef_render = false,
        selection_box = {
            {1/16, 0, 1/16, 15/16, 0.30 + i * 0.1, 15/16}
        },
        toughness = {
            hand = 0
        }
    })
end

-- Flowers [ID 11..18]
local flowers = {
    "rose",
    "tulip",
    "viola",
    "geranium",
    "mushroom_red",
    "mushroom_brown",
    "dandelion_white",
    "dandelion_yellow"
}

for _,flower in ipairs(flowers) do

    local function tchelper(first, rest)
        return first:upper()..rest:lower()
    end

    local name = flower:gsub("_", " "):gsub("(%a)([%w_']*)", tchelper)

    zeus.register_block("default:flower_" .. flower, {
        culls = false,
        solid = false,
        name = name,
        model = "default:cross",
        textures = {
            "flowers_" .. flower
        },
        lowdef_render = false,
        selection_box = {
            {4/16, 0, 4/16, 12/16, 14/16, 12/16}
        },
        toughness = {
            hand = 0
        }
    })
end