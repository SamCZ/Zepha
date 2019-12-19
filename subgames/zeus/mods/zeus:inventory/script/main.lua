print("Hello from inventory")

local shit_adding = false
zepha.register_keybind("add_shit", {
    description = "Add testing items to inventory",
    default = zepha.keys.p,
    on_press = function()
        shit_adding = true
    end,
    on_release = function()
        shit_adding = false
    end
})

zepha.delay(function()
    if shit_adding then
        local list = zepha.player:get_inventory():get_list("main")

        list:add_stack({"zeus:default:cobblestone", 1})
        list:add_stack({"zeus:default:tallgrass_5", 1})
        list:add_stack({"zeus:default:leaves", 1})
        list:add_stack({"zeus:kinetic:axle_0", 1})
        list:add_stack({"zeus:default:wood", 1})

--         list:add_stack({"zeus:materials:flint_pick_head", 1})
--         list:add_stack({"zeus:materials:flint_axe_head", 1})
--         list:add_stack({"zeus:materials:flint_shovel_head", 1})
--         list:add_stack({"zeus:materials:flint_blade_head", 1})

--         list:add_stack({"zeus:materials:plant_fibre", 1})
--         list:add_stack({"zeus:materials:plant_twine", 1})
--         list:add_stack({"zeus:materials:stick", 1})
--         list:add_stack({"zeus:materials:flint", 1})
    end
    return true
end, 0)

zepha.register_keybind("open_inventory", {
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
                            position: 0px 60px
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