zepha.register_block("zeus:default:leaves", {
    visible = true,
    culls = false,
    light_propagates = true,
    name = "Leaves",
    model = "base:leaf_like",
    textures = {
        "tint(0, zeus:default:leaves)",
        "tint(0, zeus:default:leaves_puff)"
    },
    lowdef_textures = {
        "tint(0, zeus:default:leaves_opaque)",
    },
    toughness = {
        hand = 1,
        axe = 0.2,
    },
    yields = "zeus:materials:stick"
})