zepha.register_block("zeus:default:wood", {
    name = "Log",
    model = "base:block",
    textures = {
        "zeus:default:oak_log_top",
        "zeus:default:oak_log_top",
        "zeus:default:oak_log_side"
    },
    toughness = {
        hand = 5,
        axe = 3,
    },
    drop = "zeus:default:wood",
    on_break_client = function(pos)
        zepha.add_entity("zeus:default:dropped_item", vector.add(pos, v(0.5)),
            {item = zepha.registered_blocks["zeus:default:wood"].drop});
    end
})