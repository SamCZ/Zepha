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
    if sep == nil then sep = "%s" end
    local t = {}
    for str in string.gmatch(input, '([^'..sep..']+)') do
        table.insert(t, op and op(str) or str) end
    return t
end