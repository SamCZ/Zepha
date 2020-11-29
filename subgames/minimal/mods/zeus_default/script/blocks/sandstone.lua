zepha.register_block(":sandstone", {
    name = "Sandstone",

    model = "base:block",
    textures = {
        "zeus:default:sandstone_top",
        "zeus:default:sandstone_top",
        "zeus:default:sandstone"
    },

    tool_props = {
        health = 20,
        multipliers = {
            crack = 1.8,
            smash = 2.0,
            _other = 0.25
        }
    },

    yields = "zeus:default:sand"
})