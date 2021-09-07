zepha.registered_callbacks = {}

function zepha.bind(event, fn)
    if zepha.registered_callbacks[event] == nil then zepha.registered_callbacks[event] = {} end
    if type(fn) ~= "function" then return end
    table.insert(zepha.registered_callbacks[event], fn)
end

function zepha.trigger(event, ...)
    if zepha.registered_callbacks[event] == nil then return nil end

    for i, EVENT_CALLBACK in pairs(zepha.registered_callbacks[event]) do
        EVENT_CALLBACK(...)
    end
end