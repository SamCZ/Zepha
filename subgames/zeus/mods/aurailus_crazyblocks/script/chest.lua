zepha.register_blockmodel("@aurailus:crazyblocks:box", {
    mesh = "@aurailus:crazyblocks:box.b3d"
})

zepha.register_block("@aurailus:crazyblocks:box", {
    name = "Box",
    model = "base:block",
    textures = {"zeus:default:oak_log_side", "zeus:default:oak_planks"},
    on_interact = function(dim, pos, player)
        local in_desert = player:get_dimension().identifier == 'zeus:world:endless_desert'
        player:set_dimension(in_desert and "zeus:world:default" or "zeus:world:endless_desert")
--         inventory.open_chest()
    end
})