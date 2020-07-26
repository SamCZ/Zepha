zepha.register_block(":stone", {
    name = "Stone",

    model = "base:block",
    textures = {"zeus:default:stone"},

    tool_props = {
        health = 35,
        multipliers = {
            crack = 1.8,
            smash = 1.0,
            _other = 0
        }
    },

    yields = function()
        if math.random() >= 0.5 then return "zeus:default:cobblestone"
        else return "zeus:materials:rock" end
    end
})