local max_messages = 8

local chat_wrap = zepha.build_gui(function()
    return Gui.Rect {
        position = { 4, '100%' },
        position_anchor = { 0, '200%' },
        size = { 256, 23 + max_messages * 8 },

        Gui.Rect {
            key = 'chat_tabs',
            position = { 0, -10 }
        },

        Gui.Rect {
            key = 'chat_box',
            size = { 256, 2 + max_messages * 8 }
        },

        Gui.Rect {
            key = 'chat_input',
            size = { 256, 10 },
            position = { 0, 3 + max_messages * 8 }
        }
    }
end)

local chat_box = chat_wrap:get('chat_box')
local chat_tabs = chat_wrap:get('chat_tabs')
local chat_input = chat_wrap:get('chat_input')

-- Rerenders the chat gui.
chat._refresh = function()
    chat_box:clear()
    chat_tabs:clear()

    if chat.open then
        local i = 0
        for _, identifier in ipairs(chat.channel_order) do
            local channel = chat.channels[identifier]
            chat_tabs:append(function()
                return Gui.Rect {
                    size = { 48, 10 },
                    position = { i * 49, 0 },
                    background = (chat.current_channel == identifier) and '#0005' or '#0002',

                    Gui.Rect {
                        position = { 2, 2 },
                        size = { 8 * (2/3), 8 * (2/3) },
                        background = 'multiply(' .. channel.icon .. ', ' ..
                            (chat.current_channel == identifier and '#ffe791' or '#fff') .. ')'
                    },

                    Gui.Text {
                        color = (chat.current_channel == identifier) and '#ffe791' or '#fff',
                        position = { 8, 2 },
                        scale = { 2.02/3, 2.02/3 },
                        content = channel.name
                    }
                }
            end)
            i = i + 1
        end
    end

    local channel = chat.channels[chat.current_channel]
    if not channel then return end

    local now = zepha.time.s()
    local count = 0

    local start = math.max(chat.open and 1 or channel._current_history_head, #channel.history - max_messages + 1)

    for i = #channel.history, start, -1 do
        local message = channel.history[i]
        if now - message.time > chat._message_persist_time and not chat.open then
            channel._current_history_head = i + 1
        else
            chat_box:append(function()
                return Gui.Text {
                    position = { 2, 2 + (max_messages - count - 1) * 8 },
                    background = chat.open and '#0000' or '#0003',
                    scale = { 2.02/3, 2.02/3 },
                    content = '`c7[` `r' .. message.from .. '`r` `c7]`cr ' .. message.content .. '`r'
                }
            end)
            count = count + 1
        end
    end

    chat_box.background = chat.open and '#0005' or '#0000'
    chat_input.background = chat.open and '#0005' or '#0000'
end

zepha.player:get_hud():append(chat_wrap)
chat._refresh()

-- Keyboard shortcut to toggle the chat.
zepha.register_keybind(":open_chat", {
    description = "Open Chat",
    default = zepha.keys.p,
    on_press = chat.set_open
})