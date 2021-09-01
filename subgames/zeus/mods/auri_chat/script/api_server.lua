-- Creates a new chat channel with the name and options provided.
chat.create_channel = function(identifier, options)
    chat.channels[identifier] = {
        name = options.name or identifier,
        icon = options.icon,
        history = {},
    }

    table.insert(chat.channel_order, identifier)

    zepha.send_message('@auri:chat:new_channel', {
        identifier = identifier,
        name = options.name or identifier,
        icon = options.icon,
        order = chat.channel_order
    })
end

-- Sends a message from the sender specified to the chat channel.
chat.send = function(sender, message)
    chat.send_channel(sender, nil, message)
end

-- Sends a message from the sender specified to the channel provided.
chat.send_channel = function(sender, channel_name, message)
    if not channel_name then channel_name = 'chat' end
    local channel = chat.channels[channel_name]
    if not channel then return end

    table.insert(channel.history, {
        from = sender,
        content = message,
        time = zepha.time.s()
    })

    zepha.send_message('@auri:chat:message', {
        channel = channel_name,
        from = sender,
        content = message
    })
end

zepha.bind('player_join', function(player)
    for _, identifier in ipairs(chat.channel_order) do
        local channel = chat.channels[identifier]
        zepha.send_message('@auri:chat:new_channel', {
            identifier = identifier,
            name = channel.name or identifier,
            icon = channel.icon,
            order = chat.channel_order
        })
    end
end)

zepha.bind('message', function(channel, message, player)
    if not channel:starts_with('@auri:chat') then return end

    if channel == '@auri:chat:message' then
        local user = player:get_username()
        local color = tonumber(user:sub(16, 16)) + 4
        chat.send_channel('`b`c' .. color .. user, message.channel, message.content)
    end
end)