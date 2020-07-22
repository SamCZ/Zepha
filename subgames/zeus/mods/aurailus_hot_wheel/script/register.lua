zepha.register_on("new_player", function(p)
    local inv = p:get_inventory()
    inv:add_list("hot_wheel_1", 5, 5)
    inv:add_list("hot_wheel_2", 5, 5)
    inv:add_list("hot_wheel_3", 5, 5)
    inv:add_list("hot_wheel_4", 5, 5)
    inv:add_list("hot_wheel_5", 5, 5)
    inv:add_list("hot_wheel_6", 5, 5)

--     -- Get hot wheel
--     local invs = {
--         inv:get_list("hot_wheel_1"),
--         inv:get_list("hot_wheel_2"),
--         inv:get_list("hot_wheel_3"),
--         inv:get_list("hot_wheel_4"),
--         inv:get_list("hot_wheel_5"),
--         inv:get_list("hot_wheel_6")
--     }
--
--     for _,inv in pairs(invs) do
--         inv.allow_take = function() return 0 end
--
--         inv.allow_put = function(slot, item)
--             zepha.delay(function()
--                 -- This delay is necessary to avoid the engine overwriting it with nothing
--                 inv:set_stack(slot, item)
--             end, 0)
--             return 0
--         end
--     end
end)