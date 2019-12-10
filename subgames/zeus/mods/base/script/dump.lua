-- Zepha Dump Function
-- Version 1.0

-- dumpTbl [private]
-- Recursive function to print the contents of a table.
local function dumpTbl(tbl, indent, scannedTables)
    if not scannedTables then scannedTables = {} end
    if not indent then indent = 0 end

    scannedTables[tbl] = true

    for k, v in pairs(tbl) do
        if type(k) == "number" then
            k = "[" .. k .. "]"
        end

        local indentString = string.rep("  ", indent)
        local formatting = indentString .. k .. " = "

        if type(v) == "function" then
            v = "<function>"
        elseif type(v) == "userdata" then
            v = "<userdata>"
        elseif type(v) == "string" then
            v = '"' .. v .. '"'
        elseif type(v) == "boolean" then
            v = tostring(v)
        end

        if type(v) == "table" then
            if scannedTables[v] ~= nil then
                print(formatting .. "<circular>")
            else
                print(formatting .. "{")
                dumpTbl(v, indent + 1, scannedTables)
                print(indentString .. "}")
            end
        else
            print(formatting .. v)
        end
    end
end

-- dump
-- Prints a human readable form of a value.
_G["dump"] = function(val)
    if type(val) == "table" then
        print("{")
        dumpTbl(val, 1)
        print("}")
        return
    end
    if type(val) == "string" then print('"'..val..'"') return end
    print(val)
    return
end