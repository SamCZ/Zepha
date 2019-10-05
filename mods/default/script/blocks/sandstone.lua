zepha.register_block("default:sandstone", {
    name = "Sandstone",
    model = "default:block",
    textures = {"zeus:default:sandstone"},
    toughness = {
        hand = 14,
        pick = 3
    },
    drop = "default:sand",
    on_break_client = function(pos)
        zepha.add_entity("default:dropped_item", {x = pos.x + 0.5, y = pos.y + 0.5, z = pos.z + 0.5},
            {object = zepha.registered_blocks["default:sandstone"].drop});
    end
})