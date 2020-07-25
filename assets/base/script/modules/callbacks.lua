zepha.registered_callbacks = {}

function zepha.bind(event, fn)
    if zepha.registered_callbacks[event] == nil then zepha.registered_callbacks[event] = {} end
    table.insert(zepha.registered_callbacks[event], fn)
end

function zepha.trigger(event, ...)
    if zepha.registered_callbacks[event] == nil then return nil end

    for _, EVT_CALLBACK in pairs(zepha.registered_callbacks[event]) do
        if (type(EVT_CALLBACK) == "function") then EVT_CALLBACK(...) end
    end
end