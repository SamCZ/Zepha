zepha.register_block(":cobblestone", {
    name = "Cobblestone",

    textures = { "zeus:default:cobblestone" },

    tool_props = {
        health = 25,
        multipliers = {
            crack = 1.8,
            smash = 2.0,
            _other = 0.25
        }
    },

    yields = "zeus:default:cobblestone"
})