runfile(_PATH .. 'api')
if zepha.client then runfile(_PATH .. 'gui') end

if zepha.server then
    chat.create_channel('chat', { name = 'Chat', icon = '@auri:chat:chat_icon' })
end

runfile(_PATH .. 'test')