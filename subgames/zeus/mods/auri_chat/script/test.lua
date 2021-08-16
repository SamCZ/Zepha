if zepha.server then
    chat.create_channel('commands', { name = 'Commands', icon = '@auri:chat:commands_icon' })
    chat.create_channel('debug', { name = 'Debug', icon = '@auri:chat:debug_icon' })
--
--     local random_messages = {
--         { '`b`cbA`cdu`cfr`cdi`cb!', 'the quick brown fox jumps over the lazy dog' },
--         { '`b`cbA`cdu`cfr`cdi`cb!', 'lorum ipsum dolor sit amet consequitor lorem ipsum dolor sit amet' },
--         { '`b`cbZ`cay`cet`cch`cdi`c5a', '`iomg shut the fuck up`r' },
--         { '`b`cbA`cdu`cfr`cdi`cb!', 'lol epic' },
--         { '`b`cbA`cdu`cfr`cdi`cb!', 'chat mod' },
--         { '`b`cbZ`cay`cet`cch`cdi`c5a', 'testing testing 123' },
--         { '`bJAYCEE', 'PENIS!' },
--         { '`b`cbA`cdu`cfr`cdi`cb!', '#epic' },
--         { 'alphabet', '`bABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789' }
--     }
--
--     local function random_message()
--         local message = random_messages[math.floor(math.random() * #random_messages) + 1]
--         chat.send(message[1], message[2])
--         zepha.after(random_message, math.random() > 0.5 and math.random() * 1 or math.random() * 2)
--     end
--     random_message()
elseif zepha.client then
    zepha.register_keybind(":send_test_message", {
        description = "Send test message",
        default = zepha.keys.j,
        on_press = function() chat.send('hihihih') end
    })
end