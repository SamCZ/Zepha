return function(dim, pos)
    local def = zepha.registered_blocks[dim:get_block(pos)]
    if def == nil then return nil end
    local yields = def.yields

    if type(yields) == "function" then yields = yields(pos) end
    if yields == nil or type(yields) ~= "string" then return nil end

    return yields
end