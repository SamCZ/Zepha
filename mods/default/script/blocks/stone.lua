zepha.register_block("zeus:default:stone", {
    name = "Stone",
    model = "base:block",
    textures = {"zeus:default:stone"},
    toughness = {
        hand = 14,
        pick = 3
    },
    drop = "default:cobblestone",
    on_break_client = function(pos)
        zepha.add_entity("zeus:default:dropped_item", {x = pos.x + 0.5, y = pos.y + 0.5, z = pos.z + 0.5},
            {object = zepha.registered_blocks["zeus:default:stone"].drop});
    end
})