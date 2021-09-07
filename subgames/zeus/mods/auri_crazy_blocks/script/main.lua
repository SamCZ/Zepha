require './chest'

local function stacker_action(dim, pos)
    local v = V(0, 1, 0)
    while dim:get_block(pos + v) == "@auri:crazy_blocks:stacker" do v = v + V(0, 1, 0) end
    dim:set_block(pos + v, "@auri:crazy_blocks:stacker")
end

local function inventory_action()
    inventory.open_inventory()
end

zepha.register_block("@auri:crazy_blocks:stacker", {
    name = "Stacker",
    textures = {"zeus:default:oak_planks"},
    on_interact = stacker_action,
    on_interact_client = stacker_action
})

zepha.register_block("@auri:crazy_blocks:inventory", {
    name = "Open Inventory",
    textures = {"zeus:default:cobblestone"},
    on_interact_client = inventory_action
})

if zepha.server then
    zepha.bind("new_player", function(player)
        local hw = player:get_inventory():get_list("hot_wheel_1")
        hw:add_stack({"zeus:default:dirt", 32})
--         hw:add_stack({"@aurailus:crazyblocks:stacker", 1})
--         hw:add_stack({"@aurailus:crazyblocks:inventory", 1})
        hw:add_stack({"@auri:crazy_blocks:box", 5})
    end)
end