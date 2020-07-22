zepha.register_block("zeus:default:bush_stem", {
    name = "Bush Stem",

    culls = false,
    model = "base:cross_large",
    textures = { "zeus:default:bush_stem" },
    far_render = false,

    tool_props = {
        health = 5,
        multipliers = {
            snap = 1.5,
            grab = 1.0,
            _other = 0.5
        }
    },

    yields = "zeus:materials:stick"
})