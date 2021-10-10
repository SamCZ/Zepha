-- Zepha String Library Extension
-- Version 1.0

-- string.trim
-- Returns a new string with whitespace removed.
string.trim = function(str)
    return str:match('^%s*(.-)%s*$')
end

-- string.split
-- Splits a string by a delimiter, optionally applying an operation to it after separating it.
string.split = function(input, sep, op)
    if sep == nil then sep = '%s' end
    local t = {}
    for str in string.gmatch(input, '([^'..sep..']+)') do
        table.insert(t, op and op(str) or str) end
    return t
end

-- string.escape
-- Escapes a string, replacing all graves with grave literals.
string.escape = function(str)
    return str:gsub('%`', '``')
end

-- string.starts_with
-- Checks if a string starts with the specified substring.
string.starts_with = function(str, substr)
    return str:sub(1, substr:len()) == substr
end