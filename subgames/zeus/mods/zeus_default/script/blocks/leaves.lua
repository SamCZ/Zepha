zepha.register_block(":leaves", {
    name = "Leaves",

    culls = false,
    model = "base:leaf_like",
    textures = {
        "tint(0, zeus:default:leaves)",
        "tint(0, zeus:default:leaves_puff)"
    },
    far_textures = {
        "tint(0, zeus:default:leaves_opaque)",
    },

    tool_props = {
        health = 5,
        multipliers = {
            snap = 2.0,
            cut = 2.0
        }
    },

    light_propagates = true,

    yields = "zeus:materials:stick"
})