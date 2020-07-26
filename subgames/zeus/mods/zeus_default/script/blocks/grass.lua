zepha.register_block(":grass", {
    name = "Grass",

    model = "base:block_foliage",
    textures = {
        "tint(0, zeus:default:grass_top)",
        "zeus:default:dirt",
        "tint(0, zeus:default:grass_side_under, zeus:default:grass_side_under_mask)",
        "tint(0, zeus:default:grass_floating)",
    },
    far_textures = {
        "tint(0, zeus:default:grass_top)",
        "zeus:default:dirt",
        "tint(0, zeus:default:grass_side_ld, zeus:default:grass_side_ld_mask)"
    },

    tool_props = {
        health = 25,
        multipliers = {
            scoop = 2.0,
            smash = 0,
        }
    },

    yields = function()
        if math.random() >= 0.5 then return "zeus:default:dirt"
        else return "zeus:materials:plant_fibre" end
    end
})

-- zepha.register_block("zeus:default:grass_slab", {
--     name = "Grass Slab",
--     model = "base:block_slab_foliage",
--     textures = {
--         "tint(0, zeus:default:grass_top)",
--         "zeus:default:dirt",
--         "tint(0, zeus:default:grass_top)",
--         "tint(0, zeus:default:grass_top)",
--         "tint(0, zeus:default:grass_top)",
--         "tint(0, zeus:default:grass_top)",
--         "tint(0, zeus:default:grass_floating)",
--     },
--     culls = false,
--     toughness = {
--         hand = 3,
--         shovel = 1,
--         pick = 2
--     },
--     selection_box = {
--         {0, 0, 0, 1, 0.5, 1}
--     },
--     collision_box = {
--         {0, 0, 0, 1, 0.5, 1}
--     },
--     yields = "zeus:default:grass_slab"
-- })