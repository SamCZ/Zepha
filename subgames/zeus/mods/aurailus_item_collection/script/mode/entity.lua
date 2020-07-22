local get_yield = runfile("@aurailus:item_collection/get_yield")

if zepha.server then
    zepha.register_on("break", function(pos)
        local yields = get_yield(pos)
        if yields == nil then return end

        zepha.add_entity(pos + 0.5, "@aurailus:item_collection:dropped_item", { item = yields });
    end)
end