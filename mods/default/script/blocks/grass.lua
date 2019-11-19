zepha.register_block("zeus:default:grass", {
    name = "Grass",
    model = "base:block_foliage",
    textures = {
        "biometint(zeus:default:grass_top)",
        "zeus:default:dirt",
        "zeus:default:grass_side_under",
        "biometint(zeus:default:grass_side)",
        "biometint(zeus:default:grass_floating)",
    },
    lowdef_textures = {
        "zeus:default:grass_top",
        "zeus:default:dirt",
        "zeus:default:grass_side_ld"
    },
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    },
    drop = "zeus:default:dirt",
    on_break_client = function(pos)
        zepha.add_entity("zeus:default:dropped_item", {x = pos.x + 0.5, y = pos.y + 0.5, z = pos.z + 0.5},
            {object = zepha.registered_blocks["zeus:default:grass"].drop});
    end
})