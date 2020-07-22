zepha.register_blockmodel("@aurailus:crazyblocks:box", {
    mesh = "@aurailus:crazyblocks:box.b3d"
})

zepha.register_block("@aurailus:crazyblocks:box", {
    name = "Box",
    model = "base:block",
    textures = {"zeus:default:oak_log_side", "zeus:default:oak_planks"},
    on_interact_client = function() inventory.open_chest() end
})