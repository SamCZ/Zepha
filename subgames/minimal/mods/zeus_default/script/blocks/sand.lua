zepha.register_block(":sand", {
    name = "Sand",

    model = "base:block",
    textures = { "zeus:default:sand" },

    tool_props = {
        health = 8,
        multipliers = {
            scoop = 2.0,
            smash = 0
        }
    },

    yields = "zeus:default:sand"
})