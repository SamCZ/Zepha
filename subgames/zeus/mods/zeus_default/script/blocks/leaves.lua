zepha.register_block(":leaves", {
    name = "Leaves",

    culls = false,
    model = "zepha:base:leaf_like",
    textures = {
        "zeus:default:leaves",
        "zeus:default:leaves_puff"
    },
    lowdef_textures = {
        "zeus:default:leaves_opaque",
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