-- Performs a shallow copy of a table.
function table.clone(tbl)
	local new = {}
	for k, v in pairs(tbl) do new[k] = v end
	return new
end

-- Merges two tables together
function table.merge(...)
    local new = {}
    for i, tbl in ipairs(table.pack(...)) do
        if type(tbl) == 'table' then
            for k, v in pairs(tbl) do new[k] = v end
        end
    end
    return new
end