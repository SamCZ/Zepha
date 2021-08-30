chat.open = false
chat.current_channel = nil
chat._message_persist_time = 5

-- Sends a message from the player to the chat channel.
chat.send = function(message)
    chat.send_channel(nil, message)
end

-- Sends a message from player to the channel provided.
chat.send_channel = function(channel_name, message)
    zepha.send_message('@auri:chat:message', {
        channel = channel_name,
        content = message
    })
end

local function add_channel(channel)
    chat.channels[channel.identifier] = {
        name = channel.name,
        icon = channel.icon,
        history = {},
        _current_history_head = 1
    }

    chat.channel_order = channel.order
    if chat.current_channel == nil then chat.current_channel = channel.identifier end
    chat._refresh()
end

local function add_message(message)
    local channel = chat.channels[message.channel]
    if not channel then return end

    table.insert(channel.history, {
        from = message.from,
        content = message.content,
        time = zepha.time.s()
    })

    chat._refresh()
    zepha.after(chat._refresh, chat._message_persist_time)
end

zepha.bind('message', function(channel, message)
    if not channel:starts_with('@auri:chat') then return end

    if channel == '@auri:chat:new_channel' then add_channel(message) end
    if channel == '@auri:chat:message' then add_message(message) end
end)