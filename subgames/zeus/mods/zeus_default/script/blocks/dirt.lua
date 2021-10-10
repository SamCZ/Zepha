zepha.register_block(":dirt", {
    name = "Dirt",

    textures = { "zeus:default:dirt" },

    tool_props = {
        health = 25,
        multipliers = {
            scoop = 2.0,
            smash = 0,
        }
    },

    yields = "zeus:default:dirt",

    on_interact = function(dim, pos) dim:set_block(pos, "zeus:default:grass") end,
    on_interact_client = function(dim, pos) dim:set_block(pos, "zeus:default:grass") end
})