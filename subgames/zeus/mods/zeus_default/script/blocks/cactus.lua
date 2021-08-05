zepha.register_block(":cactus", {
    name = "Cactus",

    culls = false,
    textures = {
        "tint(0, zeus:default:leaves_opaque)"
    },

    tool_props = {
        health = 5,
        multipliers = {
            snap = 2.0,
            cut = 2.0
        }
    },

    yields = "zeus:default:cactus"
})