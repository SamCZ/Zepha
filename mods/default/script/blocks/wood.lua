zepha.register_block("default:wood", {
    name = "Log",
    model = "default:block",
    textures = {
        "zeus:default:oak_log_top",
        "zeus:default:oak_log_top",
        "zeus:default:oak_log_side"
    },
    toughness = {
        hand = 5,
        axe = 3,
    },
    drop = "default:wood",
    on_break_client = function(pos)
        zepha.add_entity("default:dropped_item", {x = pos.x + 0.5, y = pos.y + 0.5, z = pos.z + 0.5},
            {object = zepha.registered_blocks["default:wood"].drop});
    end
})