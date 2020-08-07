local get_yield = runfile("@aurailus:item_collection/get_yield")

if zepha.server then
    zepha.bind("on_break", function(dim, pos)
        local yields = get_yield(dim, pos)
        if yields == nil then return end
        dim:add_entity(pos + 0.5, "@aurailus:item_collection:dropped_item", { item = yields });
    end)
end