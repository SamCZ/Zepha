zeus.register_block("default:leaves", {
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
    }
})