require("zeus:materials/items/stick")
require("zeus:materials/items/rock")
require("zeus:materials/items/flint")
require("zeus:materials/items/flint_heads")
require("zeus:materials/items/plant_fibre")
require("zeus:materials/items/plant_twine")

if zepha.server then
    zepha.bind("new_player", function(player)
        local hw = player:get_inventory():get_list("main");
        hw:add_stack({"zeus:materials:flint", 16})
        hw:add_stack({"zeus:materials:rock", 8})
        hw:add_stack({"zeus:materials:plant_twine", 4})
    end)
end