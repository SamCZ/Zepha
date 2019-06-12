zeus.register_block("default:grass", {
    name = "Grass",
    model = "default:block_foliage",
    textures = {
        "default_grass_top",
        "default_dirt",
        "default_grass_side",
        "default_grass_side",
        "default_grass_side",
        "default_grass_side",
        "default_grass_float",
    },
    lowdef_textures = {
        "default_grass_top",
        "default_dirt",
        "default_grass_side"
    },
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    },
})

zeus.register_block('default:dirt', {
    name = "Dirt",
    model = "default:block",
    textures = {"default_dirt"},
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    },
})

zeus.register_block('default:stone', {
    name = "Stone",
    model = "default:block",
    textures = {"default_stone"},
    toughness = {
        hand = 14,
        pick = 3
    },
})