print("Hello from inventory")

zepha.register_keybind("add_shit", {
    description = "Add testing items to inventory",
    default = zepha.keys.p,
    on_press = function()
        zepha.player:get_inventory():get_list("main"):add_stack({"zeus:materials:flint_pick_head", 1})
        zepha.player:get_inventory():get_list("main"):add_stack({"zeus:materials:flint_axe_head", 1})
        zepha.player:get_inventory():get_list("main"):add_stack({"zeus:materials:flint_shovel_head", 1})
        zepha.player:get_inventory():get_list("main"):add_stack({"zeus:materials:flint_blade_head", 1})
        print(zepha.player:get_inventory():get_list("main"):get_stack(3).count)
    end
})

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