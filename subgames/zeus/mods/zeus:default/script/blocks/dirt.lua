zepha.register_block("zeus:default:dirt", {
    name = "Dirt",
    model = "base:block",
    textures = {"zeus:default:dirt"},
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    },
    drop = "zeus:materials:rock",
    on_break_client = function(pos)
        zepha.add_entity("zeus:default:dropped_item", vector.add(pos, v(0.5)),
            {object = zepha.registered_blocks["zeus:default:dirt"].drop});
    end
})