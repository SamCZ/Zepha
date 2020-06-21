zepha.register_block("zeus:default:stone", {
    name = "Stone",
    model = "base:block",
    textures = {"zeus:default:stone"},
    toughness = {
        hand = 14,
        pick = 3
    },
    yields = function()
        if math.random() >= 0.5 then return "zeus:default:cobblestone"
        else return "zeus:materials:rock" end
    end
})