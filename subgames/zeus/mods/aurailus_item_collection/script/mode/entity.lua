local get_yield = runfile("@aurailus:item_collection/get_yield")

if zepha.server then
    zepha.register_on("break", function(pos)
        local yields = get_yield(pos)
        if yields == nil then return end

        zepha.add_entity("@aurailus:item_collection:dropped_item", pos + 0.5, { item = yields });
    end)
end