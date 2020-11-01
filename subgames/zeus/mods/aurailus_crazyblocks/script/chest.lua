zepha.register_blockmodel("@aurailus:crazyblocks:box", {
    mesh = "@aurailus:crazyblocks:box.b3d"
})

zepha.register_block("@aurailus:crazyblocks:box", {
    name = "Box",
    model = "base:block",
    textures = {"zeus:default:oak_log_side", "zeus:default:oak_planks"},
    on_interact = function(dim, pos, player)
        print("Interacted with pandora's box")
        player:set_dimension("zeus:world:endless_desert")
--         inventory.open_chest()
    end
})