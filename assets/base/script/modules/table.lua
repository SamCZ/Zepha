-- Performs a shallow copy of a table.
function table.clone(tbl)
    return {table.unpack(tbl)}
end