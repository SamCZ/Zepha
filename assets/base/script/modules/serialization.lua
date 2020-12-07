zepha.serialize = function(data)
    local data_type = type(data)

    if data_type == 'table' then
        local values = {}
        for k, v in pairs(data) do
            table.insert(values, zepha.serialize(k) .. ' = ' .. zepha.serialize(v))
        end
        return '{ ' .. table.concat(values, ', ') .. ' }'
    end

    if data_type == 'number' then return tostring(data) end
    if data_type == 'string' then return '"' .. data:gsub('"', '\"') .. '"' end
    if data_type == 'boolean' then return data and 'true' or 'false' end

    if data_type ~= 'nil' then print('[ERR] Invalid serialization type: ' .. data_type) end
    return 'nil'
end

local function trim(str)
    return str:match('^%s*(.-)%s*$')
end

local function split(input, sep, op)
    if sep == nil then sep = "%s" end
    local t = {}
    for str in string.gmatch(input, '([^'..sep..']+)') do
        table.insert(t, op and op(str) or str) end
    return t
end

local function find_match(str, a, b, off)
    local pattern = '[' .. a .. b .. ']'
    if not off then off = 1 end
    local nest = 0
    local found = str:find(pattern, off)
    while found do
        if str:sub(found, found) == a then nest = nest + 1
        else nest = nest - 1 end
        off = found + 1
        if nest == 0 then return found
        elseif nest < 0 then return nil end
        found = str:find(pattern, off)
    end
end

zepha.deserialize = function(str)
    str = trim(str)

    if str:sub(1, 1) == '{' then
        local tbl = {}
        str = trim(str:sub(2, #str - 1))

        while #str > 0 do
            local sep, key, val

            if str:sub(1, 1) == ',' then
                str = trim(str:sub(2))
            end

            if str:sub(1, 1) == '{' then
                -- Handling a table key
                local e = find_match(str, '{', '}')
                local tbl_key = str:sub(1, e)
                key = zepha.deserialize(tbl_key)
                str = trim(str:sub(str:find('=', e + 1) + 1))
            else
                -- Handling a normal key
                local end_ind = str:find('=')
                key = zepha.deserialize(str:sub(1, end_ind - 1))
                str = trim(str:sub(end_ind + 1))
            end

            if str:sub(1, 1) == '{' then
                -- Handling a table value
                local e = find_match(str, '{', '}')
                local tbl_val = str:sub(1, e)
                val = zepha.deserialize(tbl_val)
                str = trim(str:sub(e + 1))
            else
                -- Handling a normal value
                local end_ind = str:find(',')
                val = zepha.deserialize(str:sub(1, (end_ind or #str + 1) - 1))
                str = trim(str:sub(end_ind or #str + 1))
            end

            tbl[key] = val
        end

        return tbl
    end

    if str:find('^"') then return str:sub(2, #str - 1) end
    if str:find('^true') then return true end
    if str:find('^false') then return false end
    if str:find('^nil') then return nil end

    return tonumber(str)
end