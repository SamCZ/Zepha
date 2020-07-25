zepha.delayed_functions = {}

function zepha.after(fn, timeout)
    table.insert(zepha.delayed_functions, {fn = fn, timeout = timeout, at = zepha.time.s()})
end

function zepha.__builtin.update_delayed_functions()
    local time = zepha.time.s()
    for k, v in pairs(zepha.delayed_functions) do
        if v.at <= time then
            local redo = v.fn()
            if redo then v.at = time + v.timeout
            else table.remove(zepha.delayed_functions, k) end
        end
    end
end