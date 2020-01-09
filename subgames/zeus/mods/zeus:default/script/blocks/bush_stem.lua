zepha.register_block("zeus:default:bush_stem", {
    visible = true,
    culls = false,
    name = "Bush Stem",
    model = "base:cross_large",
    textures = {
        "zeus:default:bush_stem"
    },
    lowdef_render = false,
    toughness = {
        hand = 0.3,
        axe = 0.1
    },
    drop = "zeus:default:wood",
    on_break_client = function(pos)
        zepha.add_entity("zeus:default:dropped_item", vector.add(pos, v(0.5)),
            {item = zepha.registered_blocks["zeus:default:bush_stem"].drop});
    end
})