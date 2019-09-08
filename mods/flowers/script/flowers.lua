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

local function tchelper(first, rest)
    return first:upper()..rest:lower()
end

for _,flower in ipairs(flowers) do
    local name = flower:gsub("_", " "):gsub("(%a)([%w_']*)", tchelper)

    zepha.register_block("flowers:flower_" .. flower, {
        culls = false,
        solid = false,
        name = name,
        model = "default:plantlike",
        textures = {
            "zeus:flowers:" .. flower
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