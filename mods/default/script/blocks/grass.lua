zeus.register_block("default:grass", {
    name = "Grass",
    model = "default:block_foliage",
    textures = {
        "zeus:default:grass_top",
        "zeus:default:dirt",
        "zeus:default:grass_side",
        "zeus:default:grass_side",
        "zeus:default:grass_side",
        "zeus:default:grass_side",
        "zeus:default:grass_floating",
    },
    lowdef_textures = {
        "zeus:default:grass_top",
        "zeus:default:dirt",
        "zeus:default:grass_side"
    },
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    }
})