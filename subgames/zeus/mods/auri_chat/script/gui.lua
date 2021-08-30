local max_messages = 8
local chat_pos = { '4dp', '100ch - 100sh - 4dp - 80dp' }
local chat_size = { '256dp', 23 + max_messages * 8 .. 'dp' }

local chat_hud = zepha.Gui.Box {
    pos = chat_pos,
    size = chat_size,
}

local chat_menu = zepha.Gui.Box {
    pos = chat_pos,
    size = chat_size,

    zepha.Gui.Box {
        id = 'chat_tabs',
        pos = { '0', '-10dp' }
    },

    zepha.Gui.Box {
        id = 'chat_box',
        size = { '256dp', 2 + max_messages * 8 .. 'dp' }
    },

    zepha.Gui.Box {
        id = 'chat_input',
        size = { '256dp', '10dp' },
        pos = { '0dp', 3 + max_messages * 8 .. 'dp' }
    }
}

local chat_menu_box = chat_menu:get('chat_box')
local chat_menu_tabs = chat_menu:get('chat_tabs')
local chat_menu_input = chat_menu:get('chat_input')

-- Rerenders the chat gui.
chat._refresh = function()
    chat_hud:clear()
    chat_menu_box:clear()
    chat_menu_box.background = chat.open and '#0005' or '#0000'
    chat_menu_input.background = chat.open and '#0005' or '#0000'

    if chat.open then
        chat_menu_tabs:clear()

        local i = 0
        for _, identifier in ipairs(chat.channel_order) do
            local channel = chat.channels[identifier]
            chat_menu_tabs:append(Gui.Box {
                size = { '48dp', '10dp' },
                pos = { i * 49 .. 'dp', '0dp' },
                background = (chat.current_channel == identifier) and '#0005' or '#0002',

                zepha.Gui.Box {
                    pos = '2dp',
                    size = { '8px * 2', '8px * 2' },
                    background = 'multiply(' .. channel.icon .. ', ' ..
                        (chat.current_channel == identifier and '#ffe791' or '#fff') .. ')'
                },

                zepha.Gui.Text {
                    text_size = '2px',
                    pos = { '8dp', '2dp' },
                    content = channel.name,
                    text_color = (chat.current_channel == identifier) and '#ffe791' or '#fff',
                }
            })
            i = i + 1
        end
    end

    local channel = chat.channels[chat.current_channel]
    if not channel then return end

    local now = zepha.time.s()
    local count = 0

    local start = math.max(chat.open and 1 or channel._current_history_head, #channel.history - max_messages + 1)
    local append_to = chat.open and chat_menu_box or chat_hud
    append_to:clear()

    for i = #channel.history, start, -1 do
        local message = channel.history[i]
        if now - message.time > chat._message_persist_time and not chat.open then
            channel._current_history_head = i + 1
        else
            append_to:append(zepha.Gui.Text {
                text_size = '2px',
                background = chat.open and '#0000' or '#0003',
                pos = { '2dp', 2 + (max_messages - count - 1) * 8 .. 'dp' },
                content = '`c1[` `r' .. message.from .. '`r` `c1]`cr ' .. message.content .. '`r'
            })
            count = count + 1
        end
    end
end

zepha.player:get_hud():append(chat_hud)
chat._refresh()

-- Toggles the chat being open and listening for input.
chat.set_open = function(open)
    if zepha.player.menu ~= nil and zepha.player.menu ~= chat_menu then return end

    if open == nil then chat.open = not chat.open
    else chat.open = open end

    zepha.player.menu = chat.open and chat_menu or nil

    chat._refresh()
end

-- Keyboard shortcut to toggle the chat.
zepha.register_keybind(":open_chat", {
    description = "Open Chat",
    default = zepha.keys.p,
    on_press = chat.set_open
})