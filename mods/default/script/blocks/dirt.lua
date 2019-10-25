zepha.register_block("zeus:default:dirt", {
    name = "Dirt",
    model = "base:block",
    textures = {"zeus:default:dirt"},
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    },
    drop = "materials:rock",
    on_break_client = function(pos)
        zepha.add_entity("zeus:default:dropped_item", {x = pos.x + 0.5, y = pos.y + 0.5, z = pos.z + 0.5},
            {object = zepha.registered_blocks["default:dirt"].drop});
    end
})