--
-- Options table:
-- {
--    circular = true | false, default = true
-- }
--

zepha.serialize = function(data, opt, path, table_refs)
    if not path then path = '"root"' end
    if not opt then opt = {} end

    local circular = opt.circular == nil and true or opt.circular
    if not table_refs then table_refs = { [path] = data } end

    local data_type = type(data)

    if data_type == 'table' then
        if table_refs and table_refs[data] then
            return circular and ('#REF[' .. table_refs[data] .. ']') or 'nil' end
        table_refs[data] = path

        local values = {}
        for k, v in pairs(data) do
            local key = zepha.serialize(k, opt, '', table_refs)
            local value = zepha.serialize(v, opt, path .. '.' .. key, table_refs)
            table.insert(values, key .. ' = ' .. value)
        end
        return '{ ' .. table.concat(values, ', ') .. ' }'
    end

    if data_type == 'number' then return tostring(data) end
    if data_type == 'string' then return '"' .. data:gsub('"', '\"') .. '"' end
    if data_type == 'boolean' then return data and 'true' or 'false' end

    if data_type ~= 'nil' then print('[ERR] Invalid serialization type: ' .. data_type) end
    return 'nil'
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

zepha.deserialize = function(str, opt, path, table_refs)
    if not table_refs then table_refs = {} end
    if not path then path = '"root"' end
    if not opt then opt = {} end

    str = str:trim(str)

    if str:sub(1, 1) == '{' then
        local tbl = {}
        str = str:sub(2, #str - 1):trim()

        table_refs[path] = tbl

        while #str > 0 do
            local sep, key, val

            if str:sub(1, 1) == ',' then str = str:sub(2):trim() end

            if str:sub(1, 1) == '{' then
                -- Handling a table key
                local e = find_match(str, '{', '}')
                local tbl_key = str:sub(1, e)
                key = zepha.deserialize(tbl_key, opt, path, table_refs)
                str = str:sub(str:find('=', e + 1) + 1):trim()
            else
                -- Handling a normal key
                local end_ind = str:find('=')
                key = zepha.deserialize(str:sub(1, end_ind - 1), opt, path, table_refs)
                str = str:sub(end_ind + 1):trim()
            end

            if str:sub(1, 1) == '{' then
                -- Handling a table value
                local e = find_match(str, '{', '}')
                local tbl_val = str:sub(1, e)
                val = zepha.deserialize(tbl_val, opt, path .. '."' .. key .. '"', table_refs)
                str = str:sub(e + 1):trim()
            else
                -- Handling a normal value
                local end_ind = str:find(',')
                val = zepha.deserialize(str:sub(1, (end_ind or #str + 1) - 1),
                    opt, path .. '."' .. key .. '"', table_refs)
                str = str:sub(end_ind or #str + 1):trim()
            end

            tbl[key] = val
        end

        return tbl
    end

    if str:find('^"') then return str:sub(2, #str - 1) end
    if str:find('^true') then return true end
    if str:find('^false') then return false end
    if str:find('^nil') then return nil end
    if str:find('^#REF%[') then
        local end_ind = (str:find(']') or 7) - 1
        return table_refs[str:sub(6, end_ind)]
    end

    return tonumber(str)
end