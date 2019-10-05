zepha.register_block("default:stone", {
    name = "Stone",
    model = "default:block",
    textures = {"zeus:default:stone"},
    toughness = {
        hand = 14,
        pick = 3
    },
    drop = "default:stone",
    on_break_client = function(pos)
        zepha.add_entity("default:dropped_item", {x = pos.x + 0.5, y = pos.y + 0.5, z = pos.z + 0.5},
            {object = zepha.registered_blocks["default:stone"].drop});
    end
})