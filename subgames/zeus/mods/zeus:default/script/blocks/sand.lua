zepha.register_block("zeus:default:sand", {
    name = "Sand",
    model = "base:block",
    textures = {"zeus:default:sand"},
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    },
    drop = "zeus:default:sand",
    on_break_client = function(pos)
        zepha.add_entity("zeus:default:dropped_item", vector.add(pos, v(0.5)),
            {item = zepha.registered_blocks["zeus:default:sand"].drop});
    end
})