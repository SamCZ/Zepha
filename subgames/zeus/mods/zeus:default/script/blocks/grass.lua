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
    on_break = function(pos)
        zepha.add_entity("zeus:default:dropped_item", vector.add(pos, v(0.5)),
            {item = zepha.registered_blocks["zeus:default:grass"].drop});
    end
})

zepha.register_block("zeus:default:grass_slab", {
    name = "Grass Slab",
    model = "base:block_slab_foliage",
    textures = {
        "biometint(zeus:default:grass_top)",
        "zeus:default:dirt",
        "biometint(zeus:default:grass_top)",
        "biometint(zeus:default:grass_top)",
        "biometint(zeus:default:grass_top)",
        "biometint(zeus:default:grass_top)",
        "biometint(zeus:default:grass_floating)",
    },
    culls = false,
--     lowdef_textures = {
--         "zeus:default:grass_top",
--         "zeus:default:dirt",
--         "zeus:default:grass_side_ld"
--     },
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    },
    selection_box = {
        {0, 0, 0, 1, 0.5, 1}
    },
    collision_box = {
        {0, 0, 0, 1, 0.5, 1}
    },
    drop = "zeus:default:grass_slab",
    on_break_client = function(pos)
        zepha.add_entity("zeus:default:dropped_item", vector.add(pos, v(0.5)),
            {item = zepha.registered_blocks["zeus:default:grass_slab"].drop});
    end
})