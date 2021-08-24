local get_yield = require("@auri:yield/get_yield")

if zepha.server then
    zepha.bind("on_break", function(pos, player)
        local yields = get_yield(pos)
        if yields == nil then return end

        player:get_inventory():get_list("main"):add_stack({yields, 1})
    end)
end