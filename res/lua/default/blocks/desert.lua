zeus.register_block('default:sand', {
    name = "Sand",
    model = "default:block",
    textures = {"default_sand"},
    toughness = {
        hand = 3,
        shovel = 1,
        pick = 2
    }
})

zeus.register_block('default:sandstone', {
    name = "Sandstone",
    model = "default:block",
    textures = {"default_sandstone"},
    toughness = {
        hand = 14,
        pick = 3
    }
})