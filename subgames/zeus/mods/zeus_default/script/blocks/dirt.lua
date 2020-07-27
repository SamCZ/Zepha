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

    on_interact = function(pos) zepha.set_block(pos, "zeus:default:grass") end,
    on_interact_client = function(pos) zepha.set_block(pos, "zeus:default:grass") end
})