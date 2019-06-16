zeus.register_block('default:leaves', {
    visible = true,
    culls = false,
    name = "Log",
    model = "default:leaflike",
    textures = {
        "default_leaves",
        "default_leaves_puff"
    },
    lowdef_textures = {
        "default_leaves_opaque",
    },
    toughness = {
        hand = 1,
        axe = 0.2,
    }
})

zeus.register_block('default:wood', {
    name = "Log",
    model = "default:block",
    textures = {
        "default_log_top",
        "default_log_top",
        "default_log_side"
    },
    toughness = {
        hand = 5,
        axe = 3,
    }
})