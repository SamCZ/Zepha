print("Hello from default")

runfile(_PATH .. "blocks/_index")
runfile(_PATH .. "entity/_index")
runfile(_PATH .. "biomes/_index")

local blockTypes = {
    "zeus:default:grass_slab",
    "zeus:kinetic:axle_0",
    "zeus:default:stone",
    "zeus:default:wood",
    "zeus:default:cobblestone",
    "zeus:default:dirt",
    "zeus:default:grass",
    "zeus:default:leaves",
    "zeus:default:sand",
    "zeus:default:sandstone",
    "zeus:default:tallgrass_5"
}

for i, v in ipairs(blockTypes) do
    zepha.register_keybind("block_" .. i, {
        description = "Select Block " .. i,
        default = zepha.keys[tostring(i)],
        on_press = function()
            print("Setting block to " .. i)
            zepha.player:set_selected_block(v)
        end
    });
end

zepha.register_keybind("open_inventory", {
    description = "Open Inventory",
    default = zepha.keys.e,
    on_press = function()
        if zepha.player.menu_state == "" then
            zepha.player:open_menu([[
            body
                background: #0003

                rect[inv]
                    position: 50% 50%
                    position_anchor: 50% 50%
                    size: 236px 118px
                    padding: 20px 10px 8px 10px
                    background: asset(inventory)

                    inventory[list_main]
                        source: "local_player"
                        list: "player"
                        position: 1px 1px
                        slot_spacing: 2px 2px
                    end

--                     text[henlo_dog]
--                         content: "There's something I could never say\nIt's too complicated and you don't need to know"
--                     end

--                     rect[test]
--                         position: -16px -16px
--                         position_anchor: 50% 50%
--                         size: 16px 16px
--                         background: #fff
--                     end
                end
            end
            ]])
        else zepha.player:close_menu() end
    end
})

local chat_down = false
zepha.register_keybind("open_chat", {
    description = "Open Chat",
    default = zepha.keys.t,
    on_press = function()
        print "Opened chat!"
    end
})