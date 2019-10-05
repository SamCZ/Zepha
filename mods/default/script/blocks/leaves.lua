zepha.register_block("default:leaves", {
    visible = true,
    culls = false,
    name = "Leaves",
    model = "default:leaflike",
    textures = {
        "zeus:default:leaves",
        "zeus:default:leaves_puff"
    },
    lowdef_textures = {
        "zeus:default:leaves_opaque",
    },
    toughness = {
        hand = 1,
        axe = 0.2,
    },
    drop = "materials:stick",
    on_break_client = function(pos)
        zepha.add_entity("default:dropped_item", {x = pos.x + 0.5, y = pos.y + 0.5, z = pos.z + 0.5},
            {object = zepha.registered_blocks["default:leaves"].drop});
    end
})