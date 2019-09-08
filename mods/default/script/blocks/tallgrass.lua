for i=1,5,1 do
    zepha.register_block("default:tallgrass_" .. i, {
        culls = false,
        solid = false,
        name = "Tall Grass",
        model = "default:plantlike",
        textures = {
            "zeus:default:tallgrass_" .. i,
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