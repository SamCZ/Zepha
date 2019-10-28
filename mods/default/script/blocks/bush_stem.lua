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
        zepha.add_entity("zeus:default:dropped_item", {x = pos.x + 0.5, y = pos.y + 0.5, z = pos.z + 0.5},
            {object = zepha.registered_blocks["zeus:default:bush_stem"].drop});
    end
})