zepha.register_block(":dirt", {
    name = "Dirt",

    model = "base:block",
    textures = { "zeus:default:dirt" },

    tool_props = {
        health = 25,
        multipliers = {
            scoop = 2.0,
            smash = 0,
        }
    },

    yields = "zeus:default:dirt",
})