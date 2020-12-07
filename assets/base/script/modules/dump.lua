-- Zepha Dump Function
-- Version 1.0

--
-- Pretty-prints a lua value, properly displaying tables
-- with indentation, table collapsing, and more.
--
-- @param {any} val - The lua value to print.
-- @param {number | nil | false} nl - Controls the table expansion behavior. If left unspecified,
--   tables will expand if they contain more than 3 keys. If set to a number n,
--   tables will expand if they contain more than n keys, if set to false, tables will never expand.
-- @param {number | nil} idn - Internal value, do not assign manually.
--

_G["format"] = function(val, nl, idn)
    idn = idn or 0
    if nl == nil then nl = 3 end

    if type(val) == "function" or type(val) == "userdata" or type(val) == "thread" then return tostring(val) end
    if type(val) == "string" then return '"' .. val .. '"' end
    if type(val) == "nil" then return 'nil' end
    if type(val) == "table" then
        local strings = {}

        local num_keys = 0
        for _ in pairs(val) do num_keys = num_keys + 1 end

        local use_nl = nl == true and true or (type(nl) == 'number' and num_keys >= nl)
        local new_idn = use_nl and idn + 1 or idn
        local delim = use_nl and '\n' or ' '

        for k, v in pairs(val) do
            table.insert(strings, format(k, nl, new_idn) .. ' = ' .. format(v, nl, new_idn))
        end

        local outer_idn = ''
        if use_nl and idn then for i = 1, idn do outer_idn = outer_idn .. '\t' end end
        local inner_idn = use_nl and outer_idn .. '\t' or ''

        return '{' .. delim .. inner_idn .. table.concat(strings, ',' .. delim .. inner_idn) .. delim .. outer_idn .. '}'
    end
    return val
end

_G["dump"] = function(val, nl)
    print(format(val, nl))
end