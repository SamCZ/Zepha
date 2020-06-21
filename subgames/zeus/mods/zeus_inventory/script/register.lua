zepha.register_on("new_player", function(p)
    -- Create the main inventory list
    local inv = p:get_inventory()
    local main = inv:add_list("main", 44, 11)

    -- Bind crafting
    local craft_input = inv:add_list("craft", 4, 2)
    local craft_output = inv:add_list("craft_result", 2, 2)

    crafting.bind(craft_input, craft_output)

    -- Get hot wheel
    local invs = {
        inv:get_list("hot_wheel_1"),
        inv:get_list("hot_wheel_2"),
        inv:get_list("hot_wheel_3"),
        inv:get_list("hot_wheel_4"),
        inv:get_list("hot_wheel_5"),
        inv:get_list("hot_wheel_6")
    }

    for name,_ in pairs(zepha.registered_blocks) do
        if name:sub(1, #"zeus:default:light") == "zeus:default:light" then
            main:add_stack({name, 1})
        end
    end

    for _,inv in pairs(invs) do
        inv.allow_take = function() return 0 end

        inv.allow_put = function(slot, item)
            zepha.delay(function()
                -- This delay is necessary to avoid the engine overwriting it with nothing
                inv:set_stack(slot, item)
            end, 0)
            return 0
        end
    end
end)