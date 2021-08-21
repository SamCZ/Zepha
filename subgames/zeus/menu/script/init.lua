local menu = zepha.gui(function()
    return Gui.Box {
        background = 'zeus_background_christmas_night',

        Gui.Box {
--             id = 'particle_wrap',
            size = { '100cw', '100ch' }
        },

        Gui.Box {
            gap = 4,
            padding = 8,
            size = { 102, '100ch' },
            pos = { '20cw - 50sw', 0 },

            background = '#0135',

            Gui.Box {
                size = { nil, 30 },
                margin = { 0, 0, 0, 8 },

                background = 'zeus_logo'
            },

            Gui.Box {
--                 id = 'button_play',

--                 callbacks = {
--                     primary = function() zepha.start_game_local() end
--                 },

                padding = 5,
                size = { nil, 20 },

                cursor = "pointer",
                background = 'crop(0, 0, 90, 20, zeus_button)',
                background_hover = 'crop(0, 20, 90, 20, zeus_button)',

                Gui.Text {
                    content = 'Local Play'
                }
            },

            Gui.Box {
--                 id = 'button_servers',

--                 callbacks = {
--                     primary = function() zepha.start_game() end
--                 },

                padding = 5,
                size = { nil, 20 },

                cursor = "pointer",
                background = 'crop(0, 0, 90, 20, zeus_button)',
                background_hover = 'crop(0, 20, 90, 20, zeus_button)',

                Gui.Text {
                    content = 'Browse Servers'
                }
            }
        }
    }
end)

-- zepha.after(function()
--     menu:remove("buttonPlay")
--     menu:remove("buttonServers")
--     menu:get("sidebar"):append(function(m) return Gui.Text({
--         position = {16, 120 },
--         color = "#f00",
--         content = "YOU LOSE" }
--     ) end)
-- end, 1)

-- local particle_wrap = menu:get('particle_wrap')
-- menu(function()
--     for _ = 1, 20 do
--         local scale = 6 + math.random() * 4
--         particle_wrap:append(Gui.Rect {
--             pos = { math.floor(math.random() * 600), math.floor(math.random() * 320) },
--             background = 'particle_dark',
--             size = { scale, scale }
--         })
--     end
-- end)

-- local tick = 0
-- zepha.after(function()
--     local i = 1
--     local part = particle_wrap:get(i)
--     tick = tick + 0.012
--
--     while part ~= nil do
--         local xO = (-math.sin(tick) * 0.0125 - 0.0025) * part.size[1]
--         local pos = { part.pos[1] + xO, part.pos[2] + 0.05 * part.size[1] }
--
--         if pos[2] > 320 then pos[2] = -12 end
--         if pos[1] < -12 then pos[1] = 600 end
--
--         i = i + 1
--         part.pos = pos
--         part = particle_wrap:get(i)
--     end
--
--     return true
-- end, 0.016)

zepha.set_gui(menu)
