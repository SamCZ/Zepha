zepha.register_block("zeus:default:leaves", {
    visible = true,
    culls = false,
    name = "Leaves",
    model = "base:leaf_like",
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
    drop = "zeus:materials:stick",
    on_break_client = function(pos)
        zepha.add_entity("zeus:default:dropped_item", {x = pos.x + 0.5, y = pos.y + 0.5, z = pos.z + 0.5},
            {object = zepha.registered_blocks["zeus:default:leaves"].drop});
    end
})