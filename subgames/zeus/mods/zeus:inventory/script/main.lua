print("Hello from inventory")

if zepha.server then return end -- Only run the following code on the client.

local inventory = zepha.player:get_inventory()

local shit_adding = false
local main = inventory:add_list("main", 44, 11)

zepha.register_keybind("zeus:inventory:add_shit_b", {
    description = "Add testing items to inventory",
    default = zepha.keys.b,
    on_press = function()
        shit_adding = 1
    end,
    on_release = function()
        shit_adding = 0
    end
})

zepha.register_keybind("zeus:inventory:add_shit_i", {
    description = "Add testing items to inventory",
    default = zepha.keys.n,
    on_press = function()
        shit_adding = 2
    end,
    on_release = function()
        shit_adding = 0
    end
})

zepha.delay(function()
    if shit_adding == 1 then
        main:add_stack({"zeus:default:cobblestone", 1})
        main:add_stack({"zeus:default:tallgrass_5", 1})
        main:add_stack({"zeus:default:leaves", 1})
        main:add_stack({"zeus:kinetic:axle_0", 1})
        main:add_stack({"zeus:default:wood", 1})
    elseif shit_adding == 2 then
        main:add_stack({"zeus:materials:plant_fibre", 1})
        main:add_stack({"zeus:materials:plant_twine", 1})
        main:add_stack({"zeus:materials:stick", 1})
        main:add_stack({"zeus:materials:flint", 1})
    end
    return true
end, 0)

zepha.register_keybind("zeus:inventory:open_inventory", {
    description = "Open Inventory",
    default = zepha.keys.e,
    on_press = function()
        if zepha.player.menu_state == "" then
            zepha.player:open_menu([[
                body[body]
                    background: #0003

                    rect[inventory]
                        position: 50% 50%
                        position_anchor: 50% 55%
                        size: 218px 160px

                        rect[inv_background]
                            position: 0px 45px
                            size: 218px 100px
                            padding: 20px 10px 8px 10px
                            background: asset(zeus:inventory:inventory)

                            inventory
                                source: current_player
                                list: main
                                position: 1px 1px
                                slot_spacing: 2px 2px
                            end
                        end

                        rect[craft_background]
                            size: 218px 72px
                            position: 0px -15px
                            padding: 20px 10px 8px 10px
                            background: asset(zeus:inventory:crafting)

                            inventory
                                source: current_player
                                list: craft
                                position: 111px 1px
                                slot_spacing: 2px 2px
                            end

                            inventory
                                source: current_player
                                list: craft_result
                                position: 163px 10px
                                slot_spacing: 2px 2px
                            end
                        end

                        rect[hot_wheel]
                            size: 214px 67px
                            position: 2px 160px
                            background: asset(zeus:inventory:inventory_wheel)

                            inventory
                                source: current_player
                                list: hot_wheel_1
                                position: 9px 1px
                                slot_spacing: 2px 2px
                            end
                            inventory
                                source: current_player
                                list: hot_wheel_2
                                position: 117px 1px
                                slot_spacing: 2px 2px
                            end
                            inventory
                                source: current_player
                                list: hot_wheel_3
                                position: 125px 25px
                                slot_spacing: 2px 2px
                            end
                            inventory
                                source: current_player
                                list: hot_wheel_4
                                position: 117px 50px
                                slot_spacing: 2px 2px
                            end
                            inventory
                                source: current_player
                                list: hot_wheel_5
                                position: 9px 50px
                                slot_spacing: 2px 2px
                            end
                            inventory
                                source: current_player
                                list: hot_wheel_6
                                position: 1px 25px
                                slot_spacing: 2px 2px
                            end
                        end
                    end
                end
            ]], {
                body = {
                    left = function()
                        zepha.player:close_menu()
                    end
                },
                inv_background = {
                    left = function() --[[ Prevent close menu from triggering. ]] end
                }
            })
        else zepha.player:close_menu() end
    end
})

-- Bind crafting

local craft_input = inventory:add_list("craft", 4, 2)
local craft_output = inventory:add_list("craft_result", 2, 2)

crafting.bind(craft_input, craft_output)

-- Make hotwheel

local invs = {
    inventory:add_list("hot_wheel_1", 5, 5),
    inventory:add_list("hot_wheel_2", 5, 5),
    inventory:add_list("hot_wheel_3", 5, 5),
    inventory:add_list("hot_wheel_4", 5, 5),
    inventory:add_list("hot_wheel_5", 5, 5),
    inventory:add_list("hot_wheel_6", 5, 5)
}

for i, inv in pairs(invs) do
    inv.allow_take = function() return 0 end
    inv.allow_put = function(slot, item)
        zepha.delay(function()
            -- This delay is necessary to avoid the engine overwriting it with nothing
            inv:set_stack(slot, item)
        end, 0)
        return 0
    end
end