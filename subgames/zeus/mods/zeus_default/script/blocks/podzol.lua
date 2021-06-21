zepha.register_block(":podzol", {
    name = "Podzol",

    model = "base:block",
    textures = {
        "zeus:default:podzol",
        "zeus:default:dirt",
        "zeus:default:podzol_side"
    },

    tool_props = {
        health = 25,
        multipliers = {
            scoop = 2.0,
            smash = 0,
        }
    },

    yields = function()
        if math.random() >= 0.5 then return "zeus:default:dirt"
        else return "zeus:materials:stick" end
    end
})