require './items/stick'
require './items/rock'
require './items/flint'
require './items/flint_heads'
require './items/plant_fibre'
require './items/plant_twine'

if zepha.server then
    zepha.bind("new_player", function(player)
        local hw = player:get_inventory():get_list("main");
        hw:add_stack({ "zeus:materials:flint", 16 })
        hw:add_stack({ "zeus:materials:rock", 8 })
        hw:add_stack({ "zeus:materials:plant_twine", 4 })
    end)
end