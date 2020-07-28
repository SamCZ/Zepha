runfile(_PATH .. "chest")

local function stacker_action(pos)
    local v = V(0, 1, 0)
    while zepha.get_block(pos + v) == "@aurailus:crazyblocks:stacker" do v = v + V(0, 1, 0) end
    zepha.set_block(pos + v, "@aurailus:crazyblocks:stacker")
end

local function inventory_action()
    inventory.open_inventory()
end

zepha.register_block("@aurailus:crazyblocks:stacker", {
    name = "Stacker",
    model = "base:block",
    textures = {"zeus:default:oak_planks"},
    on_interact = stacker_action,
    on_interact_client = stacker_action
})

zepha.register_block("@aurailus:crazyblocks:inventory", {
    name = "Open Inventory",
    model = "base:block",
    textures = {"zeus:default:cobblestone"},
    on_interact_client = inventory_action
})

if zepha.server then
    zepha.bind("new_player", function(player)
        player:get_inventory():get_list("hot_wheel_1"):add_stack({"zeus:default:dirt", 32})
        player:get_inventory():get_list("hot_wheel_1"):add_stack({"@aurailus:crazyblocks:stacker", 1})
        player:get_inventory():get_list("hot_wheel_1"):add_stack({"@aurailus:crazyblocks:inventory", 1})
        player:get_inventory():get_list("hot_wheel_1"):add_stack({"@aurailus:crazyblocks:box", 1})
    end)
end